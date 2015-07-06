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

