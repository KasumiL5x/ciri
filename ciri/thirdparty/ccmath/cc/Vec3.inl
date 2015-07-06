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

#include <iostream>
#include <cassert>
#include <cmath>
#include "Common.hpp"
#include "Constants.hpp"

namespace cc {
  namespace math {
    template<typename T>
    inline Vec3<T>::Vec3() {
      this->x = static_cast<T>(0);
      this->y = static_cast<T>(0);
      this->z = static_cast<T>(0);
    }

    template<typename T>
    inline Vec3<T>::Vec3( const Vec3<T>& vec ) {
      this->x = vec.x;
      this->y = vec.y;
      this->z = vec.z;
    }

    template<typename T>
    inline Vec3<T>::Vec3( const T& val ) {
      this->x = val;
      this->y = val;
      this->z = val;
    }

    template<typename T>
    inline Vec3<T>::Vec3( const T& x, const T& y, const T& z ) {
      this->x = x;
      this->y = y;
      this->z = z;
    }

    // Accessors.
    template<typename T>
    inline T& Vec3<T>::operator[]( unsigned int index ) {
      assert(index < 3);
      return (&x)[index];
    }

    template<typename T>
    inline const T& Vec3<T>::operator[]( unsigned int index ) const {
      assert(index < 3);
      return (&x)[index];
    }

    template<typename T>
    inline T& Vec3<T>::operator()( unsigned int index ) {
      assert(index < 3);
      return (&x)[index];
    }

    // Unary arithmetic operators.
    template<typename T>
    inline Vec3<T>& Vec3<T>::operator=( const Vec3<T>& vec ) {
      this->x = vec.x;
      this->y = vec.y;
      this->z = vec.z;
      return *this;
    }

    template<typename T>
    inline Vec3<T>& Vec3<T>::operator+=( const Vec3<T>& vec ) {
      this->x += vec.x;
      this->y += vec.y;
      this->z += vec.z;
      return *this;
    }

    template<typename T>
    inline Vec3<T>& Vec3<T>::operator-=( const Vec3<T>& vec ) {
      this->x -= vec.x;
      this->y -= vec.y;
      this->z -= vec.z;
      return *this;
    }

    template<typename T>
    inline Vec3<T>& Vec3<T>::operator*=( const Vec3<T>& vec ) {
      this->x *= vec.x;
      this->y *= vec.y;
      this->z *= vec.z;
      return *this;
    }

    template<typename T>
    inline Vec3<T>& Vec3<T>::operator/=( const Vec3<T>& vec ) {
      this->x /= vec.x;
      this->y /= vec.y;
      this->z /= vec.z;
      return *this;
    }

