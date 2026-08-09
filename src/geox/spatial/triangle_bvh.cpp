#include "geox/spatial/triangle_bvh.h"

#include "geox/algorithms/triangle3.h"
#include "geox/primitives/triangle_mesh.h"

#include <algorithm>
#include <functional>
#include <limits>
#include <stdexcept>

namespace
{
    struct BuildTriangle
    {
        geox::AABB3 bounds;
        geox::Point3 centroid;
    };

    BuildTriangle makeBuildTriangle(
        const geox::TriangleMesh& mesh,
        geox::TriangleId triangleId
    )
    {
        const geox::TriangleIndices& indices = mesh.triangle(triangleId);
        const geox::Point3& a = mesh.vertex(indices[0]);
        const geox::Point3& b = mesh.vertex(indices[1]);
        const geox::Point3& c = mesh.vertex(indices[2]);

        return {
            geox::AABB3{
                geox::Point3{
                    std::min({a.x(), b.x(), c.x()}),
                    std::min({a.y(), b.y(), c.y()}),
                    std::min({a.z(), b.z(), c.z()})
                },
                geox::Point3{
                    std::max({a.x(), b.x(), c.x()}),
                    std::max({a.y(), b.y(), c.y()}),
                    std::max({a.z(), b.z(), c.z()})
                }
            },
            geox::Point3{
                (a.x() + b.x() + c.x()) / 3.0,
                (a.y() + b.y() + c.y()) / 3.0,
                (a.z() + b.z() + c.z()) / 3.0
            }
        };
    }
} // namespace

namespace geox
{

    bool TriangleBVH::empty() const noexcept
    {
        return nodes_.empty();
    }

    std::size_t TriangleBVH::nodeCount() const noexcept
    {
        return nodes_.size();
    }

    std::size_t TriangleBVH::triangleCount() const noexcept
    {
        return triangleIds_.size();
    }

    std::size_t TriangleBVH::maxDepth() const noexcept
    {
        return maxDepth_;
    }

    std::optional<ClosestPointOnMeshResult> TriangleBVH::closestPoint(
        const Point3& point
    ) const
    {
        if (mesh_ == nullptr || nodes_.empty())
            return std::nullopt;

        struct NodeVisit
        {
            NodeId nodeId;
            double lowerBound;
        };

        std::optional<ClosestPointOnMeshResult> result;
        std::vector<NodeVisit> pending;
        pending.push_back({0, nodes_.front().bounds.squaredDistance(point)});

        while (!pending.empty())
        {
            const NodeVisit visit = pending.back();
            pending.pop_back();

            if (result.has_value() && visit.lowerBound > result->squaredDistance)
                continue;

            const Node& node = nodes_.at(visit.nodeId);
            if (node.triangleCount != 0)
            {
                for (std::uint32_t offset = 0; offset < node.triangleCount; ++offset)
                {
                    const TriangleId triangleId = triangleIds_.at(node.firstTriangle + offset);
                    const TriangleIndices& indices = mesh_->triangle(triangleId);
                    const Triangle3 triangle{
                        mesh_->vertex(indices[0]),
                        mesh_->vertex(indices[1]),
                        mesh_->vertex(indices[2])
                    };
                    const Point3 closest = closestPointOnTriangle(point, triangle);
                    const double squaredDistance = point.squaredDistance(closest);

                    if (!result.has_value() ||
                        squaredDistance < result->squaredDistance ||
                        (squaredDistance == result->squaredDistance &&
                         triangleId < result->triangleId))
                    {
                        result = ClosestPointOnMeshResult{
                            triangleId,
                            closest,
                            squaredDistance
                        };
                    }
                }

                continue;
            }

            const Node& left = nodes_.at(node.left);
            const Node& right = nodes_.at(node.right);
            const double leftLowerBound = left.bounds.squaredDistance(point);
            const double rightLowerBound = right.bounds.squaredDistance(point);

            NodeVisit nearVisit{node.left, leftLowerBound};
            NodeVisit farVisit{node.right, rightLowerBound};
            if (rightLowerBound < leftLowerBound)
                std::swap(nearVisit, farVisit);

            if (!result.has_value() || farVisit.lowerBound <= result->squaredDistance)
                pending.push_back(farVisit);
            if (!result.has_value() || nearVisit.lowerBound <= result->squaredDistance)
                pending.push_back(nearVisit);
        }

        return result;
    }

