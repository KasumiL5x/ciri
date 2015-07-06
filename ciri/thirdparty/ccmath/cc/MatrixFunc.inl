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

#include <cmath>

namespace cc {
  namespace math {
    template<typename T>
    inline Mat4<T> translate( const Vec3<T>& position ) {
      Mat4<T> result;
      result[3] = Vec4<T>(position.x, position.y, position.z, static_cast<T>(1));
      return result;
    }

    template<typename T>
    inline Mat4<T> rotate( const T& angle, const Vec3<T>& axis ) {
      const T angleRads = degreesToRadians<T>(angle);
      const T c         = cos(angleRads);
      const T s         = sin(angleRads);
      const Vec3<T> axisNorm = axis.normalized();
      const Vec3<T> tmp = (static_cast<T>(1) - c) * axisNorm;

      Mat4<T> rotated;
      rotated[0][0] = c + tmp[0] * axisNorm[0];
      rotated[0][1] =     tmp[0] * axisNorm[1] + s * axisNorm[2];
      rotated[0][2] =     tmp[0] * axisNorm[2] - s * axisNorm[1];

      rotated[1][0] =     tmp[1] * axisNorm[0] - s * axisNorm[2];
      rotated[1][1] = c + tmp[1] * axisNorm[1];
      rotated[1][2] =     tmp[1] * axisNorm[2] + s * axisNorm[0];

      rotated[2][0] =     tmp[2] * axisNorm[0] + s * axisNorm[1];
      rotated[2][1] =     tmp[2] * axisNorm[1] - s * axisNorm[0];
      rotated[2][2] = c + tmp[2] * axisNorm[2];
      return rotated;
    }

    template<typename T>
    inline Mat4<T> scale( const Vec3<T>& size ) {
      Mat4<T> scaled;
      scaled[0][0] = size.x;
      scaled[1][1] = size.y;
      scaled[2][2] = size.z;
      return scaled;
    }

    template<typename T>
    inline Mat4<T> orthographic( const T& left, const T& right, const T& bottom, const T& top, const T& near, const T& far ) {
      Mat4<T> ortho;
      ortho[0][0] = static_cast<T>(2) / (right - left);
      ortho[1][1] = static_cast<T>(2) / (top - bottom);
      ortho[2][2] = static_cast<T>(-1) / (far - near);
      ortho[3][0] = -(right + left) / (right - left);
      ortho[3][1] = -(top + bottom) / (top - bottom);
      ortho[3][2] = -near / (far - near);
      return ortho;
    }

    template<typename T>
    inline Mat4<T> frustum( const T& left, const T& right, const T& bottom, const T& top, const T& near, const T& far ) {
      Mat4<T> frust(static_cast<T>(0));
      frust[0][0] = (static_cast<T>(2) * near) / (right - left);
      frust[1][1] = (static_cast<T>(2) * near) / (top - bottom);
      frust[2][0] = (right + left) / (right - left);
      frust[2][1] = (top + bottom) / (top - bottom);
      frust[2][2] = -(far + near) / (far - near);
      frust[2][3] = static_cast<T>(-1);
      frust[3][2] = -(static_cast<T>(2) * far * near) / (far - near);
      return frust;
    }

    template<typename T>
    inline Mat4<T> perspective( const T& fovY, const T& aspect, const T& near, const T& far ) {

      const T tanHalfFovY = tan(degreesToRadians<T>(fovY) / static_cast<T>(2));

      // maybe this needs to be RH instead of LH?

      Mat4<T> persp(static_cast<T>(0));
      persp[0][0] = static_cast<T>(1) / (aspect * tanHalfFovY);
      persp[1][1] = static_cast<T>(1) / (tanHalfFovY);
      persp[2][2] = -(far + near) / (far - near);
      persp[2][3] = -static_cast<T>(1);
      persp[3][2] = -(static_cast<T>(2) * far * near) / (far - near);
      return persp;

      // const T range  = tan(degreesToRadians<T>(fovY / static_cast<T>(2))) * near;
      // const T left   = -range * aspect;
      // const T right  = range * aspect;
      // const T bottom = -range;
      // const T top    = range;

      // Mat4<T> persp(static_cast<T>(0));
      // persp[0][0] = (static_cast<T>(2) * near) / (right - left);
      // persp[1][1] = (static_cast<T>(2) * near) / (top - bottom);
      // persp[2][2] = -(far + near) / (far - near);
      // persp[2][3] = -static_cast<T>(1);
      // persp[3][2] = -(static_cast<T>(2) * far * near) / (far - near);
      // return persp;
    }