    // Binary arithmetic operators.
    template<typename T>
    inline Vec3<T> operator+( const Vec3<T>& lhs, const Vec3<T>& rhs ) {
      return Vec3<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
    }
    template<typename T>
    inline Vec3<T> operator+( const T& lhs, const Vec3<T>& rhs ) {
      return Vec3<T>(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
    }
    template<typename T>
    inline Vec3<T> operator+( const Vec3<T>& lhs, const T& rhs ) {
      return Vec3<T>(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
    }

    template<typename T>
    inline Vec3<T> operator-( const Vec3<T>& lhs, const Vec3<T>& rhs ) {
      return Vec3<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
    }
    template<typename T>
    inline Vec3<T> operator-( const T& lhs, const Vec3<T>& rhs ) {
      return Vec3<T>(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
    }
    template<typename T>
    inline Vec3<T> operator-( const Vec3<T>& lhs, const T& rhs ) {
      return Vec3<T>(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs);
    }

    template<typename T>
    inline Vec3<T> operator*( const Vec3<T>& lhs, const Vec3<T>& rhs ) {
      return Vec3<T>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
    }
    template<typename T>
    inline Vec3<T> operator*( const T& lhs, const Vec3<T>& rhs ) {
      return Vec3<T>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
    }
    template<typename T>
    inline Vec3<T> operator*( const Vec3<T>& lhs, const T& rhs ) {
      return Vec3<T>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
    }

    template<typename T>
    inline Vec3<T> operator/( const Vec3<T>& lhs, const Vec3<T>& rhs ) {
      return Vec3<T>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
    }
    template<typename T>
    inline Vec3<T> operator/( const T& lhs, const Vec3<T>& rhs ) {
      return Vec3<T>(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
    }
    template<typename T>
    inline Vec3<T> operator/( const Vec3<T>& lhs, const T& rhs ) {
      return Vec3<T>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
    }

    // Unary constant operators.
    template<typename T>
    inline Vec3<T> operator-( const Vec3<T>& vec ) {
      return Vec3<T>(-vec.x, -vec.y, -vec.z);
    }

    template<typename T>
    inline std::ostream& operator<<( std::ostream& os, const Vec3<T>& vec ) {
      os << "x[" << vec.x << "], y[" << vec.y << "], z[" << vec.z << "]" << std::endl;
      return os;
    }

    // Other things.
    template<typename T>
    inline T Vec3<T>::magnitude() const {
      return static_cast<T>(sqrt(x*x + y*y + z*z));
    }
    
    template<typename T>
    inline T Vec3<T>::sqrMagnitude() const {
      return x*x + y*y + z*z;
    }
    
    template<typename T>
    inline void Vec3<T>::normalize() {
      const T eps = static_cast<T>(EPSILON);
      T mag = magnitude();
      mag = (mag <= eps) ? static_cast<T>(1) : mag;

      x /= mag;
      y /= mag;
      z /= mag;
      x = (fabs(x) < eps) ? static_cast<T>(0) : x;
      y = (fabs(y) < eps) ? static_cast<T>(0) : y;
      z = (fabs(z) < eps) ? static_cast<T>(0) : z;
    }
    
    template<typename T>
    inline Vec3<T> Vec3<T>::normalized() const {
      const T eps = static_cast<T>(EPSILON);
      T mag = magnitude();
      mag = (mag <= eps) ? static_cast<T>(1) : mag;

      Vec3<T> result(*this);
      result.x /= mag;
      result.y /= mag;
      result.z /= mag;

      result.x = (fabs(result.x) < eps) ? static_cast<T>(0) : result.x;
      result.y = (fabs(result.y) < eps) ? static_cast<T>(0) : result.y;
      result.z = (fabs(result.z) < eps) ? static_cast<T>(0) : result.z;
      return result;
    }
    
    template<typename T>
    inline bool Vec3<T>::equalTo( const Vec3<T>& rhs ) const {
      return (*this - rhs).sqrMagnitude() < static_cast<T>(EPSILON);
    }
    
    template<typename T>
    inline T Vec3<T>::dot( const Vec3<T>& rhs ) const {
      return x*rhs.x + y*rhs.y + z*rhs.z;
    }
    
    template<typename T>
    inline Vec3<T> Vec3<T>::cross( const Vec3<T>& rhs ) const {
      return Vec3<T>(y * rhs.z - z * rhs.y, -x * rhs.z + z * rhs.x, x * rhs.y - y * rhs.x);
    }
    
    template<typename T>
    inline T Vec3<T>::distance( const Vec3<T>& rhs ) const {
      return (*this - rhs).magnitude();
    }
    template<typename T>
    inline T Vec3<T>::sqrDistance( const Vec3<T>& rhs ) const {
      return (*this - rhs).sqrMagnitude();
    }
    
    template<typename T>
    inline Vec3<T> Vec3<T>::minimum( const Vec3<T>& rhs ) const {
      return Vec3<T>(math::minimum<T>(x, rhs.x), math::minimum<T>(y, rhs.y), math::minimum<T>(z, rhs.z));
    }
    
    template<typename T>
    inline Vec3<T> Vec3<T>::maximum( const Vec3<T>& rhs ) const {
      return Vec3<T>(math::maximum<T>(x, rhs.x), math::maximum<T>(y, rhs.y), math::maximum<T>(z, rhs.z));
    }
    
    template<typename T>
    inline Vec3<T> Vec3<T>::lerp( const Vec3<T>& to, const float& t ) {
      const float s = clamp<float>(t, 0.0f, 1.0f);
      return Vec3<T>(math::lerp<T>(x, to.x, s), math::lerp<T>(y, to.y, s), math::lerp<T>(z, to.z, s));
    }
    
    template<typename T>
    inline Vec3<T> Vec3<T>::reflect( const Vec3<T>& direction ) {
      return static_cast<T>(-2) * direction.dot(*this) * direction + (*this);
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::zero() {
      return Vec3<T>(static_cast<T>(0));
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::one() {
      return Vec3<T>(static_cast<T>(1));
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::up() {
      return Vec3<T>(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0));
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::down() {
      return Vec3<T>(static_cast<T>(0), static_cast<T>(-1), static_cast<T>(0));
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::left() {
      return Vec3<T>(static_cast<T>(-1), static_cast<T>(0), static_cast<T>(0));
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::right() {
      return Vec3<T>(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0));
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::forward() {
      return Vec3<T>(static_cast<T>(0), static_cast<T>(0), static_cast<T>(-1));
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::backward() {
      return Vec3<T>(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
    }


  } /* math */
} /* cc */
