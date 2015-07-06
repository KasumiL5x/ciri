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
 * 2014-03-23 18:08
 *  - Fixed orthographic function.  It also takes two extra parameters, [near] and [far].
 * 
 * 2013-11-15 13:57
 *  - Initial version.
 */

#include "Mat4.hpp"
#include "Vec4.hpp"
#include "Quaternion.hpp"

#ifndef __CC_MATH_MATRIXFUNC__
#define __CC_MATH_MATRIXFUNC__

namespace cc {
  namespace math {
    /**
     * Creates a translation matrix.
     * @param[in] position Position for the matrix to represent.
     * @return Translation matrix.
     */
    template<typename T>
    inline Mat4<T> translate( const Vec3<T>& position );

    /**
     * Creates a rotation matrix around a given axis and angle.
     * @param[in] angle Angle of the rotation.
     * @param[in] axis  The axis to rotate around.
     * @return Rotation matrix.
     */
    template<typename T>
    inline Mat4<T> rotate( const T& angle, const Vec3<T>& axis );

    /**
     * Creates a scaling matrix.
     * @param[in] size Scale values for the matrix.
     * @return Scaling matrix.
     */
    template<typename T>
    inline Mat4<T> scale( const Vec3<T>& size );

    /**
     * Creates an orthographic projection matrix.
     * @param[in] left   Left coordinate of the viewport.
     * @param[in] right  Right coordinate of the viewport.
     * @param[in] bottom Bottom coordinate of the viewport.
     * @param[in] top    Top coordinate of the viewport.
     * @param[in] near   The near plane.
     * @param[in] far    The far plane.
     * @return Orthographic projection matrix.
     */
    template<typename T>
    inline Mat4<T> orthographic( const T& left, const T& right, const T& bottom, const T& top, const T& near, const T& far );

    /**
     * Creates a frustum matrix.
     * @param[in] left   Left coordinate of the viewport.
     * @param[in] right  Right coordinate of the viewport.
     * @param[in] bottom Bottom coordinate of the viewport.
     * @param[in] top    Top coordinate of the viewport.
     * @param[in] near   Distance to the near plane.
     * @param[in] far    Distance to the far plane.
     * @return Frustum matrix.
     */
    template<typename T>
    inline Mat4<T> frustum( const T& left, const T& right, const T& bottom, const T& top, const T& near, const T& far );

    /**
     * Creates a perspective projection matrix.
     * @param[in] fovY   Field of view vertically.
     * @param[in] aspect Aspect ratio.
     * @param[in] near   Distance to the near plane.
     * @param[in] far    Distance to the far plane.
     * @return Perspective matrix.
     */
    template<typename T>
    inline Mat4<T> perspective( const T& fovY, const T& aspect, const T& near, const T& far );

    /**
     * Creates a lookat matrix.
     * @param[in] eye    Position of the eye.
     * @param[in] target Target to look at.
     * @param[in] up     Local up vector.
     * @return Lookat matrix.
     */
    template<typename T>
    inline Mat4<T> lookAt( const Vec3<T>& eye, const Vec3<T>& target, const Vec3<T>& up );

    /**
     * Inverses a matrix.
     * @param[in] mat Matrix to be inversed.
     * @return An inversed copy of the given matrix.
     */
    template<typename T>
    inline Mat4<T> inverse( const Mat4<T>& mat );

    /**
     * Creates an axis-angle matrix.
     * @param[in] axis  Axis of rotation.
     * @param[in] angle Angle of rotation.
     * @return Axis-angle matrix.
     */
    template<typename T>
    inline Mat4<T> axisAngle( const Vec3<T>& axis, float angle );

		/**
		 * Decomposes a world matrix into translation, rotation, and scale.
		 * @param[in]  mat       World matrix to decompose.
		 * @param[out] outPos    Output translation.
		 * @param[out] outOrient Output orientation quaternion.
		 * @param[out] outScale  Output scale.
		 * @return True if the decomposition was successful, and false otherwise.
		 */
		template<typename T>
		inline bool decompose( const Mat4<T>& mat, Vec3<T>* outPos, Quaternion<T>* outOrient, Vec3<T>* outScale );
  } /* math */
} /* cc */

#include "MatrixFunc.inl"

#endif  /* __CC_MATH_MATRIXFUNC__ */

