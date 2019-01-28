#include "vector3.h"
#include <iostream>

using namespace std;
using namespace bigrock;

int main(int argc, char const *argv[])
{
    cout << "Vector3 test" << endl;
    cout << "Comparing distances between (0,0,0) and (1,1,1)" << endl;
    const float dist = 1.73205080757;
    cout << "Expected: " << dist << " Actual: " << Vector3(0,0,0).distance_to(Vector3(1,1,1)) << endl;
    cout << "Comparing squared distances between (0,0,0) and (1,1,1)" << endl;
    cout << "Expected: " << (dist * dist) << " Actual: " << Vector3(0,0,0).distance_squared_to(Vector3(1,1,1)) << endl;
    cout << "Testing is_normalized()" << endl;
    Vector3 norm(1, 1, 1);
    norm = norm.normalized();
    cout << "Normalized vector: (" << norm.x << ", " << norm.y << ", " << norm.z << ")" << endl;
    cout << "norm.length_squared(): " << norm.length_squared() << endl;
    cout << "norm.length_squared() == 1: " << (norm.length_squared() == 1) << endl;
    cout << "norm.is_normalized(): Expected: " << 1 << " Actual: " << norm.is_normalized() << endl;
    return 0;
}
