# GEOX

GEOX is a C++17 geometry library. Its public API is separated into the independent `geox::dim2` and `geox::dim3` modules. Mesh topology, BVH, and VTK I/O belong to `geox::dim3`.

Use granular headers in library code, or one of the umbrella headers:

- [`geox/dim2.h`](include/geox/dim2.h) — complete 2D API
- [`geox/dim3.h`](include/geox/dim3.h) — complete 3D API
- [`geox/geox.h`](include/geox/geox.h) — both dimensions

### Primitives

- 2D [`Vector`](include/geox/dim2/primitives/vector.h), [`Point`](include/geox/dim2/primitives/point.h), [`Segment`](include/geox/dim2/primitives/segment.h), and [`Triangle`](include/geox/dim2/primitives/triangle.h) in `geox::dim2`
- 3D [`Vector`](include/geox/dim3/primitives/vector.h), [`Point`](include/geox/dim3/primitives/point.h), [`Segment`](include/geox/dim3/primitives/segment.h), and [`Triangle`](include/geox/dim3/primitives/triangle.h) in `geox::dim3`
- [Triangle meshes](include/geox/dim3/mesh/triangle_mesh.h): `geox::dim3::TriangleMesh`, with vertex/triangle counts, index and geometric validation, per-triangle area and normal, and triangle-orientation reversal

### Mesh topology

- [Triangle topology](include/geox/dim3/topology/triangle_topology.h): edges, triangle adjacency, incident triangles, boundary and non-manifold edge detection
- [Orientation analysis](include/geox/dim3/topology/orientation.h): checks orientability and winding consistency, creates and applies a face-flip plan, reverses mesh orientation, detects closed meshes, and calculates signed volume
- [Half-edge topology](include/geox/dim3/topology/half_edge_topology.h): directed per-face edges with `next`, `previous`, and `twin` navigation, plus origin and destination vertices. It is available for manifold meshes whose adjacent triangles are consistently oriented.
- [Boundary utilities](include/geox/dim3/topology/boundary.h): collects boundary vertices and edges and extracts ordered, closed boundary loops from a half-edge topology

### Algorithms

- [2D segments](include/geox/dim2/algorithms/segment.h): point-on-segment test, closest point, squared distance, and segment intersection
- [3D segments](include/geox/dim3/algorithms/segment.h): point-on-segment test, closest point, squared distance, and intersection of coplanar or collinear segments; skew segments do not intersect
- [2D triangles](include/geox/dim2/algorithms/triangle.h): barycentric coordinates and point-in-triangle test
- [3D triangles](include/geox/dim3/algorithms/triangle.h): barycentric coordinates, point-in-triangle test, closest point on the triangle, and closest point on its boundary edges
- [Triangle meshes](include/geox/dim3/algorithms/triangle_mesh.h): exact linear closest-point query over all mesh triangles

### Spatial queries

- [2D AABB](include/geox/dim2/spatial/aabb.h) and [3D AABB](include/geox/dim3/spatial/aabb.h): point containment, overlap tests, merging, and squared distance lower bounds
- [2D bounds](include/geox/dim2/spatial/bounds.h) for segments and triangles; [3D bounds](include/geox/dim3/spatial/bounds.h) additionally supports individual mesh triangles and complete triangle meshes
- [Triangle BVH](include/geox/dim3/spatial/triangle_bvh.h): a flat bounding-volume hierarchy for nearest-point queries and AABB-overlap candidate search over a `geox::dim3::TriangleMesh`

### Mesh I/O

- [VTK export](include/geox/dim3/io/vtk.h): writes a triangle mesh as an ASCII legacy VTK `POLYDATA` file for viewing in ParaView

## Build

The project exports `geox::core`, `geox::dim2`, `geox::dim3`, and the aggregate `geox::geox` CMake targets. Examples and tests are separate optional targets:

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

Install the package and consume only the required dimension:

```sh
cmake -S . -B build-library \
    -DGEOX_BUILD_EXAMPLES=OFF \
    -DGEOX_BUILD_TESTS=OFF \
    -DCMAKE_INSTALL_PREFIX=/path/to/geox
cmake --build build-library
cmake --install build-library
```

```cmake
find_package(geox 1.0 CONFIG REQUIRED)

target_link_libraries(my_2d_target PRIVATE geox::dim2)
target_link_libraries(my_3d_target PRIVATE geox::dim3)
# Or use the complete API:
target_link_libraries(my_target PRIVATE geox::geox)
```

## Usage

### 2D segment intersection

```cpp
#include "geox/dim2/algorithms/segment.h"

namespace gx2 = geox::dim2;

const gx2::Segment first{gx2::Point{0.0, 0.0}, gx2::Point{2.0, 2.0}};
const gx2::Segment second{gx2::Point{0.0, 2.0}, gx2::Point{2.0, 0.0}};
const gx2::SegmentIntersection intersection = gx2::intersect(first, second);
```

### Triangle operations

Include headers from `include/` and use the corresponding dimension namespace:

```cpp
#include "geox/dim3/algorithms/triangle.h"

namespace gx3 = geox::dim3;

gx3::Triangle triangle{
    gx3::Point{0.0, 0.0, 0.0},
    gx3::Point{1.0, 0.0, 0.0},
    gx3::Point{0.0, 1.0, 0.0},
};

double area = triangle.area();
gx3::Vector normal = triangle.normal();
gx3::Point closest = gx3::closestPointOnTriangle(
    gx3::Point{0.8, 0.8, 1.0}, triangle
);
```

