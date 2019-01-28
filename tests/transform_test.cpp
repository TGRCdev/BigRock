#include "transform.h"
#include "vector3.h"

using namespace std;
using namespace bigrock;

int main(int argc, char const *argv[])
{
    cout << "Transform Test" << endl;
    Transform t;
    cout << "Constructed Transform" << endl;
    Vector3 origin(3.5, 8, 2);
    cout << "Setting origin to " << origin << endl;
    t.origin = origin;
    cout << "Origin set" << endl;
    Vector3 rel(1, 0, 0);
    cout << "Getting global position of " << rel << endl;
    cout << "Result: " << t.to_global(rel) << endl;
    Vector3 scale(0.5, 1, 1);
    cout << "Setting scale to " << scale << endl;
    t.scale = scale;
    cout << "Scale set" << endl;
    cout << "Getting global position of " << rel << endl;
    cout << "Result: " << t.to_global(rel) << endl;
    cout << "Getting relative position of " << origin << endl;
    cout << "Result: " << t.to_local(origin) << endl;
    cout << "Rotating Transform 90 degrees on the Y axis" << endl;
    t.rotate_degrees(Vector3(0, 1, 0), 90);
    cout << "Resetting scale" << endl;
    t.scale = Vector3(1,1,1);
    cout << "Getting global position of " << rel << endl;
    cout << "Result: " << t.to_global(rel) << endl;
    cout << "New Transform. Origin: (0,0,0)" << endl;
    Transform t2 = Transform(Vector3());
    cout << "Rotating new transform 90 degrees on the Y axis" << endl;
    t2.rotate_degrees(Vector3(0, 1, 0), 90);
    cout << "Getting global position of " << rel << endl;
    cout << "Expected: (0, 0, -1) Actual: " << t2.to_global(rel) << endl;
    return 0;
}
