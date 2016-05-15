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
