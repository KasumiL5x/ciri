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

#include "Mat4.hpp"

namespace cc {
  namespace math {
    template<typename T>
    inline Mat4<T>::Mat4() {
      const T zero = static_cast<T>(0);
      const T one  = static_cast<T>(1);
      data[0] = Vec4<T>(one, zero, zero, zero);
      data[1] = Vec4<T>(zero, one, zero, zero);
      data[2] = Vec4<T>(zero, zero, one, zero);
      data[3] = Vec4<T>(zero, zero, zero, one);
    }

    template<typename T>
    inline Mat4<T>::Mat4( const T& val ) {
      const T zero = static_cast<T>(0);
      data[0] = Vec4<T>(val, zero, zero, zero);
      data[1] = Vec4<T>(zero, val, zero, zero);
      data[2] = Vec4<T>(zero, zero, val, zero);
      data[3] = Vec4<T>(zero, zero, zero, val);
    }

    template<typename T>
    inline Mat4<T>::Mat4( const T& x0, const T& y0, const T& z0, const T& w0,
          const T& x1, const T& y1, const T& z1, const T& w1,
          const T& x2, const T& y2, const T& z2, const T& w2,
          const T& x3, const T& y3, const T& z3, const T& w3 ) {
      this->data[0] = Vec4<T>(x0, y0, z0, w0);
      this->data[1] = Vec4<T>(x1, y1, z1, w1);
      this->data[2] = Vec4<T>(x2, y2, z2, w2);
      this->data[3] = Vec4<T>(x3, y3, z3, w3);
    }

    template<typename T>
    inline Mat4<T>::Mat4( const Mat4<T>& val ) {
      this->data[0] = val.data[0];
      this->data[1] = val.data[1];
      this->data[2] = val.data[2];
      this->data[3] = val.data[3];
    }

    // Accessors.
    template<typename T>
    inline Vec4<T>& Mat4<T>::operator[]( unsigned int index ) {
      assert(index < 4);
      return this->data[index];
    }

    template<typename T>
    inline const Vec4<T>& Mat4<T>::operator[]( unsigned int index ) const {
      assert(index < 4);
      return this->data[index];
    }

    template<typename T>
    inline T& Mat4<T>::operator()( unsigned int row, unsigned int column ) {
      assert(row < 4 && column < 4);
      return this->data[row][column];
    }

    template<typename T>
    inline const T& Mat4<T>::operator()( unsigned int row, unsigned int column ) const {
      assert(row < 4 && column < 4);
      return this->data[row][column];
    }

    // Unary arithmetic operators.
    template<typename T>
    inline Mat4<T>& Mat4<T>::operator=( const Mat4<T>& val ) {
      this->data[0] = val.data[0];
      this->data[1] = val.data[1];
      this->data[2] = val.data[2];
      this->data[3] = val.data[3];
      return *this;
    }

    // Binary arithmetic operators.
    template<typename T>
    inline Vec4<T> operator*( const Mat4<T>& lhs, const Vec4<T>& rhs ) {
      return Vec4<T>(lhs[0][0] * rhs.x + lhs[1][0] * rhs.y + lhs[2][0] * rhs.z + lhs[3][0] * rhs.w,
                     lhs[0][1] * rhs.x + lhs[1][1] * rhs.y + lhs[2][1] * rhs.z + lhs[3][1] * rhs.w,
                     lhs[0][2] * rhs.x + lhs[1][2] * rhs.y + lhs[2][2] * rhs.z + lhs[3][2] * rhs.w,
                     lhs[0][3] * rhs.x + lhs[1][3] * rhs.y + lhs[2][3] * rhs.z + lhs[3][3] * rhs.w);
    }

    template<typename T>
    inline Vec4<T> operator*( const Vec4<T>& lhs, const Mat4<T>& rhs ) {
      return Vec4<T>(rhs[0][0] * lhs.x + rhs[0][1] * lhs.y + rhs[0][2] * lhs.z + rhs[0][3] * lhs.w,
                      rhs[1][0] * lhs.x + rhs[1][1] * lhs.y + rhs[1][2] * lhs.z + rhs[1][3] * lhs.w,
                      rhs[2][0] * lhs.x + rhs[2][1] * lhs.y + rhs[2][2] * lhs.z + rhs[2][3] * lhs.w,
                      rhs[3][0] * lhs.x + rhs[3][1] * lhs.y + rhs[3][2] * lhs.z + rhs[3][3] * lhs.w);

    }

