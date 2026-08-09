# GEOX

GEOX is a C++ geometry library. The `geox` namespace provides 2D and 3D primitives, algorithms, mesh topology, and VTK I/O tools.

### Primitives

- [Vectors](include/geox/primitives/vectors.h): `vector2d` and `vector3d`, with arithmetic, dot and cross products, norms, and normalization
- [Points](include/geox/primitives/points.h): `Point2` and `Point3`, with vector translation and distance calculations
- [Segments](include/geox/primitives/segments.h): `Segment2` and `Segment3`, with direction, length, and interpolation through `pointAt(t)`
- [Triangles](include/geox/primitives/triangles.h): `Triangle2` and `Triangle3`, with area, edges, degeneracy checks, and 3D normals
- [Triangle meshes](include/geox/primitives/triangle_mesh.h): `TriangleMesh`, vertex/triangle counts, index and geometric validation, per-triangle area and normals, and triangle-orientation reversal

### Mesh topology

- [Triangle topology](include/geox/topology/triangle_topology.h): edges, triangle adjacency, incident triangles, boundary and non-manifold edge detection
- [Orientation analysis](include/geox/topology/orientation.h): checks orientability and winding consistency, creates and applies a face-flip plan, reverses mesh orientation, detects closed meshes, and calculates signed volume
- [Half-edge topology](include/geox/topology/half_edge_topology.h): directed per-face edges with `next`, `previous`, and `twin` navigation, plus origin and destination vertices. It is available for manifold meshes whose adjacent triangles are consistently oriented.
- [Boundary utilities](include/geox/topology/boundary.h): collects boundary vertices and edges and extracts ordered, closed boundary loops from a half-edge topology

### Algorithms

- [2D segments](include/geox/algorithms/segment2.h): point-on-segment test, closest point, squared distance, and segment intersection
- [3D segments](include/geox/algorithms/segment3.h): point-on-segment test, closest point, squared distance, and intersection of coplanar or collinear segments; skew segments do not intersect
- [2D triangles](include/geox/algorithms/triangle2.h): barycentric coordinates and point-in-triangle test
- [3D triangles](include/geox/algorithms/triangle3.h): barycentric coordinates, point-in-triangle test, closest point on the triangle, and closest point on its boundary edges

### Mesh I/O

- [VTK export](include/geox/io/vtk.h): writes a triangle mesh as an ASCII legacy VTK `POLYDATA` file for viewing in ParaView

## Build

The project builds a C++17 library named `geox`. Examples and tests are
separate optional targets:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

For multi-configuration generators, add `-C Debug` to the `ctest` command.

```sh
cmake -S . -B build-library -DGEOX_BUILD_EXAMPLES=OFF -DGEOX_BUILD_TESTS=OFF
cmake -S . -B build-tests -DGEOX_BUILD_EXAMPLES=OFF -DGEOX_BUILD_TESTS=ON
```

Available CMake options:

- `GEOX_BUILD_EXAMPLES` — build usage examples (default: `ON`)
- `GEOX_BUILD_TESTS` — build and register CTest tests (default: `ON`)

## Usage

### Triangle operations

Include headers from `include/` and use the `geox` namespace:

```cpp
#include "geox/algorithms/triangle3.h"

using namespace geox;

Triangle3 triangle{
    Point3{0.0, 0.0, 0.0},
    Point3{1.0, 0.0, 0.0},
    Point3{0.0, 1.0, 0.0},
};

double area = triangle.area();
vector3d normal = triangle.normal();
Point3 closest = closestPointOnTriangle(Point3{0.8, 0.8, 1.0}, triangle);
```

### Triangle mesh topology

Build topology for a mesh of two adjacent triangles:

```cpp
#include "geox/primitives/triangle_mesh.h"
#include "geox/topology/triangle_topology.h"

using namespace geox;

TriangleMesh mesh{
    {
        Point3{0.0, 0.0, 0.0},
        Point3{1.0, 0.0, 0.0},
        Point3{1.0, 1.0, 0.0},
        Point3{0.0, 1.0, 0.0},
    },
    {
        TriangleIndices{0, 1, 2},
        TriangleIndices{0, 2, 3},
    },
};

TriangleTopology topology = buildTriangleTopology(mesh);
bool manifold = topology.isManifold();
std::size_t edgeCount = topology.edgeCount();
```

### Mesh orientation and boundaries

Analyze a manifold mesh before building its half-edge representation. If its face
winding is inconsistent, apply the returned correction plan and rebuild the
triangle topology. Boundary loops are available for open, consistently oriented
manifold meshes.

```cpp
#include "geox/topology/boundary.h"
#include "geox/topology/half_edge_topology.h"
#include "geox/topology/orientation.h"

#include <stdexcept>
#include <vector>

using namespace geox;

TriangleTopology topology = buildTriangleTopology(mesh);
OrientationAnalysis orientation = analyzeOrientation(mesh, topology);

if (!orientation.orientable)
    throw std::runtime_error("Mesh cannot be consistently oriented");

applyOrientation(mesh, orientation);
topology = buildTriangleTopology(mesh);

HalfEdgeTopology halfEdges = buildHalfEdgeTopology(mesh, topology);
std::vector<BoundaryLoop> loops = extractBoundaryLoops(mesh, halfEdges);

bool closed = isClosed(topology);
double volume = signedVolume(mesh);
```

### Kepler–Poinsot polyhedron: great icosahedron

The great icosahedron is a non-convex regular Kepler–Poinsot polyhedron. Its mesh has 12 vertices, 30 edges, and 20 triangular faces; five faces meet at every vertex.

The [great icosahedron demo](examples/vtk/great_icosahedron_vtk_demo.cpp) builds this closed manifold mesh, validates its topology, and exports it to VTK.

![Great icosahedron](assets/great_icosahedron.png)

After building the project, generate the VTK file with:

```sh
cmake --build build --target generate_great_icosahedron_vtk
```

This creates `build/great_icosahedron.vtk`, which can be opened, for example, in *ParaView*.
