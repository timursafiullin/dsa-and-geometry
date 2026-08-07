#include "geometry/primitives/triangle_mesh.h"

namespace dsa::geometry
{

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

    double TriangleMesh::triangleArea(std::size_t index) const
    {
        const TriangleIndices& indices = triangles_.at(index);
        return dsa::geometry::triangleArea(
            vertices_.at(indices[0]),
            vertices_.at(indices[1]),
            vertices_.at(indices[2])
        );
    }

    vector3d TriangleMesh::triangleNormal(std::size_t index) const
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
