#include "geox/dim3/mesh/triangle_mesh.h"

#include <limits>
#include <stdexcept>
#include <utility>

namespace geox::dim3
{
    TriangleMesh::TriangleMesh(
        const std::vector<Point>& vertices,
        const std::vector<::geox::dim3::TriangleIndices>& triangles
    ) : vertices_(vertices), triangles_(triangles) {}

    const std::vector<Point>& TriangleMesh::vertices() const { return vertices_; }
    const std::vector<::geox::dim3::TriangleIndices>& TriangleMesh::triangles() const { return triangles_; }
    const Point& TriangleMesh::vertex(VertexId index) const { return vertices_.at(index); }
    const ::geox::dim3::TriangleIndices& TriangleMesh::triangle(TriangleId index) const { return triangles_.at(index); }

    VertexId TriangleMesh::addVertex(const Point& vertex)
    {
        if (vertices_.size() > std::numeric_limits<VertexId>::max())
            throw std::overflow_error("The mesh cannot contain more vertices.");

        vertices_.push_back(vertex);
        return static_cast<VertexId>(vertices_.size() - 1);
    }

    TriangleId TriangleMesh::addTriangle(const ::geox::dim3::TriangleIndices& indices)
    {
        if (triangles_.size() > std::numeric_limits<TriangleId>::max())
            throw std::overflow_error("The mesh cannot contain more triangles.");

        triangles_.push_back(indices);
        return static_cast<TriangleId>(triangles_.size() - 1);
    }

    void TriangleMesh::reverseTriangleOrientation(TriangleId triangleId)
    {
        ::geox::dim3::TriangleIndices& indices = triangles_.at(triangleId);
        std::swap(indices[1], indices[2]);
    }

    std::size_t TriangleMesh::vertexCount() const noexcept { return vertices_.size(); }
    std::size_t TriangleMesh::triangleCount() const noexcept { return triangles_.size(); }
    bool TriangleMesh::isValid() const { return hasValidIndices() && hasDistinctTriangleVertices(); }
    bool TriangleMesh::isGeometricallyValid() const { return isValid() && !hasDegenerateTriangles(); }

    double TriangleMesh::triangleArea(TriangleId index) const
    {
        const ::geox::dim3::TriangleIndices& indices = triangles_.at(index);
        return ::geox::dim3::triangleArea(vertices_.at(indices[0]), vertices_.at(indices[1]), vertices_.at(indices[2]));
    }

    Vector TriangleMesh::triangleNormal(TriangleId index) const
    {
        const ::geox::dim3::TriangleIndices& indices = triangles_.at(index);
        return Triangle(vertices_.at(indices[0]), vertices_.at(indices[1]), vertices_.at(indices[2])).normal();
    }

    bool TriangleMesh::hasValidIndices() const
    {
        for (const ::geox::dim3::TriangleIndices& indices : triangles_)
            if (indices[0] >= vertices_.size() || indices[1] >= vertices_.size() || indices[2] >= vertices_.size()) return false;
        return true;
    }

    bool TriangleMesh::hasDistinctTriangleVertices() const
    {
        for (const ::geox::dim3::TriangleIndices& indices : triangles_)
            if (indices[0] == indices[1] || indices[0] == indices[2] || indices[1] == indices[2]) return false;
        return true;
    }

    bool TriangleMesh::hasDegenerateTriangles() const
    {
        for (const ::geox::dim3::TriangleIndices& indices : triangles_)
            if (Triangle(vertices_[indices[0]], vertices_[indices[1]], vertices_[indices[2]]).isDegenerate()) return true;
        return false;
    }
} // namespace geox::dim3
