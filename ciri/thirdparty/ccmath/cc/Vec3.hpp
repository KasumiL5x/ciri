#ifndef __CC_MATH_VEC3__
#define	__CC_MATH_VEC3__

namespace cc {
  namespace math {
    template<typename T>
    class Vec3 {
    public:
      inline Vec3();
      inline Vec3( const Vec3<T>& vec );
      inline Vec3( const T& val);
      inline Vec3( const T& x, const T& y, const T& z );

      // Accessors.
      inline T&       operator[]( unsigned int index );
      inline const T& operator[]( unsigned int index ) const;
      inline T&       operator()( unsigned int index );
			inline const T& operator()( unsigned int index ) const;

      // Unary arithmetic operators.
      inline Vec3<T>& operator= ( const Vec3<T>& vec );
      inline Vec3<T>& operator+=( const Vec3<T>& vec );
      inline Vec3<T>& operator-=( const Vec3<T>& vec );
      inline Vec3<T>& operator*=( const Vec3<T>& vec );
      inline Vec3<T>& operator/=( const Vec3<T>& vec );

      // Binary arithmetic operators.
      // All defined in vec3.inl.

      // Unary constant operators.
      // All defined in vec3.inl.

      // Other things.
      inline T       magnitude   () const;
      inline T       sqrMagnitude() const;
      inline void    normalize   ();
      inline Vec3<T> normalized  () const;
      inline bool    equalTo     ( const Vec3<T>& rhs ) const;
      inline T       dot         ( const Vec3<T>& rhs ) const;
      inline Vec3<T> cross       ( const Vec3<T>& rhs ) const;
      inline T       distance    ( const Vec3<T>& rhs ) const;
      inline T       sqrDistance ( const Vec3<T>& rhs ) const;
      inline Vec3<T> minimum     ( const Vec3<T>& rhs ) const;
      inline Vec3<T> maximum     ( const Vec3<T>& rhs ) const;
      inline Vec3<T> lerp        ( const Vec3<T>& to, const float& t ) const;
      inline Vec3<T> reflect     ( const Vec3<T>& direction ); // Where this vector is the position.

      // Static stuff.
      inline static Vec3<T> zero    ();
      inline static Vec3<T> one     ();
      inline static Vec3<T> up      ();
      inline static Vec3<T> down    ();
      inline static Vec3<T> left    ();
      inline static Vec3<T> right   ();
      inline static Vec3<T> forward ();
      inline static Vec3<T> backward();

    public:
      union {
        struct {
          T x, y, z;
        };
        struct {
          T r, g, b;
        };
      };
    };
  } /* math */
  
  // Typedefs
  typedef cc::math::Vec3<float>        Vec3f;
  typedef cc::math::Vec3<double>       Vec3d;
  typedef cc::math::Vec3<int>          Vec3i;
  typedef cc::math::Vec3<unsigned int> Vec3ui;
  
} /* cc */

#include "Vec3.inl"

#endif	/* __CC_MATH_VEC3__ */

