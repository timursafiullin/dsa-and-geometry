#pragma once

#include "geox/algorithms/triangle_mesh.h"
#include "geox/mesh_ids.h"
#include "geox/spatial/aabb.h"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace geox
{

    class TriangleMesh;

    struct TriangleBVHBuildOptions
    {
        // Must be positive.
        std::size_t maxLeafSize = 4;
    };

    class TriangleBVH
    {
    public:
        TriangleBVH() = default;

        bool empty() const noexcept;

        std::size_t nodeCount() const noexcept;
        std::size_t triangleCount() const noexcept;
        std::size_t maxDepth() const noexcept;

        std::optional<ClosestPointOnMeshResult> closestPoint(
            const Point3& point
        ) const;

        // Returns triangles whose bounding boxes overlap queryBox.
        // These are candidates; triangle geometry itself may not overlap queryBox.
        std::vector<TriangleId> overlapCandidates(
            const AABB3& queryBox
        ) const;

    private:
        using NodeId = std::uint32_t;

        struct Node
        {
            AABB3 bounds;

            NodeId left;
            NodeId right;

            std::uint32_t firstTriangle;
            std::uint32_t triangleCount;
        };

        friend TriangleBVH buildTriangleBVH(
            const TriangleMesh& mesh,
            const TriangleBVHBuildOptions& options
        );

        // Non-owning. The referenced mesh must outlive this index and must not
        // be modified after construction.
        const TriangleMesh* mesh_ = nullptr;

        std::vector<Node> nodes_;
        std::vector<TriangleId> triangleIds_;

        std::size_t maxDepth_ = 0;
    };

    TriangleBVH buildTriangleBVH(
        const TriangleMesh& mesh,
        const TriangleBVHBuildOptions& options = {}
    );

} // namespace geox