### Triangle mesh topology

Build topology for a mesh of two adjacent triangles:

```cpp
#include "geox/dim3/mesh/triangle_mesh.h"
#include "geox/dim3/topology/triangle_topology.h"

namespace gx3 = geox::dim3;

gx3::TriangleMesh mesh{
    {
        gx3::Point{0.0, 0.0, 0.0},
        gx3::Point{1.0, 0.0, 0.0},
        gx3::Point{1.0, 1.0, 0.0},
        gx3::Point{0.0, 1.0, 0.0},
    },
    {
        gx3::TriangleIndices{0, 1, 2},
        gx3::TriangleIndices{0, 2, 3},
    },
};

gx3::TriangleTopology topology = gx3::buildTriangleTopology(mesh);
bool manifold = topology.isManifold();
std::size_t edgeCount = topology.edgeCount();
```

### Mesh orientation and boundaries

Analyze a manifold mesh before building its half-edge representation. If its face
winding is inconsistent, apply the returned correction plan and rebuild the
triangle topology. Boundary loops are available for open, consistently oriented
manifold meshes.

```cpp
#include "geox/dim3/topology/boundary.h"
#include "geox/dim3/topology/half_edge_topology.h"
#include "geox/dim3/topology/orientation.h"

#include <stdexcept>
#include <vector>

namespace gx3 = geox::dim3;

gx3::TriangleTopology topology = gx3::buildTriangleTopology(mesh);
gx3::OrientationAnalysis orientation = gx3::analyzeOrientation(mesh, topology);

if (!orientation.orientable)
    throw std::runtime_error("Mesh cannot be consistently oriented");

gx3::applyOrientation(mesh, orientation);
topology = gx3::buildTriangleTopology(mesh);

gx3::HalfEdgeTopology halfEdges = gx3::buildHalfEdgeTopology(mesh, topology);
std::vector<gx3::BoundaryLoop> loops = gx3::extractBoundaryLoops(mesh, halfEdges);

bool closed = gx3::isClosed(topology);
double volume = gx3::signedVolume(mesh);
```

### Triangle-mesh spatial queries

`TriangleBVH` accelerates the same nearest-point query implemented by
`closestPointOnMesh`. The BVH is non-owning: keep the mesh alive and rebuild
the index after changing coordinates of indexed vertices or adding/removing
triangles.

```cpp
#include "geox/dim3/mesh/triangle_mesh.h"
#include "geox/dim3/spatial/aabb.h"
#include "geox/dim3/spatial/bounds.h"
#include "geox/dim3/spatial/triangle_bvh.h"

#include <optional>
#include <vector>

namespace gx3 = geox::dim3;

gx3::TriangleMesh mesh{
    {
        gx3::Point{0.0, 0.0, 0.0}, gx3::Point{1.0, 0.0, 0.0},
        gx3::Point{1.0, 1.0, 0.0}, gx3::Point{0.0, 1.0, 0.0},
        gx3::Point{0.0, 0.0, 1.0}, gx3::Point{1.0, 0.0, 1.0},
        gx3::Point{1.0, 1.0, 1.0}, gx3::Point{0.0, 1.0, 1.0},
    },
    {
        gx3::TriangleIndices{0, 2, 1}, gx3::TriangleIndices{0, 3, 2},
        gx3::TriangleIndices{4, 5, 6}, gx3::TriangleIndices{4, 6, 7},
        gx3::TriangleIndices{0, 1, 5}, gx3::TriangleIndices{0, 5, 4},
        gx3::TriangleIndices{1, 2, 6}, gx3::TriangleIndices{1, 6, 5},
        gx3::TriangleIndices{2, 3, 7}, gx3::TriangleIndices{2, 7, 6},
        gx3::TriangleIndices{3, 0, 4}, gx3::TriangleIndices{3, 4, 7},
    },
};

gx3::TriangleBVH bvh = gx3::buildTriangleBVH(mesh);
const std::optional<gx3::ClosestPointOnMeshResult> closest =
    bvh.closestPoint(gx3::Point{1.7, 0.3, 0.6});

const std::optional<gx3::AABB> meshBounds = gx3::boundingBox(mesh);
const std::vector<gx3::TriangleId> candidates = bvh.overlapCandidates(
    gx3::AABB{gx3::Point{0.95, 0.2, 0.2}, gx3::Point{1.05, 0.8, 0.8}}
);
```

`closest` contains the closest point, its squared distance, and the source
triangle ID. `candidates` are conservative: their bounding boxes overlap the
query box, while the triangles themselves may not.

### Kepler–Poinsot polyhedron: great icosahedron

The great icosahedron is a non-convex regular Kepler–Poinsot polyhedron. Its mesh has 12 vertices, 30 edges, and 20 triangular faces; five faces meet at every vertex.

The [great icosahedron demo](examples/vtk/great_icosahedron_vtk_demo.cpp) builds this closed manifold mesh, validates its topology, and exports it to VTK.

![Great icosahedron](assets/great_icosahedron.png)

After building the project, generate the VTK file by running the demo:

```sh
./build/examples/great_icosahedron_vtk_demo build/great_icosahedron.vtk
```

This creates `build/great_icosahedron.vtk`, which can be opened, for example, in *ParaView*.
