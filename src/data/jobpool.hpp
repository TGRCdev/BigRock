#ifndef BIGROCK_JOBPOOL_H
#define BIGROCK_JOBPOOL_H
#pragma once

#include <stack>

#include "thread.h"

namespace bigrock {
namespace data {

class JobPool
{
    #if !BR_DISABLE_MULTITHREADING
public:
    typedef int (*job_func_t)(void*);

    struct job_t
    {
        job_t();
        job_t(const job_t &other);
        
        void* userdata;
        job_func_t func;
    };

    struct worker_t
    {
        private:

        struct worker_thread_data
        {
            ~worker_thread_data();

            worker_t *worker;
            job_t *job;
            JobPool *pool;
        };

        static int worker_thread(void *userdata);

        thread_ptr_t workthread;
        thread_atomic_int_t working;

        public:
        worker_t();
        ~worker_t();

        bool start(job_t &job, JobPool *pool);
        bool is_working() {return thread_atomic_int_load(&working) != 0;}
    };

    

private:
    thread_atomic_int_t ending; // Set to 1 in destructor
    thread_atomic_int_t idle; // Set to 1 before jobs_done is raised, 0 when work is added

    std::stack<job_t> jobs; // TODO: Object pooling?
    thread_signal_t jobs_done; // Raised when the stack goes empty and all workers are idle
    thread_mutex_t jobs_mutex;

    thread_ptr_t boss; // Distributes jobs to workers
    thread_signal_t boss_signal; // Raised when either jobs are added, or workers go idle

    // Works on jobs, size is number of cores
    // When a worker's pointer is NULL, that worker is considered idle
    const unsigned char worker_count;
    worker_t *workers;

    static int boss_thread(void *userdata); // userdata is the JobPool the boss owns

    #endif
public:
    /// Guaranteed to return at least 1
    static int get_number_of_cores();

    #if !BR_DISABLE_MULTITHREADING

    JobPool(const unsigned char thread_count = get_number_of_cores());

    // Waits for all current jobs to complete before destroying workers and boss.
    ~JobPool();

    // Adds a job to the stack and notifies the boss thread
    void add_job(job_t job);
    void add_job(job_func_t func, void* userdata);

    // Acquires the job stack's lock and adds all the jobs
    void add_jobs(job_t* jobs, int num_jobs);

    template<class ConstIterator>
    void add_jobs(ConstIterator &start, ConstIterator &end);

    // Blocks the calling thread until all jobs (including ones added by existing jobs) have been completed
    void wait_until_empty();

    friend class worker_t;
    #endif
};

}}

#endif // BIGROCK_JOBPOOL_H