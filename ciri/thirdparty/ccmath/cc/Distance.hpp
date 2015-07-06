/*
  ccmath is a collection of helpful mathematical functions and classes.
  It was developed for use in small, personal projects and should not be
  used as a replacement for more complex mathematical libraries.
  
  The MIT License (MIT)

  Copyright (c) 2014 Daniel Green

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  Don't claim it as your own.  Give credit where credit is due.

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

/**
 * @author  Daniel Green
 *
 * 2013-11-15 13:57
 *  - Initial version.
 */

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

