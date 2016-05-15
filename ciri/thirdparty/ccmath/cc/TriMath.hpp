#ifndef __CC_MATH_TRIMATH__
#define	__CC_MATH_TRIMATH__

#include "Vec3.hpp"

namespace cc {
  namespace math {
    /**
     * Computes the non-normalized normal of a triangle.
     * @param[in] p0 First vertex of the triangle.
     * @param[in] p1 Second vertex of the triangle.
     * @param[in] p2 Third vertex of the triangle.
     * @return Non-normalized normal of the triangle.
     */
    template<typename T>
    inline Vec3<T> computeTriangleNormal( const Vec3<T>& p0, const Vec3<T>& p1, const Vec3<T>& p2 ) {
      return (p1 - p0).cross(p2 - p0);
    }
    
    /**
     * Computes the area of a triangle.
     * @param[in] p0 First vertex of the triangle.
     * @param[in] p1 Second vertex of the triangle.
     * @param[in] p2 Third vertex of the triangle.
     * @return Area of the triangle.
     */
    template<typename T>
    inline T computeTriangleArea( const Vec3<T>& p0, const Vec3<T>& p1, const Vec3<T>& p2 ) {
      // Won't work w/ ints because of 0.5, but fuck it, who uses integer triangles?
      return computeTriangleNormal<T>(p0, p1, p2).magnitude() * static_cast<T>(0.5);
    }
  }
}

#endif	/* __CC_MATH_TRIMATH__ */