    std::vector<TriangleId> TriangleBVH::overlapCandidates(
        const AABB3& queryBox
    ) const
    {
        std::vector<TriangleId> candidates;
        if (nodes_.empty())
            return candidates;

        std::vector<NodeId> pending{0};
        while (!pending.empty())
        {
            const NodeId nodeId = pending.back();
            pending.pop_back();

            const Node& node = nodes_.at(nodeId);
            if (!node.bounds.overlaps(queryBox))
                continue;

            if (node.triangleCount != 0)
            {
                for (std::uint32_t offset = 0; offset < node.triangleCount; ++offset)
                {
                    const TriangleId triangleId = triangleIds_.at(node.firstTriangle + offset);
                    if (makeBuildTriangle(*mesh_, triangleId).bounds.overlaps(queryBox))
                        candidates.push_back(triangleId);
                }
                continue;
            }

            pending.push_back(node.right);
            pending.push_back(node.left);
        }

        return candidates;
    }

    TriangleBVH buildTriangleBVH(
        const TriangleMesh& mesh,
        const TriangleBVHBuildOptions& options
    )
    {
        if (options.maxLeafSize == 0)
            throw std::invalid_argument("Triangle BVH leaf size must be positive.");
        if (!mesh.isValid())
            throw std::invalid_argument("Cannot build a triangle BVH for an invalid triangle mesh.");
        if (mesh.triangleCount() >= std::numeric_limits<TriangleId>::max())
            throw std::overflow_error("Triangle mesh exceeds the supported triangle ID range.");

        TriangleBVH result;
        result.mesh_ = &mesh;

        if (mesh.triangleCount() == 0)
            return result;

        std::vector<BuildTriangle> buildTriangles;
        buildTriangles.reserve(mesh.triangleCount());
        result.triangleIds_.reserve(mesh.triangleCount());

        for (TriangleId triangleId = 0; triangleId < mesh.triangleCount(); ++triangleId)
        {
            buildTriangles.push_back(makeBuildTriangle(mesh, triangleId));
            result.triangleIds_.push_back(triangleId);
        }

        const std::size_t maximumNodeId = std::numeric_limits<TriangleBVH::NodeId>::max();
        std::function<TriangleBVH::NodeId(std::uint32_t, std::uint32_t, std::size_t)> buildNode;
        buildNode = [&result, &buildTriangles, &buildNode, maximumNodeId, &options](
            std::uint32_t firstTriangle,
            std::uint32_t triangleCount,
            std::size_t depth
        ) -> TriangleBVH::NodeId
        {
            if (result.nodes_.size() >= maximumNodeId)
                throw std::overflow_error("Triangle BVH exceeds the supported node ID range.");

            AABB3 triangleBounds = buildTriangles.at(
                result.triangleIds_.at(firstTriangle)
            ).bounds;
            AABB3 centroidBounds = AABB3{
                buildTriangles.at(result.triangleIds_.at(firstTriangle)).centroid,
                buildTriangles.at(result.triangleIds_.at(firstTriangle)).centroid
            };

            for (std::uint32_t offset = 1; offset < triangleCount; ++offset)
            {
                const BuildTriangle& triangle = buildTriangles.at(
                    result.triangleIds_.at(firstTriangle + offset)
                );
                triangleBounds = merge(triangleBounds, triangle.bounds);
                centroidBounds = merge(
                    centroidBounds,
                    AABB3{triangle.centroid, triangle.centroid}
                );
            }

            const TriangleBVH::NodeId nodeId =
                static_cast<TriangleBVH::NodeId>(result.nodes_.size());
            result.nodes_.push_back(TriangleBVH::Node{
                triangleBounds,
                0,
                0,
                firstTriangle,
                triangleCount
            });
            result.maxDepth_ = std::max(result.maxDepth_, depth);

            if (triangleCount <= options.maxLeafSize)
                return nodeId;

            const std::size_t axis = centroidBounds.longestAxis();
            const std::uint32_t middle = firstTriangle + triangleCount / 2;
            std::nth_element(
                result.triangleIds_.begin() + firstTriangle,
                result.triangleIds_.begin() + middle,
                result.triangleIds_.begin() + firstTriangle + triangleCount,
                [&buildTriangles, axis](TriangleId first, TriangleId second)
                {
                    const double firstCoordinate = buildTriangles.at(first).centroid[
                        static_cast<int>(axis)
                    ];
                    const double secondCoordinate = buildTriangles.at(second).centroid[
                        static_cast<int>(axis)
                    ];

                    return firstCoordinate == secondCoordinate
                        ? first < second
                        : firstCoordinate < secondCoordinate;
                }
            );

            const std::uint32_t leftCount = middle - firstTriangle;
            const std::uint32_t rightCount = triangleCount - leftCount;
            const TriangleBVH::NodeId left = buildNode(firstTriangle, leftCount, depth + 1);
            const TriangleBVH::NodeId right = buildNode(middle, rightCount, depth + 1);

            TriangleBVH::Node& node = result.nodes_.at(nodeId);
            node.left = left;
            node.right = right;
            node.firstTriangle = 0;
            node.triangleCount = 0;
            return nodeId;
        };

        buildNode(0, static_cast<std::uint32_t>(result.triangleIds_.size()), 0);
        return result;
    }

} // namespace geox
