#include "geox/topology/orientation.h"

#include "geox/primitives/triangle_mesh.h"
#include "geox/topology/triangle_topology.h"

#include <algorithm>
#include <stdexcept>

namespace
{

    void validateMeshAndTopology(
        const geox::TriangleMesh& mesh,
        const geox::TriangleTopology& topology
    )
    {
        if (!mesh.isValid())
            throw std::invalid_argument("Cannot analyze an invalid triangle mesh.");

        if (mesh.vertexCount() != topology.vertexCount() ||
            mesh.triangleCount() != topology.triangleCount())
        {
            throw std::invalid_argument("Triangle topology does not match the mesh.");
        }
    }

    std::size_t findLocalEdge(
        const geox::TriangleTopology::TriangleEdges& triangleEdges,
        geox::EdgeId edgeId
    )
    {
        const auto iterator = std::find(
            triangleEdges.begin(),
            triangleEdges.end(),
            edgeId
        );

        if (iterator == triangleEdges.end())
            throw std::logic_error("Triangle does not contain its incident edge.");

        return static_cast<std::size_t>(iterator - triangleEdges.begin());
    }

    bool hasTriangle(
        const geox::EdgeIncidence& incidence,
        geox::TriangleId triangleId
    )
    {
        return std::find(
            incidence.triangles.begin(),
            incidence.triangles.end(),
            triangleId
        ) != incidence.triangles.end();
    }

} // namespace

namespace geox
{

    bool areAdjacentTrianglesConsistentlyOriented(
        const TriangleMesh& mesh,
        TriangleId firstTriangle,
        TriangleId secondTriangle,
        EdgeId commonEdge,
        const TriangleTopology& topology
    )
    {
        validateMeshAndTopology(mesh, topology);

        if (firstTriangle == secondTriangle)
            throw std::invalid_argument("Adjacent triangles must be distinct.");

        const EdgeIncidence& incidence = topology.edgeIncidence(commonEdge);

        if (!incidence.isInteriorManifold() ||
            !hasTriangle(incidence, firstTriangle) ||
            !hasTriangle(incidence, secondTriangle))
        {
            throw std::invalid_argument("Triangles do not share an interior manifold edge.");
        }

        const std::size_t firstLocalEdge = findLocalEdge(
            topology.triangleEdges(firstTriangle),
            commonEdge
        );

        const std::size_t secondLocalEdge = findLocalEdge(
            topology.triangleEdges(secondTriangle),
            commonEdge
        );

        const TriangleIndices& firstIndices = mesh.triangle(firstTriangle);
        const TriangleIndices& secondIndices = mesh.triangle(secondTriangle);

        const VertexId firstOrigin = firstIndices[firstLocalEdge];
        const VertexId firstDestination = firstIndices[(firstLocalEdge + 1) % 3];

        const VertexId secondOrigin = secondIndices[secondLocalEdge];
        const VertexId secondDestination = secondIndices[(secondLocalEdge + 1) % 3];

        return firstOrigin == secondDestination &&
               firstDestination == secondOrigin;
    }

    bool isConsistentlyOriented(
        const TriangleMesh& mesh,
        const TriangleTopology& topology
    )
    {
        return analyzeOrientation(mesh, topology).consistentlyOriented;
    }