    template<typename T>
    inline Mat4<T> operator*( const Mat4<T>& lhs, const Mat4<T>& rhs ) {
      const Vec4<T> srcA0 = lhs[0];
      const Vec4<T> srcA1 = lhs[1];
      const Vec4<T> srcA2 = lhs[2];
      const Vec4<T> srcA3 = lhs[3];

      const Vec4<T> srcB0 = rhs[0];
      const Vec4<T> srcB1 = rhs[1];
      const Vec4<T> srcB2 = rhs[2];
      const Vec4<T> srcB3 = rhs[3];

      Mat4<T> result;
      result[0] = srcA0 * srcB0[0] + srcA1 * srcB0[1] + srcA2 * srcB0[2] + srcA3 * srcB0[3];
		  result[1] = srcA0 * srcB1[0] + srcA1 * srcB1[1] + srcA2 * srcB1[2] + srcA3 * srcB1[3];
		  result[2] = srcA0 * srcB2[0] + srcA1 * srcB2[1] + srcA2 * srcB2[2] + srcA3 * srcB2[3];
		  result[3] = srcA0 * srcB3[0] + srcA1 * srcB3[1] + srcA2 * srcB3[2] + srcA3 * srcB3[3];
      return result;
    }
    
    template<typename T>
    inline Mat4<T> operator*( const Mat4<T>& lhs, const T& rhs ) {
      Mat4<T> result;
      result[0] = lhs[0] * rhs;
      result[1] = lhs[1] * rhs;
      result[2] = lhs[2] * rhs;
      result[3] = lhs[3] * rhs;
      return result;
    }

    template<typename T>
    inline std::ostream& operator<<( std::ostream& os, const Mat4<T>& m ) {
      os << m.data[0] << m.data[1] << m.data[2] << m.data[3];
      return os;
    }

    // Get/set components of the matrix.
    template<typename T>
    inline Vec4<T> Mat4<T>::getForward() const {
      return -data[2];
    }

    template<typename T>
    inline Vec4<T> Mat4<T>::getBackward() const {
      return data[2];
    }

    template<typename T>
    inline Vec4<T> Mat4<T>::getUp() const {
      return data[1];
    }

    template<typename T>
    inline Vec4<T> Mat4<T>::getDown() const {
      return -data[1];
    }

    template<typename T>
    inline Vec4<T> Mat4<T>::getRight() const {
      return data[0];
    }

    template<typename T>
    inline Vec4<T> Mat4<T>::getLeft() const {
      return -data[0];
    }

    template<typename T>
    inline Vec4<T> Mat4<T>::getTranslation() const {
      return data[3];
    }

    template<typename T>
    inline void Mat4<T>::setForward( const Vec4<T>& val ) {
      data[2] = -val;
    }

    template<typename T>    
    inline void Mat4<T>::setBackward( const Vec4<T>& val ) {
      data[2] = val;
    }

    template<typename T>    
    inline void Mat4<T>::setUp( const Vec4<T>& val ) {
      data[1] = val;
    }

    template<typename T>    
    inline void Mat4<T>::setDown( const Vec4<T>& val ) {
      data[1] = -val;
    }

    template<typename T>    
    inline void Mat4<T>::setRight( const Vec4<T>& val ) {
      data[0] = val;
    }

    template<typename T>    
    inline void Mat4<T>::setLeft( const Vec4<T>& val ) {
      data[0] = -val;
    }

    template<typename T>    
    inline void Mat4<T>::setTranslation( const Vec4<T>& val ) {
      data[3] = val;
    }

    template<typename T>
    inline void Mat4<T>::transpose() {
      const T x0 = data[0][0];
      const T x1 = data[0][1];
      const T x2 = data[0][2];
      const T x3 = data[0][3];
      const T y0 = data[1][0];
      const T y1 = data[1][1];
      const T y2 = data[1][2];
      const T y3 = data[1][3];
      const T z0 = data[2][0];
      const T z1 = data[2][1];
      const T z2 = data[2][2];
      const T z3 = data[2][3];
      const T w0 = data[3][0];
      const T w1 = data[3][1];
      const T w2 = data[3][2];
      const T w3 = data[3][3];

      data[0][0] = x0; data[0][1] = y0; data[0][2] = z0; data[0][3] = w0;
      data[1][0] = x1; data[1][1] = y1; data[1][2] = z1; data[1][3] = w1;
      data[2][0] = x2; data[2][1] = y2; data[2][2] = z2; data[2][3] = w2;
      data[3][0] = x3; data[3][1] = y3; data[3][2] = z3; data[3][3] = w3;
    }

