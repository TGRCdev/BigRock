#include "../../src/data/cell.hpp"
#include "../../src/data/tools/ellipsoid.hpp"
#include "../../src/data/actions/emplace.hpp"
#include "../../src/mesh/marchingcubescpu.hpp"

#include <iostream>
#include <fstream>

using namespace std;

using namespace bigrock::data;
using namespace bigrock::mesh;
using namespace bigrock::data::tools;
using namespace bigrock::data::actions;

void output_aabb_as_obj(std::ostream &out, const AABB &aabb)
{
    glm::vec3 min = aabb.getMin();
    glm::vec3 max = aabb.getMax();
    out << "v " << min.x << " " << min.y << " " << min.z << "\n"; // 1
    out << "v " << min.x << " " << max.y << " " << min.z << "\n"; // 2
    out << "v " << max.x << " " << max.y << " " << min.z << "\n"; // 3
    out << "v " << max.x << " " << min.y << " " << min.z << "\n"; // 4
    out << "v " << min.x << " " << min.y << " " << max.z << "\n"; // 5
    out << "v " << min.x << " " << max.y << " " << max.z << "\n"; // 6
    out << "v " << max.x << " " << max.y << " " << max.z << "\n"; // 7
    out << "v " << max.x << " " << min.y << " " << max.z << "\n"; // 8
    out << "f 4 3 2\n";
    out << "f 4 2 1\n";
    out << "f 8 7 6\n";
    out << "f 8 6 5\n";
    out << "f 1 5 8\n";
    out << "f 4 1 8\n";
}

int main()
{
    Ellipsoid t;
    t.transform.origin = glm::vec3(0.4f, 0.25f, 0.25f);
    t.transform.scale = glm::vec3(0.4f);
    Emplace a;
    cout << "Marching Cubes Test" << endl;
    Cell cell;
    cout << "Applying 2 Ellipsoid tools at max_depth 4...";
    cell.apply(t, a, 4);
    t.transform.origin.x = 0.6f;
    cell.apply(t, a, 4);
    cout << "Done." << endl;
    cout << "Outputting cell AABB as OBJ file...";
    fstream c_aabb_out("cell_aabb.obj", ios::out);
    output_aabb_as_obj(c_aabb_out, cell.get_aabb());
    c_aabb_out.close();
    cout << "Done." << endl;
    cout << "Outputting tool AABB as OBJ file...";
    fstream t_aabb_out("tool_aabb.obj", ios::out);
    output_aabb_as_obj(t_aabb_out, t.get_aabb());
    t_aabb_out.close();
    cout << "Done." << endl;
    cout << "Converting Cell to Marching Cubes mesh...";
    clock_t start = clock();
    std::unique_ptr<Mesh> cellmesh = MarchingCubesCPU().generate(cell);
    clock_t end = clock();
    if(!cellmesh)
    {
        cout << "Failed." << endl;
        cout << "Could not complete Marching Cubes mesh of the cell." << endl;
        return 1;
    }
    cout << "Done." << endl;
    cout << "Terrain meshing took " << (end - start) << " clocks (about " << ((end - start) / CLOCKS_PER_SEC) << " seconds)" << endl;
    #ifndef NDEBUG
    cout << "Outputting as OBJ file...";
    fstream out("terrain_mesh.obj", ios::out);
    out << *cellmesh;
    out.close();
    cout << "Done." << endl;
    #endif
    return 0;
}