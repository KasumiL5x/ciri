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
 */

#ifndef __CC_MATH_VEC2__
#define	__CC_MATH_VEC2__

namespace cc {
  namespace math {
    template<typename T>
    class Vec2 {
    public:
      inline Vec2();
      inline Vec2( const Vec2<T>& vec );
      inline Vec2( const T& val );
      inline Vec2( const T& x, const T& y );

      // Accessors.
      inline T&       operator[]( unsigned int index );
      inline const T& operator[]( const unsigned int index ) const;
      inline T&       operator()( unsigned int index );

      // Unary arithmetic operators.
      inline Vec2<T>& operator= ( const Vec2<T>& vec );
      inline Vec2<T>& operator+=( const Vec2<T>& vec );
      inline Vec2<T>& operator-=( const Vec2<T>& vec );
      inline Vec2<T>& operator*=( const Vec2<T>& vec );
      inline Vec2<T>& operator/=( const Vec2<T>& vec );

      // Binary arithmetic operators.
      // All defined in vec2.inl.

      // Binary comparison operators.
      // All defined in vec2.inl.

      // Unary constant operators.
      // All defined in vec2.inl.

      // Other things.
      inline T       magnitude   () const;
      inline T       sqrMagnitude() const;
      inline void    normalize   ();
      inline Vec2<T> normalized  () const;
      inline bool    equalTo     ( const Vec2<T>& rhs ) const;
      inline T       dot         ( const Vec2<T>& rhs ) const;
      // Treats as 3d vectors and returns the magnitude of the 3d cross product
      // where the Z component is zero.  Returned value is the area of the parallelogram
      // created by the two vectors.
      inline T       cross3d     ( const Vec2<T>& rhs ) const;
      // Returns a vector perpendicular to this one.
      inline Vec2<T> cross2d     () const;
      inline T       distance    ( const Vec2<T>& rhs ) const;
      inline Vec2<T> minimum     ( const Vec2<T>& rhs ) const;
      inline Vec2<T> maximum     ( const Vec2<T>& rhs ) const;
      inline Vec2<T> lerp        ( const Vec2<T>& to, const float& t ) const;

      // Static stuff.
      inline static Vec2<T> zero();
      inline static Vec2<T> one();

    public:
      union {
        struct {
          T x, y;
        };
        struct {
          T r, g;
        };
      };
    };
  } /* math */
  
  // Typedefs.
  typedef cc::math::Vec2<float>        Vec2f;
  typedef cc::math::Vec2<double>       Vec2d;
  typedef cc::math::Vec2<int>          Vec2i;
  typedef cc::math::Vec2<unsigned int> Vec2ui;
  
} /* cc */

// Inline implementations.
#include "Vec2.inl"

#endif	/* __CC_MATH_VEC2__ */