    template<typename T>
    inline Mat4<T> Mat4<T>::transposed() const {
      return Mat4<T>(data[0][0], data[1][0], data[2][0], data[3][0],
                     data[0][1], data[1][1], data[2][1], data[3][1],
                     data[0][2], data[1][2], data[2][2], data[3][2],
                     data[0][3], data[1][3], data[2][3], data[3][3]);
    }
    
    template<typename T>
    inline float Mat4<T>::determinant() const {
      // http://www.euclideanspace.com/maths/algebra/matrix/functions/determinant/fourD/index.htm
      // This needs optimizing (some elements are accessed multiple times), but it's Sunday morning and I'm tired.
      return static_cast<float>(
      data[0][3] * data[1][2] * data[2][1] * data[3][0] - data[0][2] * data[1][3] * data[2][1] * data[3][0] - data[0][3] *
      data[1][1] * data[2][2] * data[3][0] + data[0][1] * data[1][3] * data[2][2] * data[3][0] +
      data[0][2] * data[1][1] * data[2][3] * data[3][0] - data[0][1] * data[1][2] * data[2][3] * data[3][0] - data[0][3] *
      data[1][2] * data[2][0] * data[3][1] + data[0][2] * data[1][3] * data[2][0] * data[3][1] +
      data[0][3] * data[1][0] * data[2][2] * data[3][1] - data[0][0] * data[1][3] * data[2][2] * data[3][1] - data[0][2] *
      data[1][0] * data[2][3] * data[3][1] + data[0][0] * data[1][2] * data[2][3] * data[3][1] +
      data[0][3] * data[1][1] * data[2][0] * data[3][2] - data[0][1] * data[1][3] * data[2][0] * data[3][2] - data[0][3] *
      data[1][0] * data[2][1] * data[3][2] + data[0][0] * data[1][3] * data[2][1] * data[3][2] +
      data[0][1] * data[1][0] * data[2][3] * data[3][2] - data[0][0] * data[1][1] * data[2][3] * data[3][2] - data[0][2] *
      data[1][1] * data[2][0] * data[3][3] + data[0][1] * data[1][2] * data[2][0] * data[3][3] +
      data[0][2] * data[1][0] * data[2][1] * data[3][3] - data[0][0] * data[1][2] * data[2][1] * data[3][3] - data[0][1] *
      data[1][0] * data[2][2] * data[3][3] + data[0][0] * data[1][1] * data[2][2] * data[3][3]);
    }
    
