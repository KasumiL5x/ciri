#ifndef __CC_MATH_COMMON__
#define	__CC_MATH_COMMON__

#include "Constants.hpp"
#include <cstdlib>

namespace cc {
  namespace math {

    /**
     * Fast inverse square root (1/sqrt X).
     */
   static float fastInvSqrt(float x) {
      float xhalf = 0.5f * x;
      int i = *(int*)&x;         // evil floating point bit level hacking
      i = 0x5f3759df - (i >> 1);  // what the fuck?
      x = *(float*)&i;
      x = x*(1.5f-(xhalf*x*x));
      return x;
    }

    /**
     * Converts degrees to radians.
     * @param[in] degrees The angle in degrees.
     * @return The angle in radians.
     */
    template<typename T>
    inline T degreesToRadians( T degrees ) {
      return degrees * DEG_TO_RAD;
    }

    /**
     * Converts radians to degrees.
     * @param[in] radians The angle in radians.
     * @return The angle in degrees.
     */
    template<typename T>
    inline T radiansToDegrees( T radians ) {
      return radians * RAD_TO_DEG;
    }

    /**
     * Clamps a value within a range.
     * @param[in] val The value to clamp.
     * @param[in] min Minimum value.
     * @param[in] max Maximum value.
     * @return The clamped value.
     */
    template<typename T>
    inline T clamp( T val, T min, T max ) {
      if( val < min ) {
        return min;
      }

      if( val > max ) {
        return max;
      }

      return val;
    }

    /**
     * Returns the minimum of two given values.
     * @param[in] a First value.
     * @param[in] b Second value.
     * @return Smaller of the two given values.
     */
    template<typename T>
    inline T minimum( T a, T b ) {
      return (a >= b) ? b : a;
    }

    /**
     * Returns the maximum of two given values.
     * @param[in] a First value.
     * @param[in] b Second value.
     * @return Larger of the two given values.
     */
    template<typename T>
    inline T maximum( T a, T b ) {
      return (a <= b) ? b : a;
    }

    /**
     * Clamps a value from zero to one.
     * @param[in] value Value to saturate.
     * @return Input value clamped zero to one.
     */
    template<typename T>
    inline T saturate( T value ) {
      return maximum<T>(0.0, minimum<T>(1.0, value));
    }

    /**
     * Linearly interpolates between two values.
     * @param[in] p0 Vvalue to lerp from.
     * @param[in] p1 Value to lerp to.
     * @param[in] s  Percentage to lerp.
     * @return Linearly interpolated value.
     */
    template<typename T>
    inline T lerp( T p0, T p1, float s ) {
      return p0 + (p1 - p0) * s;
    }

    /**
     * Interpolates using smoothstep.
     * @param[in] p0 Value to interpolate from.
     * @param[in] p1 Value to interpolate to.
     * @param[in] s  Percentage to lerp.
     * @return Smoothstepped value.
     */
    template<typename T>
    inline T smoothstep( T p0, T p1, float s ) {
      T t = saturate<T>((s-p0)/(p1-p0));
      return t*t*(static_cast<T>(3.0)-(static_cast<T>(2.0)*t));
    }

    /**
     * Computes the percent of a value between a range.
     * @param[in] min Minimum of the range.
     * @param[in] max Maximum of the range.
     * @param[in] val Value to compute percent of (should be within min..max).
     * @return Percent of val within the range min...max.
     */
    template<typename T>
    inline T percent( T min, T max, T val ) {
      return (val - min) / (max - min);
    }

    /**
     * Returns the percentage of a value.
     * @param[in] val     Value to compute a percentage of.
     * @param[in] percent Percentage of the value to return.
     * @return Percentage of the given value.
     */
    template<typename T>
    inline T percent( T val, float percent ) {
      return val * percent / static_cast<T>(100);
    }

