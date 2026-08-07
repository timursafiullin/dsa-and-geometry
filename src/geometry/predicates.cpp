#include <geometry/predicates.h>

namespace dsa::geometry
{

    double orient2d(const Point2& a, const Point2& b, const Point2& c)
    {
        return (b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x());
    }

} // namespace dsa::geometry