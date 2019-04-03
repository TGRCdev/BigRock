#include "../terrain/data/terrain.hpp"
#include "../terrain/data/tool.hpp"
#include "../terrain/data/action.hpp"
#include "../terrain/mesh/marching_cubes.hpp"
#include "../math/vector3.hpp"

#include <fstream>
#include <iostream>

using namespace std;
using namespace bigrock;

int main()
{
    cout << "BigRock 0.2 Mesh Test" << endl;
    terrain::Cell c;
    cout << "Cell created" << endl;
    terrain::EllipsoidTool t;
    terrain::EmplaceAction a;
    a.material = 1;
    t.transform.scale = math::Vector3(0.5,0.5,0.5);
    c.apply_tool(t, a);
    cout << "Action applied" << endl;
    terrain::Mesh mesh = terrain::MarchingCubes().polygonise(c, 16);
    cout << "Mesh polygonised" << endl;

    fstream out;
    out.open("./test_mesh.obj", fstream::out);

    cout << "File opened" << endl;

    out << "# BigRock 0.2 Test Mesh" << endl;

    for(int i = 0; i < mesh.vertices.size(); i++)
    {
        terrain::Mesh::Vertex vert = mesh.vertices[i];
        out << "v " << vert.position.x << " " << vert.position.y << " " << vert.position.z << endl;
        out << "vn " << vert.normal.x << " " << vert.normal.y << " " << vert.normal.z << endl;
    }

    for(int i = 0; i < mesh.material_indices[0].size(); i += 3)
    {
        out << "f " << mesh.material_indices[0][i  ] + 1;
        out << " " << mesh.material_indices[0][i+1] + 1;
        out << " " << mesh.material_indices[0][i+2] + 1;
        out << endl;
    }

    out.close();
    cout << "File closed" << endl;
    return 0;
}