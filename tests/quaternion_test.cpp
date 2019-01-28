#include "quaternion.h"
#include "vector3.h"
#include "math_funcs.h"

using namespace std;
using namespace bigrock;

int main(int argc, char const *argv[])
{
    Quaternion quat;
    cout << "Created Quaternion" << endl;
    cout << "Rotating Z axis by 45 degrees" << endl;
    quat.rotate_euler(Vector3(MathFuncs::degrees_to_radians(45), 0, 0));
    cout << "Rotated" << endl;
    cout << "Right: " << quat.get_right() << " Up: " << quat.get_up() << " Forward: " << quat.get_forward() << endl;
    cout << "is_normalized = " << quat.is_normalized() << endl;
    Vector3 rel = Vector3(-1, 0, 0);
    cout << "Transforming point " << rel << endl;
    Vector3 point = quat.xform(rel);
    cout << "Resulting point: " << point << endl;
    cout << "Reversing transform on point" << endl;
    cout << "Resulting point: " << quat.xform_inv(point) << endl;
    return 0;
}
