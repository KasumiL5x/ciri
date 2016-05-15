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
			inline const T& operator()( unsigned int index ) const;

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
			inline T       sqrDistance ( const Vec2<T>& rhs ) const;
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

