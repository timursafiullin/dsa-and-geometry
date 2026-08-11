#pragma once

#include "geox/dim3/mesh/ids.h"
#include "geox/dim3/primitives/triangle.h"

#include <array>
#include <cstddef>
#include <vector>

namespace geox::dim3
{
    using TriangleIndices = std::array<VertexId, 3>;

    class TriangleMesh
    {
    public:
        TriangleMesh() = default;
        TriangleMesh(
            const std::vector<Point>& vertices,
            const std::vector<TriangleIndices>& triangles
        );

        const std::vector<Point>& vertices() const;
        const std::vector<TriangleIndices>& triangles() const;
        const Point& vertex(VertexId index) const;
        const TriangleIndices& triangle(TriangleId index) const;

        VertexId addVertex(const Point& vertex);
        TriangleId addTriangle(const TriangleIndices& indices);
        void reverseTriangleOrientation(TriangleId triangleId);

        std::size_t vertexCount() const noexcept;
        std::size_t triangleCount() const noexcept;
        bool isValid() const;
        bool isGeometricallyValid() const;
        double triangleArea(TriangleId index) const;
        Vector triangleNormal(TriangleId index) const;

    private:
        bool hasValidIndices() const;
        bool hasDistinctTriangleVertices() const;
        bool hasDegenerateTriangles() const;

        std::vector<Point> vertices_;
        std::vector<TriangleIndices> triangles_;
    };
} // namespace geox::dim3