    template<typename T>
    inline Mat4<T> lookAt( const Vec3<T>& eye, const Vec3<T>& target, const Vec3<T>& up ) {
      const Vec3<T> f = (target - eye).normalized();
      const Vec3<T> s = f.cross(up).normalized();
      const Vec3<T> u = s.cross(f);

      Mat4<T> result(static_cast<T>(1));
      result[0][0] = s.x;
      result[1][0] = s.y;
      result[2][0] = s.z;
      result[0][1] = u.x;
      result[1][1] = u.y;
      result[2][1] = u.z;
      result[0][2] = -f.x;
      result[1][2] = -f.y;
      result[2][2] = -f.z;
      result[3][0] = -s.dot(eye);
      result[3][1] = -u.dot(eye);
      result[3][2] = f.dot(eye);
      return result;


      // const Vec3<T> f = (target- eye).normalized();
      // Vec3<T> u       = up.normalized();
      // const Vec3<T> s = (f.cross(u)).normalized();
      // u = s.cross(f);

      // Mat4<T> look(static_cast<T>(1));
      // look[0][0] = s.x;
      // look[1][0] = s.y;
      // look[2][0] = s.z;
      // look[0][1] = u.x;
      // look[1][1] = u.y;
      // look[2][1] = u.z;
      // look[0][2] = -f.x;
      // look[1][2] = -f.y;
      // look[2][2] = -f.z;
      // look[3][0] = -s.dot(eye);
      // look[3][1] = -u.dot(eye);
      // look[3][2] = f.dot(eye);
      // return look;
    }

    template<typename T>
    inline Mat4<T> inverse( const Mat4<T>& mat ) {
      T a0 = mat.data[0][0]*mat.data[1][1] - mat.data[0][1]*mat.data[1][0];
      T a1 = mat.data[0][0]*mat.data[1][2] - mat.data[0][2]*mat.data[1][0];
      T a2 = mat.data[0][0]*mat.data[1][3] - mat.data[0][3]*mat.data[1][0];
      T a3 = mat.data[0][1]*mat.data[1][2] - mat.data[0][2]*mat.data[1][1];
      T a4 = mat.data[0][1]*mat.data[1][3] - mat.data[0][3]*mat.data[1][1];
      T a5 = mat.data[0][2]*mat.data[1][3] - mat.data[0][3]*mat.data[1][2];
      T b0 = mat.data[2][0]*mat.data[3][1] - mat.data[2][1]*mat.data[3][0];
      T b1 = mat.data[2][0]*mat.data[3][2] - mat.data[2][2]*mat.data[3][0];
      T b2 = mat.data[2][0]*mat.data[3][3] - mat.data[2][3]*mat.data[3][0];
      T b3 = mat.data[2][1]*mat.data[3][2] - mat.data[2][2]*mat.data[3][1];
      T b4 = mat.data[2][1]*mat.data[3][3] - mat.data[2][3]*mat.data[3][1];
      T b5 = mat.data[2][2]*mat.data[3][3] - mat.data[2][3]*mat.data[3][2];

      T det = a0*b5 - a1*b4 + a2*b3 + a3*b2 - a4*b1 + a5*b0;
      if ( fabs(det) > math::EPSILON )
      {
          Mat4<T> inv;
          inv.data[0][0] = + mat.data[1][1]*b5 - mat.data[1][2]*b4 + mat.data[1][3]*b3;
          inv.data[1][0] = - mat.data[1][0]*b5 + mat.data[1][2]*b2 - mat.data[1][3]*b1;
          inv.data[2][0] = + mat.data[1][0]*b4 - mat.data[1][1]*b2 + mat.data[1][3]*b0;
          inv.data[3][0] = - mat.data[1][0]*b3 + mat.data[1][1]*b1 - mat.data[1][2]*b0;
          inv.data[0][1] = - mat.data[0][1]*b5 + mat.data[0][2]*b4 - mat.data[0][3]*b3;
          inv.data[1][1] = + mat.data[0][0]*b5 - mat.data[0][2]*b2 + mat.data[0][3]*b1;
          inv.data[2][1] = - mat.data[0][0]*b4 + mat.data[0][1]*b2 - mat.data[0][3]*b0;
          inv.data[3][1] = + mat.data[0][0]*b3 - mat.data[0][1]*b1 + mat.data[0][2]*b0;
          inv.data[0][2] = + mat.data[3][1]*a5 - mat.data[3][2]*a4 + mat.data[3][3]*a3;
          inv.data[1][2] = - mat.data[3][0]*a5 + mat.data[3][2]*a2 - mat.data[3][3]*a1;
          inv.data[2][2] = + mat.data[3][0]*a4 - mat.data[3][1]*a2 + mat.data[3][3]*a0;
          inv.data[3][2] = - mat.data[3][0]*a3 + mat.data[3][1]*a1 - mat.data[3][2]*a0;
          inv.data[0][3] = - mat.data[2][1]*a5 + mat.data[2][2]*a4 - mat.data[2][3]*a3;
          inv.data[1][3] = + mat.data[2][0]*a5 - mat.data[2][2]*a2 + mat.data[2][3]*a1;
          inv.data[2][3] = - mat.data[2][0]*a4 + mat.data[2][1]*a2 - mat.data[2][3]*a0;
          inv.data[3][3] = + mat.data[2][0]*a3 - mat.data[2][1]*a1 + mat.data[2][2]*a0;

          T invDet = ((T)1)/det;
          inv.data[0][0] *= invDet;
          inv.data[0][1] *= invDet;
          inv.data[0][2] *= invDet;
          inv.data[0][3] *= invDet;
          inv.data[1][0] *= invDet;
          inv.data[1][1] *= invDet;
          inv.data[1][2] *= invDet;
          inv.data[1][3] *= invDet;
          inv.data[2][0] *= invDet;
          inv.data[2][1] *= invDet;
          inv.data[2][2] *= invDet;
          inv.data[2][3] *= invDet;
          inv.data[3][0] *= invDet;
          inv.data[3][1] *= invDet;
          inv.data[3][2] *= invDet;
          inv.data[3][3] *= invDet;

          return inv;
      }

      return Mat4<T>(static_cast<T>(1));
    }

