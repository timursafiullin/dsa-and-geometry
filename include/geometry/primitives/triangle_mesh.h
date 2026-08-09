#pragma once

#include <cstdint>
#include <cstddef>
#include <array>
#include <vector>

#include "geometry/primitives/triangles.h"
#include "geometry/mesh_ids.h"

namespace dsa::geometry
{

    using TriangleIndices = std::array<VertexId, 3>;

    class TriangleMesh
    {
    public:
        TriangleMesh() = default;
        TriangleMesh(
            const std::vector<Point3>& vertices,
            const std::vector<TriangleIndices>& triangles
        );

        const std::vector<Point3>& vertices() const;
        const std::vector<TriangleIndices>& triangles() const;

        const Point3& vertex(VertexId index) const;
        const TriangleIndices& triangle(TriangleId index) const;

        VertexId addVertex(const Point3& vertex);
        TriangleId addTriangle(const TriangleIndices& indices);

        void reverseTriangleOrientation(TriangleId triangleId);

        std::size_t vertexCount() const noexcept;
        std::size_t triangleCount() const noexcept;

        bool isValid() const;
        bool isGeometricallyValid() const;

        double triangleArea(TriangleId index) const;
        vector3d triangleNormal(TriangleId index) const;

    private:
        bool hasValidIndices() const;
        bool hasDistinctTriangleVertices() const;
        bool hasDegenerateTriangles() const;

        std::vector<Point3> vertices_;
        std::vector<TriangleIndices> triangles_;
    };

} // namespace dsa::geometry
