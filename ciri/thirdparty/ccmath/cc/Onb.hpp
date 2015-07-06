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

#ifndef __CC_MATH_ONB__
#define __CC_MATH_ONB__

#include "Vec3.hpp"

namespace cc {
  namespace math {
    // Orthonormal basis
    template<typename T>
    class Onb {
    public:
      inline Onb( );
      inline Onb( const Vec3<T>& a, const Vec3<T>& b, const Vec3<T>& c );

      inline const Vec3<T>& u( ) const;
      inline const Vec3<T>& v( ) const;
      inline const Vec3<T>& w( ) const;

      inline void initFromU ( const Vec3<T>& u );
      inline void initFromV ( const Vec3<T>& v );
      inline void initFromW ( const Vec3<T>& w );
      inline void initFromUV( const Vec3<T>& u, const Vec3<T>& v );
      inline void initFromVU( const Vec3<T>& v, const Vec3<T>& u );
      inline void initFromUW( const Vec3<T>& u, const Vec3<T>& w );
      inline void initFromWU( const Vec3<T>& w, const Vec3<T>& u );
      inline void initFromVW( const Vec3<T>& v, const Vec3<T>& w );
      inline void initFromWV( const Vec3<T>& w, const Vec3<T>& v );

      inline void set( const Vec3<T>& a, const Vec3<T>& b, const Vec3<T>& c );

    private:
      Vec3<T> _u;
      Vec3<T> _v;
      Vec3<T> _w;
    };
  } /* math */

  // Typedefs.
  typedef cc::math::Onb<float>        Onbf;
  typedef cc::math::Onb<double>       Onbd;
  typedef cc::math::Onb<int>          Onbi;
  typedef cc::math::Onb<unsigned int> Onbui;

} /* cc */

#include "Onb.inl"

#endif /* __CC_MATH_ONB__ */
