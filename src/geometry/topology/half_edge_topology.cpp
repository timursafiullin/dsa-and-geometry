#include "geometry/topology/half_edge_topology.h"

#include "geometry/primitives/triangle_mesh.h"
#include "geometry/topology/triangle_topology.h"

#include <stdexcept>

namespace
{

    std::size_t findLocalEdge(
        const dsa::geometry::TriangleTopology::TriangleEdges& triangleEdges,
        dsa::geometry::EdgeId edgeId
    )
    {
        for (std::size_t localEdge = 0; localEdge < triangleEdges.size(); ++localEdge)
            if (triangleEdges[localEdge] == edgeId)
                return localEdge;

        throw std::logic_error("Triangle does not contain its incident edge.");
    }

} // namespace

namespace dsa::geometry
{

    // MARK: HalfEdgeTopology
    std::size_t HalfEdgeTopology::halfEdgeCount() const noexcept
    {
        return twins_.size();
    }

    TriangleId HalfEdgeTopology::triangle(HalfEdgeId halfEdge) const
    {
        if (halfEdge >= halfEdgeCount())
            throw std::out_of_range("Half-edge index is out of range.");

        return halfEdge / 3;
    }

    std::size_t HalfEdgeTopology::localEdge(HalfEdgeId halfEdge) const
    {
        if (halfEdge >= halfEdgeCount())
            throw std::out_of_range("Half-edge index is out of range.");

        return halfEdge % 3;
    }

    EdgeId HalfEdgeTopology::edge(HalfEdgeId halfEdge) const
    {
        return edges_.at(halfEdge);
    }

    HalfEdgeId HalfEdgeTopology::next(HalfEdgeId halfEdge) const
    {
        const TriangleId triangleId = triangle(halfEdge);
        const std::size_t nextLocalEdge = (localEdge(halfEdge) + 1) % 3;

        return 3 * triangleId + nextLocalEdge;
    }

    HalfEdgeId HalfEdgeTopology::previous(HalfEdgeId halfEdge) const
    {
        const TriangleId triangleId = triangle(halfEdge);
        const std::size_t previousLocalEdge = (localEdge(halfEdge) + 2) % 3;

        return 3 * triangleId + previousLocalEdge;
    }

    HalfEdgeId HalfEdgeTopology::twin(HalfEdgeId halfEdge) const
    {
        return twins_.at(halfEdge);
    }

    bool HalfEdgeTopology::hasTwin(HalfEdgeId halfEdge) const
    {
        return twin(halfEdge) != InvalidHalfEdgeId;
    }

    bool HalfEdgeTopology::isBoundary(HalfEdgeId halfEdge) const
    {
        return !hasTwin(halfEdge);
    }

    HalfEdgeId HalfEdgeTopology::nextBoundary(
        HalfEdgeId boundaryHalfEdge
    ) const
    {
        if (!isBoundary(boundaryHalfEdge))
            throw std::invalid_argument("Expected a boundary half-edge.");

        HalfEdgeId candidate = next(boundaryHalfEdge);

        while (hasTwin(candidate))
            candidate = next(twin(candidate));

        return candidate;
    }

    VertexId HalfEdgeTopology::origin(
        const TriangleMesh& mesh,
        HalfEdgeId halfEdge
    ) const
    {
        return mesh.triangle(triangle(halfEdge)).at(localEdge(halfEdge));
    }

    VertexId HalfEdgeTopology::destination(
        const TriangleMesh& mesh,
        HalfEdgeId halfEdge
    ) const
    {
        const std::size_t destinationLocalEdge = (localEdge(halfEdge) + 1) % 3;

        return mesh.triangle(triangle(halfEdge)).at(destinationLocalEdge);
    }

    // MARK: BuildHalfEdgeTopology
    HalfEdgeTopology buildHalfEdgeTopology(
        const TriangleMesh& mesh,
        const TriangleTopology& topology
    )
    {
        if (!mesh.isValid())
            throw std::invalid_argument("Cannot build half-edge topology for an invalid mesh.");

        if (mesh.triangleCount() != topology.triangleCount())
            throw std::invalid_argument("Mesh and topology triangle counts do not match.");

        if (mesh.vertexCount() != topology.vertexCount())
            throw std::invalid_argument("Mesh and topology vertex counts do not match.");

        if (!topology.isManifold())
            throw std::invalid_argument("Cannot build half-edge topology for a non-manifold mesh.");

        HalfEdgeTopology result;

        result.edges_.resize(3 * topology.triangleCount());
        result.twins_ = std::vector<HalfEdgeId>(
            3 * topology.triangleCount(),
            InvalidHalfEdgeId
        );

        for (TriangleId triangleId = 0;
             triangleId < topology.triangleCount();
             ++triangleId)
        {
            const auto& triangleEdges = topology.triangleEdges(triangleId);

            for (std::size_t localEdge = 0; localEdge < triangleEdges.size(); ++localEdge)
            {
                const HalfEdgeId halfEdge = 3 * triangleId + localEdge;
                result.edges_[halfEdge] = triangleEdges[localEdge];
            }
        }

        for (EdgeId edgeId = 0; edgeId < topology.edgeCount(); ++edgeId)
        {
            const EdgeIncidence& incidence = topology.edgeIncidence(edgeId);

            if (incidence.isBoundary())
                continue;

            if (!incidence.isInteriorManifold())
                throw std::logic_error("Expected a manifold edge incidence.");

            const TriangleId firstTriangle = incidence.triangles.at(0);
            const TriangleId secondTriangle = incidence.triangles.at(1);

            const std::size_t firstLocalEdge = findLocalEdge(
                topology.triangleEdges(firstTriangle),
                edgeId
            );

            const std::size_t secondLocalEdge = findLocalEdge(
                topology.triangleEdges(secondTriangle),
                edgeId
            );

            const HalfEdgeId firstHalfEdge = 3 * firstTriangle + firstLocalEdge;
            const HalfEdgeId secondHalfEdge = 3 * secondTriangle + secondLocalEdge;

            const TriangleIndices& firstIndices = mesh.triangle(firstTriangle);
            const TriangleIndices& secondIndices = mesh.triangle(secondTriangle);

            const VertexId firstOrigin = firstIndices.at(firstLocalEdge);
            const VertexId firstDestination = firstIndices.at((firstLocalEdge + 1) % 3);

            const VertexId secondOrigin = secondIndices.at(secondLocalEdge);
            const VertexId secondDestination = secondIndices.at((secondLocalEdge + 1) % 3);

            if (firstOrigin != secondDestination ||
                firstDestination != secondOrigin)
            {
                throw std::invalid_argument(
                    "Cannot build half-edge topology for inconsistently oriented triangles."
                );
            }

            result.twins_[firstHalfEdge] = secondHalfEdge;
            result.twins_[secondHalfEdge] = firstHalfEdge;
        }

        return result;
    }

} // namespace dsa::geometry