    OrientationAnalysis analyzeOrientation(
        const TriangleMesh& mesh,
        const TriangleTopology& topology
    )
    {
        validateMeshAndTopology(mesh, topology);

        OrientationAnalysis result;
        result.flipTriangles.resize(mesh.triangleCount(), 0);

        std::vector<std::uint8_t> visited(mesh.triangleCount(), 0);
        std::vector<TriangleId> pending;

        if (!topology.isManifold())
        {
            result.orientable = false;
            result.consistentlyOriented = false;
        }

        for (TriangleId firstTriangle = 0;
             firstTriangle < mesh.triangleCount();
             ++firstTriangle)
        {
            if (visited[firstTriangle])
                continue;

            ++result.connectedComponents;
            visited[firstTriangle] = 1;
            pending.push_back(firstTriangle);

            while (!pending.empty())
            {
                const TriangleId triangleId = pending.back();
                pending.pop_back();

                for (EdgeId edgeId : topology.triangleEdges(triangleId))
                {
                    const EdgeIncidence& incidence = topology.edgeIncidence(edgeId);

                    if (incidence.isNonManifold())
                    {
                        result.orientable = false;
                        result.consistentlyOriented = false;
                    }

                    if (!incidence.isInteriorManifold())
                    {
                        for (TriangleId neighbor : incidence.triangles)
                        {
                            if (neighbor != triangleId && !visited[neighbor])
                            {
                                visited[neighbor] = 1;
                                pending.push_back(neighbor);
                            }
                        }

                        continue;
                    }

                    const TriangleId neighbor = incidence.triangles[0] == triangleId
                        ? incidence.triangles[1]
                        : incidence.triangles[0];

                    const bool consistentlyOriented =
                        areAdjacentTrianglesConsistentlyOriented(
                            mesh,
                            triangleId,
                            neighbor,
                            edgeId,
                            topology
                        );

                    if (!consistentlyOriented)
                        result.consistentlyOriented = false;

                    const std::uint8_t neighborFlip =
                        result.flipTriangles[triangleId] ^
                        static_cast<std::uint8_t>(!consistentlyOriented);

                    if (!visited[neighbor])
                    {
                        result.flipTriangles[neighbor] = neighborFlip;
                        visited[neighbor] = 1;
                        pending.push_back(neighbor);
                    }
                    else if (result.flipTriangles[neighbor] != neighborFlip)
                    {
                        result.orientable = false;
                    }
                }
            }
        }

        return result;
    }

    void applyOrientation(
        TriangleMesh& mesh,
        const OrientationAnalysis& analysis
    )
    {
        if (!mesh.isValid())
            throw std::invalid_argument("Cannot orient an invalid triangle mesh.");

        if (!analysis.orientable)
            throw std::invalid_argument("Cannot orient a non-orientable mesh.");

        if (analysis.flipTriangles.size() != mesh.triangleCount())
            throw std::invalid_argument("Orientation analysis does not match the mesh.");

        for (TriangleId triangleId = 0;
             triangleId < mesh.triangleCount();
             ++triangleId)
        {
            const std::uint8_t flip = analysis.flipTriangles[triangleId];

            if (flip > 1)
                throw std::invalid_argument("Triangle orientation flip must be zero or one.");

            if (flip)
                mesh.reverseTriangleOrientation(triangleId);
        }
    }

    void reverseOrientation(TriangleMesh& mesh)
    {
        for (TriangleId triangleId = 0;
             triangleId < mesh.triangleCount();
             ++triangleId)
        {
            mesh.reverseTriangleOrientation(triangleId);
        }
    }

    bool isClosed(const TriangleTopology& topology)
    {
        if (!topology.isManifold())
            return false;

        for (EdgeId edgeId = 0; edgeId < topology.edgeCount(); ++edgeId)
            if (topology.isBoundaryEdge(edgeId)) return false;

        return true;
    }

    double signedVolume(const TriangleMesh& mesh)
    {
        if (!mesh.isValid())
            throw std::invalid_argument("Cannot calculate volume of an invalid triangle mesh.");

        double volume = 0.0;

        for (const TriangleIndices& indices : mesh.triangles())
        {
            const Point3& a = mesh.vertex(indices[0]);
            const Point3& b = mesh.vertex(indices[1]);
            const Point3& c = mesh.vertex(indices[2]);

            volume += a.x() * (b.y() * c.z() - b.z() * c.y()) +
                      a.y() * (b.z() * c.x() - b.x() * c.z()) +
                      a.z() * (b.x() * c.y() - b.y() * c.x());
        }

        return volume / 6.0;
    }

} // namespace geox
