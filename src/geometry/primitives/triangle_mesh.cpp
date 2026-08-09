#include "geometry/primitives/triangle_mesh.h"

#include <limits>
#include <stdexcept>

namespace dsa::geometry
{

    TriangleMesh::TriangleMesh(
        const std::vector<Point3>& vertices,
        const std::vector<TriangleIndices>& triangles
    ) : vertices_(vertices), triangles_(triangles) {}

    const std::vector<Point3>& TriangleMesh::vertices() const
    {
        return vertices_;
    }

    const std::vector<TriangleIndices>& TriangleMesh::triangles() const
    {
        return triangles_;
    }

    const Point3& TriangleMesh::vertex(VertexId index) const
    {
        return vertices_.at(index);
    }

    const TriangleIndices& TriangleMesh::triangle(TriangleId index) const
    {
        return triangles_.at(index);
    }

    VertexId TriangleMesh::addVertex(const Point3& vertex)
    {
        if (vertices_.size() > std::numeric_limits<VertexId>::max())
            throw std::overflow_error("The mesh cannot contain more vertices.");

        vertices_.push_back(vertex);
        return static_cast<VertexId>(vertices_.size() - 1);
    }

    void TriangleMesh::addTriangle(const TriangleIndices& indices)
    {
        triangles_.push_back(indices);
    }

    std::size_t TriangleMesh::vertexCount() const noexcept { return vertices_.size(); }

    std::size_t TriangleMesh::triangleCount() const noexcept { return triangles_.size(); }

    bool TriangleMesh::isValid() const
    {
        return hasValidIndices() &&
               hasDistinctTriangleVertices();
    }

    bool TriangleMesh::isGeometricallyValid() const
    {
        return isValid() &&
               !hasDegenerateTriangles();
    }

    double TriangleMesh::triangleArea(TriangleId index) const
    {
        const TriangleIndices& indices = triangles_.at(index);
        return dsa::geometry::triangleArea(
            vertices_.at(indices[0]),
            vertices_.at(indices[1]),
            vertices_.at(indices[2])
        );
    }

    vector3d TriangleMesh::triangleNormal(TriangleId index) const
    {
        const TriangleIndices& indices = triangles_.at(index);
        return Triangle3(
            vertices_.at(indices[0]),
            vertices_.at(indices[1]),
            vertices_.at(indices[2])
        ).normal();
    }

    bool TriangleMesh::hasValidIndices() const
    {
        for (const TriangleIndices& indices : triangles_)
        {
            if (indices[0] >= vertices_.size() ||
                indices[1] >= vertices_.size() ||
                indices[2] >= vertices_.size())
            {
                return false;
            }
        }
        return true;
    }

    bool TriangleMesh::hasDistinctTriangleVertices() const
    {
        for (const TriangleIndices& indices : triangles_)
        {
            if (indices[0] == indices[1] ||
                indices[0] == indices[2] ||
                indices[1] == indices[2])
            {
                return false;
            }
        }
        return true;
    }

    bool TriangleMesh::hasDegenerateTriangles() const
    {
        for (const TriangleIndices& indices : triangles_)
        {
            if (Triangle3(
                vertices_[indices[0]],
                vertices_[indices[1]],
                vertices_[indices[2]]
            ).isDegenerate())
            {
                return true;
            }
        }
        return false;
    }

} // namespace dsa::geometry