    template<typename T>
    inline Mat4<T> axisAngle( const Vec3<T>& axis, float angle ) {
      const float c = cos(angle);
      const float s = sin(angle);
      const float invC = 1.0f - c;

      Mat4<T> mat;

      mat[0][0] = c + (axis.x * axis.x) * invC;
      mat[0][1] = axis.x * axis.y * invC + axis.z * s;
      mat[0][2] = axis.x * axis.z * invC - axis.y * s;
      mat[0][3] = 0.0f;

      mat[1][0] = axis.y * axis.x * invC - axis.z * s;
      mat[1][1] = c + (axis.y * axis.y) * invC;
      mat[1][2] = axis.y * axis.z * invC + axis.x * s;
      mat[1][3] = 0.0f;

      mat[2][0] = axis.z * axis.x * invC + axis.y * s;
      mat[2][1] = axis.z * axis.y * invC - axis.x * s;
      mat[2][2] = c + (axis.z * axis.z) * invC;
      mat[2][3] = 0.0f;

      mat[3][0] = 0.0f;
      mat[3][1] = 0.0f;
      mat[3][2] = 0.0f;
      mat[3][3] = 1.0f;

      return mat;
    }

		template<typename T>
		inline bool decompose( const Mat4<T>& mat, Vec3<T>* outPos, Quaternion<T>* outOrient, Vec3<T>* outScale ) {
			if( !outPos || !outOrient || !outScale )
			{
				return false;
			}

			outPos->x = mat[3][0];
			outPos->y = mat[3][1];
			outPos->z = mat[3][2];

			float xs = 0.0f;
			float ys = 0.0f;
			float zs = 0.0f;
			if( math::sign(mat[0][0] * mat[0][1] * mat[0][2] * mat[0][3]) < 0 )
			{
				xs = -1.0f;
			}
			else
			{
				xs = 1.0f;
			}
			if( math::sign(mat[1][0] * mat[1][1] * mat[1][2] * mat[1][3]) < 0 )
			{
				ys = -1.0f;
			}
			else
			{
				ys = 1.0f;
			}
			if( math::sign(mat[2][0] * mat[2][1] * mat[2][2] * mat[2][3]) < 0 )
			{
				zs = -1.0f;
			}
			else
			{
				zs = 1.0f;
			}
			outScale->x = xs * sqrtf(mat[0][0] * mat[0][0] + mat[0][1] * mat[0][1] + mat[0][2] * mat[0][2]);
			outScale->y = ys * sqrtf(mat[1][0] * mat[1][0] + mat[1][1] * mat[1][1] + mat[1][2] * mat[1][2]);
			outScale->z = zs * sqrtf(mat[2][0] * mat[2][0] + mat[2][1] * mat[2][1] + mat[2][2] * mat[2][2]);

			if( math::equal(outScale->x, 0.0f) || math::equal(outScale->y, 0.0f) || math::equal(outScale->z, 0.0f) )
			{
				*outOrient = Quatf();
				return false;
			}

			const Mat4f otherMat(mat[0][0] / outScale->x, mat[0][1] / outScale->x, mat[0][2] / outScale->x, 0.0f,
													 mat[1][0] / outScale->y, mat[1][1] / outScale->y, mat[1][2] / outScale->y, 0.0f,
													 mat[2][0] / outScale->z, mat[2][1] / outScale->z, mat[2][2] / outScale->z, 0.0f,
													 0.0f, 0.0f, 0.0f, 1.0f);
			*outOrient = Quatf::createFromMatrix(otherMat);
			return true;
		}
  } /* math */
} /* cc */