    template<typename T>
    inline void Mat4<T>::invert() {
      // Calculate determinant.
      const float deter = determinant();
      // If it's 0, this matrix can't be inverted.
      if( math::equal(deter, 0.0f) ) {
        return;
      }
      
      Mat4<T> inv;
      inv[0][0] = data[1][2]*data[2][3]*data[3][1] - data[1][3]*data[2][2]*data[3][1] + data[1][3]*data[2][1]*data[3][2] - data[1][1]*data[2][3]*data[3][2] - data[1][2]*data[2][1]*data[3][3] + data[1][1]*data[2][2]*data[3][3];
      inv[0][1] = data[0][3]*data[2][2]*data[3][1] - data[0][2]*data[2][3]*data[3][1] - data[0][3]*data[2][1]*data[3][2] + data[0][1]*data[2][3]*data[3][2] + data[0][2]*data[2][1]*data[3][3] - data[0][1]*data[2][2]*data[3][3];
      inv[0][2] = data[0][2]*data[1][3]*data[3][1] - data[0][3]*data[1][2]*data[3][1] + data[0][3]*data[1][1]*data[3][2] - data[0][1]*data[1][3]*data[3][2] - data[0][2]*data[1][1]*data[3][3] + data[0][1]*data[1][2]*data[3][3];
      inv[0][3] = data[0][3]*data[1][2]*data[2][1] - data[0][2]*data[1][3]*data[2][1] - data[0][3]*data[1][1]*data[2][2] + data[0][1]*data[1][3]*data[2][2] + data[0][2]*data[1][1]*data[2][3] - data[0][1]*data[1][2]*data[2][3];
      inv[1][0] = data[1][3]*data[2][2]*data[3][0] - data[1][2]*data[2][3]*data[3][0] - data[1][3]*data[2][0]*data[3][2] + data[1][0]*data[2][3]*data[3][2] + data[1][2]*data[2][0]*data[3][3] - data[1][0]*data[2][2]*data[3][3];
      inv[1][1] = data[0][2]*data[2][3]*data[3][0] - data[0][3]*data[2][2]*data[3][0] + data[0][3]*data[2][0]*data[3][2] - data[0][0]*data[2][3]*data[3][2] - data[0][2]*data[2][0]*data[3][3] + data[0][0]*data[2][2]*data[3][3];
      inv[1][2] = data[0][3]*data[1][2]*data[3][0] - data[0][2]*data[1][3]*data[3][0] - data[0][3]*data[1][0]*data[3][2] + data[0][0]*data[1][3]*data[3][2] + data[0][2]*data[1][0]*data[3][3] - data[0][0]*data[1][2]*data[3][3];
      inv[1][3] = data[0][2]*data[1][3]*data[2][0] - data[0][3]*data[1][2]*data[2][0] + data[0][3]*data[1][0]*data[2][2] - data[0][0]*data[1][3]*data[2][2] - data[0][2]*data[1][0]*data[2][3] + data[0][0]*data[1][2]*data[2][3];
      inv[2][0] = data[1][1]*data[2][3]*data[3][0] - data[1][3]*data[2][1]*data[3][0] + data[1][3]*data[2][0]*data[3][1] - data[1][0]*data[2][3]*data[3][1] - data[1][1]*data[2][0]*data[3][3] + data[1][0]*data[2][1]*data[3][3];
      inv[2][1] = data[0][3]*data[2][1]*data[3][0] - data[0][1]*data[2][3]*data[3][0] - data[0][3]*data[2][0]*data[3][1] + data[0][0]*data[2][3]*data[3][1] + data[0][1]*data[2][0]*data[3][3] - data[0][0]*data[2][1]*data[3][3];
      inv[2][2] = data[0][1]*data[1][3]*data[3][0] - data[0][3]*data[1][1]*data[3][0] + data[0][3]*data[1][0]*data[3][1] - data[0][0]*data[1][3]*data[3][1] - data[0][1]*data[1][0]*data[3][3] + data[0][0]*data[1][1]*data[3][3];
      inv[2][3] = data[0][3]*data[1][1]*data[2][0] - data[0][1]*data[1][3]*data[2][0] - data[0][3]*data[1][0]*data[2][1] + data[0][0]*data[1][3]*data[2][1] + data[0][1]*data[1][0]*data[2][3] - data[0][0]*data[1][1]*data[2][3];
      inv[3][0] = data[1][2]*data[2][1]*data[3][0] - data[1][1]*data[2][2]*data[3][0] - data[1][2]*data[2][0]*data[3][1] + data[1][0]*data[2][2]*data[3][1] + data[1][1]*data[2][0]*data[3][2] - data[1][0]*data[2][1]*data[3][2];
      inv[3][1] = data[0][1]*data[2][2]*data[3][0] - data[0][2]*data[2][1]*data[3][0] + data[0][2]*data[2][0]*data[3][1] - data[0][0]*data[2][2]*data[3][1] - data[0][1]*data[2][0]*data[3][2] + data[0][0]*data[2][1]*data[3][2];
      inv[3][2] = data[0][2]*data[1][1]*data[3][0] - data[0][1]*data[1][2]*data[3][0] - data[0][2]*data[1][0]*data[3][1] + data[0][0]*data[1][2]*data[3][1] + data[0][1]*data[1][0]*data[3][2] - data[0][0]*data[1][1]*data[3][2];
      inv[3][3] = data[0][1]*data[1][2]*data[2][0] - data[0][2]*data[1][1]*data[2][0] + data[0][2]*data[1][0]*data[2][1] - data[0][0]*data[1][2]*data[2][1] - data[0][1]*data[1][0]*data[2][2] + data[0][0]*data[1][1]*data[2][2];
      
      const float oneOverDet = 1.0f / deter;
      *this = inv * oneOverDet;
    }

  } /* math */
} /* cc */
