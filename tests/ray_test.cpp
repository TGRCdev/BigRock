#include <iostream>
#include "ray.h"
#include "vector3.h"

using namespace std;
using namespace bigrock;

int main(int argc, char const *argv[])
{
    Ray test_ray(Vector3(0,0,0), Vector3(0,1,0));
    cout << "Constructing ray with origin (0,0,0) and direction (0,1,0)" << endl;
    cout << "Getting point at length 2.3 on the ray" << endl;
    Vector3 p = test_ray.get_point_on_ray(2.3);
    cout << "test_ray.get_point_on_ray(2.3) = (" << p.x << ", " << p.y << ", " << p.z << ")" << endl;
    cout << "Getting test_ray.get_closest_point(Vector3(1, 0.7, 0))" << endl;
    Vector3 point(1, 0.7, 0);
    Vector3 closest = test_ray.get_closest_point(point);
    cout << "Expected: (0, 0.7, 0) Actual: (" << closest.x << ", " << closest.y << ", " << closest.z << ")" << endl;
    return 0;
}
