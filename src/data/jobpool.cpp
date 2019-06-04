#include "jobpool.hpp"

#define THREAD_IMPLEMENTATION
#include "thread.h"

#if defined(unix) || defined(__unix__) || defined(__unix)
#include <unistd.h>
#else
#include <windows.h>
#endif

namespace bigrock {
namespace data {

int JobPool::get_number_of_cores()
{
#if defined( _WIN32 )
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    int nmCpu = sysinfo.dwNumberOfProcessors;
    if(nmCpu <= 0)
        return 1;
    else
        return nmCpu;
#elif defined(unix) || defined(__unix__) || defined(__unix) // People are saying that some compilers don't define any of these, god I hope not.
    int nmCpu = sysconf(_SC_NPROCESSORS_ONLN);
    return (nmCpu > 0 ? nmCpu : 1);
#elif defined( __APPLE__ ) || defined( __BSD__ )
    int mib[4];
    int numCPU;
    std::size_t len = sizeof(numCPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &numCPU, &len, NULL, 0);

    if (numCPU < 1) 
    {
        mib[1] = HW_NCPU;
        sysctl(mib, 2, &numCPU, &len, NULL, 0);
        if (numCPU < 1)
            numCPU = 1;
    }
    return numCPU; // what the hell, unix
#else
    #pragma message "Couldn't find the target operating system, number of cores will always be one."
    return 1;
#endif
}

JobPool::worker_t::worker_t()
{
    thread_atomic_int_store(&working, 0);
    workthread = NULL;
}

JobPool::worker_t::~worker_t()
{
    if(workthread)
    {
        thread_join(workthread);
        thread_destroy(workthread);
    }
}

JobPool::job_t::job_t()
{
    func = NULL;
    userdata = NULL;
}

JobPool::job_t::job_t(const job_t &other)
{
    this->func = other.func;
    this->userdata = other.userdata;
}

JobPool::worker_t::worker_thread_data::~worker_thread_data()
{
    delete job;
}

bool JobPool::worker_t::start(job_t &job, JobPool *pool)
{
    if(!job.func || is_working())
        return false;
    
    thread_atomic_int_store(&working, 1);

    if(workthread)
    {
        thread_join(workthread);
        thread_destroy(workthread);
    }
    
    worker_thread_data *data = new worker_thread_data();
    data->job = new job_t(job);
    data->pool = pool;
    data->worker = this;
    
    workthread = thread_create(&worker_thread, data, NULL, THREAD_STACK_SIZE_DEFAULT);
    
    return true;
}

int JobPool::worker_t::worker_thread(void *userdata)
{ // userdata is info about the worker, job and pool
    worker_thread_data *data = reinterpret_cast<worker_thread_data*>(userdata);
    if(!data || !data->job || !data->worker)
    {
        delete data;
        thread_atomic_int_store(&data->worker->working, 0);
        thread_signal_raise(&data->pool->boss_signal);
        return 1;
    }
    bool has_job = true;
    
    while(has_job)
    {
        if(!data->job || !data->job->func)
            has_job = false;
        else
            data->job->func(data->job->userdata);

        // check for additional work if a pool was specified
        if(data->pool)
        {
            thread_mutex_lock(&data->pool->jobs_mutex);
            if(!data->pool->jobs.empty())
            {
                delete data->job;
                data->job = new job_t(data->pool->jobs.top());
                data->pool->jobs.pop();
            }
            else
            {
                has_job = false;
            }
            thread_mutex_unlock(&data->pool->jobs_mutex);
        }
        else
        {
            has_job = false;
        }
    }

    // Return self to idle state
    thread_atomic_int_store(&data->worker->working, 0);
    delete data;
    thread_signal_raise(&data->pool->boss_signal);
    return 0;
}

JobPool::JobPool() : worker_count(get_number_of_cores())
{
    thread_atomic_int_store(&ending, 0);
    thread_atomic_int_store(&idle, 1);
    workers = new worker_t[worker_count];
    
    thread_signal_init(&boss_signal);
    thread_signal_init(&jobs_done);
    thread_mutex_init(&jobs_mutex);
    
    boss = thread_create(boss_thread, this, NULL, THREAD_STACK_SIZE_DEFAULT);
}

JobPool::~JobPool()
{
    wait_until_empty();

    thread_atomic_int_store(&ending, 1);
    thread_signal_raise(&boss_signal); // Boss will see ending is set to true and terminate

    delete[] workers;

    thread_signal_term(&boss_signal);
    thread_signal_term(&jobs_done);
    thread_mutex_term(&jobs_mutex);
}

void JobPool::add_job(job_t job)
{
    thread_mutex_lock(&jobs_mutex);
    jobs.push(job);
    thread_mutex_unlock(&jobs_mutex);
    thread_atomic_int_store(&idle, 0);
    thread_signal_raise(&boss_signal);
}

void JobPool::add_job(int(*func)(void*), void* userdata)
{
    job_t dat;
    dat.func = func;
    dat.userdata = userdata;
    add_job(dat);
}

void JobPool::add_jobs(job_t* jobarr, int num_jobs)
{
    if(!jobarr || num_jobs <= 0)
        return;
    
    thread_mutex_lock(&jobs_mutex);
    for(int i = 0; i < num_jobs; i++)
        jobs.push(jobarr[i]);
    thread_mutex_unlock(&jobs_mutex);

    thread_signal_raise(&boss_signal);
}

void JobPool::wait_until_empty()
{
    if(thread_atomic_int_load(&idle) == 0)
        thread_signal_wait(&jobs_done, THREAD_SIGNAL_WAIT_INFINITE);
}

int JobPool::boss_thread(void *userdata)
{
    JobPool *pool = reinterpret_cast<JobPool*>(userdata);
    while(thread_atomic_int_load(&pool->ending) == 0)
    {
        thread_signal_wait(&pool->boss_signal, THREAD_SIGNAL_WAIT_INFINITE); // Wait for wakeup signal
        // Check if there are jobs
        thread_mutex_lock(&pool->jobs_mutex);
        bool idle_workers = true;
        if(!pool->jobs.empty())
        {
            while(!pool->jobs.empty() && idle_workers)
            { // Jobs need workers, look for an open worker
                worker_t *worker = NULL;
                for(int i = 0; !worker && i < pool->worker_count; i++)
                {
                    if(!pool->workers[i].is_working())
                        worker = pool->workers + i;
                }

                if(worker) // A worker was available for work
                {
                    worker->start(pool->jobs.top(), pool);
                    pool->jobs.pop();
                }
                else
                {
                    idle_workers = false; // No workers are idle
                }
            }
            // Either all jobs were assigned, or no idle workers were found
        }
        else // No jobs, check if all workers are idle
        {
            if(thread_atomic_int_load(&pool->idle) == 0) // work was previously being done
            {
                bool working = false;

                for(int i = 0; !working && i < pool->worker_count; i++)
                    working = pool->workers[i].is_working();
                
                if(!working)
                {
                    thread_atomic_int_store(&pool->idle, 1);
                    thread_signal_raise(&pool->jobs_done);
                }
            }
        }
        thread_mutex_unlock(&pool->jobs_mutex);
    }
    return 0;
}

}}