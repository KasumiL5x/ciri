#ifndef __CC_MATH_DISTANCE__
#define	__CC_MATH_DISTANCE__

#include "Vec3.hpp"

namespace cc {
  namespace math {
    /**
     * Compute the perpendicular distance from a point to a plane.
     * @param[in] point       Point to test from.
     * @param[in] planePoint  Point on the plane.
     * @param[in] planeNormal Normal of the plane.
     * @return perpendicular distance from the point to the plane.
     */
    template<typename T>
    inline float perpendicularDistanceToPointFromPlane( const Vec3<T>& point, const Vec3<T>& planePoint, const Vec3<T>& planeNormal ) {
      return point.dot(planeNormal) - planePoint.dot(planeNormal);
    }
  } /* math */
} /* cc */

#endif	/* __CC_MATH_DISTANCE__ */

