# Data Structures, Algorithms and Geometry

This repository contains simple C++ implementations of common data structures, algorithms, and geometry components. It is used for studying how they work and how to implement them.

## Implemented data structures

- [Array](include/data_structures/array.h)
- [Vector](include/data_structures/vector.h)
- [Singly linked list](include/data_structures/singly_linked_list.h)
- [Doubly linked list](include/data_structures/doubly_linked_list.h)
- [Queue](include/data_structures/queue.h)
- [Stack](include/data_structures/stack.h)

The shared [list node](include/data_structures/ListNode.h) implementation is now used by the linked lists and the queue.

## Geometry module

The `dsa::geometry` namespace contains the repository's geometry functionality. It currently includes the following 2D and 3D types and utilities:

- [Vectors](include/geometry/primitives/vectors.h): `vector2d` and `vector3d`, with arithmetic, dot and cross products, norms, and normalization
- [Points](include/geometry/primitives/points.h): `Point2` and `Point3`, with vector translation and distance calculations
- [Segments](include/geometry/primitives/segments.h): `Segment2` and `Segment3`, with direction, length, and interpolation through `pointAt(t)`
- [Triangles](include/geometry/primitives/triangles.h): `Triangle2` and `Triangle3`, with area, edges, degeneracy checks, and 3D normals
- [Triangle meshes](include/geometry/primitives/triangle_mesh.h): `TriangleMesh`, vertex/triangle counts, index validation, geometric validation, per-triangle area, and normals

All geometry types use `double` coordinates. Segment interpolation accepts `t` in the inclusive range `[0, 1]`.

## Build and use

The project builds a C++17 library named `dsa-studying`:

```sh
cmake -S . -B build
cmake --build build
```

Include headers from `include/` and use the `dsa::geometry` namespace:

```cpp
#include "geometry/primitives/triangles.h"

using namespace dsa::geometry;

Triangle3 triangle{
    Point3{0.0, 0.0, 0.0},
    Point3{1.0, 0.0, 0.0},
    Point3{0.0, 1.0, 0.0},
};

double area = triangle.area();
vector3d normal = triangle.normal();
```

The collection is gradually expanding.