    /**
     * Returns the sign of a value.
     * @param[in] val Value to compute the sign of.
     * @return Returns 1 if the value is positive; otherwise returns -1.
     */
    template<typename T>
    inline int sign( T val ) {
      return (val < static_cast<T>(0)) ? -1 : 1;
    }

    /**
     * TODO.
     * @param[in] val1    TODO.
     * @param[in] val2    TODO.
     * @param[in] val3    TODO.
     * @param[in] amount1 TODO.
     * @param[in] amount2 TODO.
     * @return TODO.
     */
    template<typename T>
    inline T barycentric( T val1, T val2, T val3, T amount1, T amount2 ) {
      return ((val1 + (amount1 * (val2 - val1))) + (amount2 * (val3 - val1)));
    }

    /**
     * TODO.
     * @param[in] val1   TODO.
     * @param[in] val2   TODO.
     * @param[in] val3   TODO.
     * @param[in] val4   TODO.
     * @param[in] amount TODO.
     * @return TODO.
     */
    template<typename T>
    inline T catmullRom( T val1, T val2, T val3, T val4, float amount ) {
      const T n  = amount * amount;
      const T n2 = amount * n;
      const T half  = static_cast<T>(0.5f);
      const T two   = static_cast<T>(2);
      const T three = static_cast<T>(3);
      const T four  = static_cast<T>(4);
      const T five  = static_cast<T>(5);
      return (half * ((((two * val2) + ((-val1 + val3) * amount)) + (((((two * val1) - (five * val2)) + (four * val3)) - val4) * n)) + ((((-val1 + (three * val2)) - (three * val3)) + val4) * n2)));
    }

    /**
     * TODO.
     * @param[in] val1   TODO.
     * @param[in] tan1   TODO.
     * @param[in] val2   TODO.
     * @param[in] tan2   TODO.
     * @param[in] amount TODO.
     * @return TODO.
     */
    template<typename T>
    inline T hermite( T val1, T tan1, T val2, T tan2, T amount ) {
      const T n3 = amount;
      const T n  = n3 * n3;
      const T n2 = n3 * n;
      const T n7 = ((static_cast<T>(2) * n2) - (static_cast<T>(3) * n)) + static_cast<T>(1);
      const T n6 = (static_cast<T>(-2) * n2) + (static_cast<T>(3) * n);
      const T n5 = (n2 - (static_cast<T>(2) * n)) + n3;
      const T n4 = n2 - n;
      return ((((val1 * n7) + (val2 * n6)) + (tan1 * n5)) + (tan2 * n4));
    }

    /**
     * Test the equality of two values.  Note that this does NOT use the == operator.  Numbers only.
     * @param[in] val1 First value.
     * @param[in] val2 Second value.
     * @return True if the values are equal; false otherwise.
     */
    template<typename T>
    inline bool equal( T val1, T val2 ) {
      const T diff = val1 - val2;
      return diff >= static_cast<T>(0) && diff <= static_cast<T>(EPSILON);
    }

    /**
     * Compute Young's modulus (tensile/elastic modulus).
     * @param[in] mass      Total mass of the object.
     * @param[in] gravity   Total effect of gravity on the object.
     * @param[in] area      Total area the object takes up.
     * @param[in] length    Total length of the object.
     * @param[in] extension Stretch amount of the object.
     * @return Computed modulus.
     */
    template<typename T>
    inline T youngs( T mass, T gravity, T area, T length, T extension ) {
      // Force
      const T F = mass * gravity;
      // Young's = F*L / (E*A)
      return (F * length) / (extension * area);
    }

    /**
     * Wraps the given angle in the given range.
     * @param[in] angle The angle to wrap.
     * @param[in] min   The lower-bound to wrap.
     * @param[in] max   The upper-bound to wrap.
     * @return Wrapped angle in the range [min, max].
    */
    template<typename T>
    inline T wrapAngle( T angle, T min, T max ) {
      return fmodf(fmodf(angle - min, max - min) + (max - min), max - min) + min;
    }
  } /* math */
} /* cc */

#endif	/* __CC_MATH_COMMON__ */

