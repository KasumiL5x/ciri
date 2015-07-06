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
 * 2014-01-26 11:55
 *  - Added determinant, inverse, and * operator w/ T.
 * 
 * 2014-01-25 16:49
 *  - Added transpose and transposed functions.  How did I forget them before?!
 *
 * 2014-01-25 15:50
 *  - Added getters and setters for forward, backward, up, down, left, right, and translation.
 *
 * 2013-11-15 13:57
 *  - Initial version.
 */

#ifndef __CC_MATH_MAT4__
#define	__CC_MATH_MAT4__

#include "Vec4.hpp"

namespace cc {
  namespace math {
    template<typename T>
    class Mat4 {
    public:
      inline Mat4<T>();
      inline Mat4<T>( const T& val );
      inline Mat4<T>( const T& x0, const T& y0, const T& z0, const T& w0,
                      const T& x1, const T& y1, const T& z1, const T& w1,
                      const T& x2, const T& y2, const T& z2, const T& w2,
                      const T& x3, const T& y3, const T& z3, const T& w3 );
      inline Mat4<T>( const Mat4<T>& val );

      // Accessors.
      inline Vec4<T>&       operator[]( unsigned int index );
      inline const Vec4<T>& operator[]( unsigned int index ) const;
      inline T&             operator()( unsigned int row, unsigned int column );
      inline const T&       operator()( unsigned int row, unsigned int column ) const;

      // Unary arithmetic operators.
      inline Mat4<T>& operator=( const Mat4<T>& val );

      // Binary arithmetic operators.
      // All defined in mat4.inl

    // Get/set components of the matrix.
    inline Vec4<T> getForward    () const;
    inline Vec4<T> getBackward   () const;
    inline Vec4<T> getUp         () const;
    inline Vec4<T> getDown       () const;
    inline Vec4<T> getRight      () const;
    inline Vec4<T> getLeft       () const;
    inline Vec4<T> getTranslation() const;
    inline void    setForward    ( const Vec4<T>& val );
    inline void    setBackward   ( const Vec4<T>& val );
    inline void    setUp         ( const Vec4<T>& val );
    inline void    setDown       ( const Vec4<T>& val );
    inline void    setRight      ( const Vec4<T>& val );
    inline void    setLeft       ( const Vec4<T>& val );
    inline void    setTranslation( const Vec4<T>& val );

    // Other things.
    inline void    transpose ();
    inline Mat4<T> transposed() const;
    inline float   determinant() const;
    inline void    invert();

    public:
      Vec4<T> data[4];
    };
  } /* math */
  
  // Typedefs.
  typedef cc::math::Mat4<float>        Mat4f;
  typedef cc::math::Mat4<double>       Mat4d;
  typedef cc::math::Mat4<int>          Mat4i;
  typedef cc::math::Mat4<unsigned int> Mat4ui;
  
} /* cc */

#include "Mat4.inl"

#endif	/* __CC_MATH_MAT4__ */

