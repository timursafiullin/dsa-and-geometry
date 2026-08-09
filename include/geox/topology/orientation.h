#pragma once

#include "geox/mesh_ids.h"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace geox
{

    class TriangleMesh;
    class TriangleTopology;

    struct OrientationAnalysis
    {
        // A mesh with non-manifold edges is not considered orientable.
        bool orientable = true;
        bool consistentlyOriented = true;

        // Number of triangle components connected through edges.
        std::size_t connectedComponents = 0;

        // A correction plan produced by analyzeOrientation().
        // The value is 1 for triangles whose orientation must be reversed.
        std::vector<std::uint8_t> flipTriangles;
    };

    // Topology passed to orientation checks must be built from mesh in its
    // current orientation.

    // Returns true when the two triangles traverse their common edge
    // in opposite directions.
    bool areAdjacentTrianglesConsistentlyOriented(
        const TriangleMesh& mesh,
        TriangleId firstTriangle,
        TriangleId secondTriangle,
        EdgeId commonEdge,
        const TriangleTopology& topology
    );

    bool isConsistentlyOriented(
        const TriangleMesh& mesh,
        const TriangleTopology& topology
    );

    OrientationAnalysis analyzeOrientation(
        const TriangleMesh& mesh,
        const TriangleTopology& topology
    );

    // Rebuild TriangleTopology after changing a mesh orientation.
    void applyOrientation(
        TriangleMesh& mesh,
        const OrientationAnalysis& analysis
    );

    void reverseOrientation(
        TriangleMesh& mesh
    );

    // A closed mesh is a manifold mesh without boundary edges.
    bool isClosed(
        const TriangleTopology& topology
    );

    // Returns the oriented volume integral. It represents a volume only
    // for a closed, consistently oriented mesh.
    double signedVolume(
        const TriangleMesh& mesh
    );

} // namespace geox
