#pragma once

#include <cstdint>
#include <cstddef>
#include <array>
#include <vector>

#include "geometry/primitives/triangles.h"

namespace dsa::geometry
{

    using VertexId = std::uint32_t;
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

        const Point3& vertex(std::size_t index) const;
        const TriangleIndices& triangle(std::size_t index) const;

        VertexId addVertex(const Point3& vertex);
        void addTriangle(const TriangleIndices& indices);

        std::size_t vertexCount() const noexcept;
        std::size_t triangleCount() const noexcept;

        bool isValid() const;
        bool isGeometricallyValid() const;

        double triangleArea(std::size_t index) const;
        vector3d triangleNormal(std::size_t index) const;

    private:
        bool hasValidIndices() const;
        bool hasDistinctTriangleVertices() const;
        bool hasDegenerateTriangles() const;

        std::vector<Point3> vertices_;
        std::vector<TriangleIndices> triangles_;
    };

} // namespace dsa::geometry
