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
 * 2014-03-23
 *  - Included Vec3 and Mat4 headers.
 *
 * 2013-11-15 13:57
 *  - Initial version.
 */

#ifndef __CC_MATH_QUATERNION__
#define	__CC_MATH_QUATERNION__

#include "Vec3.hpp"
#include "Mat4.hpp"

namespace cc {
  namespace math {
    template<typename T>
    class Quaternion {
    public:
      inline Quaternion();
      inline Quaternion( const T& xx, const T& yy, const T& zz );
      inline Quaternion( const T& xx, const T& yy, const T& zz, const T& ww );
      inline Quaternion( const Vec3<T>& vec );
      inline Quaternion( const Vec3<T>& vec, const T& ww );

      // Get the scalar component.
      inline T scalar() const;
      // Get the vector component.
      inline Vec3<T> vector() const;

      // Get the length.
      inline T length() const;
      // Get the conjugate.
      inline Quaternion<T> conjugate() const;
      // Get the angle this quaternion represents.
      inline T angle() const;
      // Get the axis of rotation this quaternion represents.
      inline Vec3<T> axis() const;
      // Rotate a quaternion by this quaternion.
      inline Quaternion<T> rotate( const Quaternion<T>& rhs ) const;
      // Rotate a vector around this quaternion.
      inline Vec3<T> rotate( const Vec3<T>& vec ) const;
      // Normalize this quaternion.
      inline void normalize();
      // Get a normalized version of this quaternion.
      inline Quaternion<T> normalized() const;
      // Add a scaled vector.
      inline void addScaledVector( const Vec3<T>& vec, const float scale );
      // Rotate a vector by this quaternion.
      inline Vec3<T> rotateVector( const Vec3<T>& vec ) const;
      // Return the inverse of this quaternion.
      inline Quaternion<T> inverse() const;
      // Dot product with another quaternion.
      inline T dot( const Quaternion<T>& rhs ) const;

      // Create a quaternion from an angle and an axis.
      static Quaternion<T> angleAxis( const Vec3<T>& axis, const T& angle );
      // Create a quaternion from Euler angles.
      static Quaternion<T> createFromEulerAngles( const T& x, const T& y, const T& z );
      // Extract Euler angles from a quaternion.
      static Vec3<T> createEulerAngles( const Quaternion<T>& q );
      // Create a quaternion from a matrix.
      static Quaternion<T> createFromMatrix( const Mat4<T>& m );
      // Create a matrix from a quaternion.
      static Mat4<T> createMatrixFromQuaternion( const Quaternion<T>& q );


    public:
      T x;
      T y;
      T z;
      T w;
    };
  } /* math */
  
  // Typedefs.
  typedef cc::math::Quaternion<float>        Quatf;
  typedef cc::math::Quaternion<double>       Quatd;
  typedef cc::math::Quaternion<int>          Quati;
  typedef cc::math::Quaternion<unsigned int> Quatui;
  
} /* cc */

#include "Quaternion.inl"

#endif	/* __CC_MATH_QUATERNION__ */

