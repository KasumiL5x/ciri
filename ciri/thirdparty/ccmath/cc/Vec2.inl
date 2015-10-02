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
    inline Vec2<T>::Vec2() {
      this->x = static_cast<T>(0);
      this->y = static_cast<T>(0);
    }

    template<typename T>
    inline Vec2<T>::Vec2( const Vec2<T>& vec ) {
      this->x = vec.x;
      this->y = vec.y;
    }

    template<typename T>
    inline Vec2<T>::Vec2( const T& val ) {
      this->x = val;
      this->y = val;
    }

    template<typename T>
    inline Vec2<T>::Vec2( const T& x, const T& y ) {
      this->x = x;
      this->y = y;
    }

    // Accessors.
    template<typename T>
    inline T& Vec2<T>::operator[]( unsigned int index ) {
      assert(index < 2);
      return (&x)[index];
    }

    template<typename T>
    inline const T& Vec2<T>::operator[]( const unsigned int index ) const {
      assert(index < 2);
      return (&x)[index];
    }

    template<typename T>
    inline T& Vec2<T>::operator()( unsigned int index ) {
      assert(index < 2);
      return (&x)[index];
    }

    // Unary arithmetic operators.
    template<typename T>
    inline Vec2<T>& Vec2<T>::operator=( const Vec2<T>& vec ) {
      this->x = vec.x;
      this->y = vec.y;
      return *this;
    }

    template<typename T>
    inline Vec2<T>& Vec2<T>::operator+=( const Vec2<T>& vec ) {
      this->x += vec.x;
      this->y += vec.y;
      return *this;
    }

    template<typename T>
    inline Vec2<T>& Vec2<T>::operator-=( const Vec2<T>& vec ) {
      this->x -= vec.x;
      this->y -= vec.y;
      return *this;
    }

    template<typename T>
    inline Vec2<T>& Vec2<T>::operator*=( const Vec2<T>& vec ) {
      this->x *= vec.x;
      this->y *= vec.y;
      return *this;
    }

    template<typename T>
    inline Vec2<T>& Vec2<T>::operator/=( const Vec2<T>& vec ) {
      this->x /= vec.x;
      this->y /= vec.y;
      return *this;
    }
    
    // Binary arithmetic operators.
    template<typename T>
    inline Vec2<T> operator+( const Vec2<T>& lhs, const Vec2<T>& rhs ) {
      return Vec2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
    }
    template<typename T>
    inline Vec2<T> operator+( const T& lhs, const Vec2<T>& rhs ) {
      return Vec2<T>(lhs + rhs.x, lhs + rhs.y);
    }
    template<typename T>
    inline Vec2<T> operator+( const Vec2<T>& lhs, const T& rhs ) {
      return Vec2<T>(lhs.x + rhs, lhs.y + rhs);
    }

    template<typename T>
    inline Vec2<T> operator-( const Vec2<T>& lhs, const Vec2<T>& rhs ) {
      return Vec2<T>(lhs.x - rhs.x, lhs.y - rhs.y);
    }
    template<typename T>
    inline Vec2<T> operator-( const T& lhs, const Vec2<T>& rhs ) {
      return Vec2<T>(lhs - rhs.x, lhs - rhs.y);
    }
    template<typename T>
    inline Vec2<T> operator-( const Vec2<T>& lhs, const T& rhs ) {
      return Vec2<T>(lhs.x - rhs, lhs.y - rhs);
    }

    template<typename T>
    inline Vec2<T> operator*( const Vec2<T>& lhs, const Vec2<T>& rhs ) {
      return Vec2<T>(lhs.x * rhs.x, lhs.y * rhs.y);
    }
    template<typename T>
    inline Vec2<T> operator*( const T& lhs, const Vec2<T>& rhs ) {
      return Vec2<T>(lhs * rhs.x, lhs * rhs.y);
    }
    template<typename T>
    inline Vec2<T> operator*( const Vec2<T>& lhs, const T& rhs ) {
      return Vec2<T>(lhs.x * rhs, lhs.y * rhs);
    }

    template<typename T>
    inline Vec2<T> operator/( const Vec2<T>& lhs, const Vec2<T>& rhs ) {
      return Vec2<T>(lhs.x / rhs.x, lhs.y / rhs.y);
    }
    template<typename T>
    inline Vec2<T> operator/( const T& lhs, const Vec2<T>& rhs ) {
      return Vec2<T>(lhs / rhs.x, lhs / rhs.y);
    }
    template<typename T>
    inline Vec2<T> operator/( const Vec2<T>& lhs, const T& rhs ) {
      return Vec2<T>(lhs.x / rhs, lhs.y / rhs);
    }

    // Binary comparison operators.
    template<typename T>
    inline bool operator==( const Vec2<T>& lhs, const Vec2<T>& rhs ) {
    	return (lhs.x == rhs.x) && (lhs.y == rhs.y);
    }

    template<typename T>
    inline bool operator !=( const Vec2<T>& lhs, const Vec2<T>& rhs ) {
    	return (lhs.x != rhs.x) || (lhs.y != rhs.y);
    }
    
    // Unary constant operators.
    template<typename T>
    inline Vec2<T> operator-( const Vec2<T>& vec ) {
      return Vec2<T>(-vec.x, -vec.y);
    }

    template<typename T>
    inline std::ostream& operator<<( std::ostream& os, const Vec2<T>& vec ) {
      os << "x[" << vec.x << "], y[" << vec.y << "]" << std::endl;
      return os;
    }

    // Other things.
    template<typename T>
    inline T Vec2<T>::magnitude() const {
      return static_cast<T>(sqrt(x*x + y*y));
    }
    template<typename T>
    inline T Vec2<T>::sqrMagnitude() const {
      return x*x + y*y;
    }
    template<typename T>
    inline void Vec2<T>::normalize() {
      const T eps = static_cast<T>(EPSILON);
      T mag = magnitude();
      mag = (mag <= eps) ? static_cast<T>(1) : mag;

      x /= mag;
      y /= mag;
      x = (fabs(x) < eps) ? static_cast<T>(0) : x;
      y = (fabs(y) < eps) ? static_cast<T>(0) : y;
    }
    template<typename T>
    inline Vec2<T> Vec2<T>::normalized() const {
      const T eps = static_cast<T>(EPSILON);
      T mag = magnitude();
      mag = (mag <= eps) ? static_cast<T>(1) : mag;

      Vec2<T> result(*this);
      result.x /= mag;
      result.y /= mag;
      result.x = (fabs(result.x) < eps) ? static_cast<T>(0) : result.x;
      result.y = (fabs(result.y) < eps) ? static_cast<T>(0) : result.y;
      return result;
    }
    template<typename T>
    inline bool Vec2<T>::equalTo( const Vec2<T>& rhs ) const {
      return sqrMagnitude(*this - rhs) < static_cast<T>(EPSILON);
    }
    template<typename T>
    inline T Vec2<T>::dot( const Vec2<T>& rhs ) const {
      return x*rhs.x + y*rhs.y;
    }
    template<typename T>
    inline T Vec2<T>::cross3d( const Vec2<T>& rhs ) const {
      return x*rhs.y - y*rhs.x;
    }
    template<typename T>
    inline Vec2<T> Vec2<T>::cross2d() const {
      return Vec2<T>(y, -x);
    }
    template<typename T>
    inline T Vec2<T>::distance( const Vec2<T>& rhs ) const {
      return (*this - rhs).magnitude();
    }
		template<typename T>
		inline T Vec2<T>::sqrDistance( const Vec2<T>& rhs ) const {
			return (*this - rhs).sqrMagnitude();
		}
    template<typename T>
    inline Vec2<T> Vec2<T>::minimum( const Vec2<T>& rhs ) const {
      return Vec2<T>(math::minimum<T>(x, rhs.x), math::minimum<T>(y, rhs.y));
    }
    template<typename T>
    inline Vec2<T> Vec2<T>::maximum( const Vec2<T>& rhs ) const {
      return Vec2<T>(math::maximum<T>(x, rhs.x), math::maximum<T>(y, rhs.y));
    }

    template<typename T>
    inline Vec2<T> Vec2<T>::lerp( const Vec2<T>& to, const float& t ) const {
      const float s = math::clamp<float>(t, 0.0f, 1.0f);
      return Vec2<T>(math::lerp<T>(x, to.x, s), math::lerp<T>(y, to.y, s));
    }

    template<typename T>
    inline Vec2<T> Vec2<T>::zero() {
      return Vec2<T>(static_cast<T>(0));
    }

    template<typename T>
    inline Vec2<T> Vec2<T>::one() {
      return Vec2<T>(static_cast<T>(1));
    }

  } /* math */
} /* cc */
