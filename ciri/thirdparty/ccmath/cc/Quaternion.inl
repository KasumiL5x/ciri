#include <iostream>

namespace cc {
  namespace math {
    template<typename T>
    inline Quaternion<T>::Quaternion() 
      : x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0)), w(static_cast<T>(1)) {
    }

    template<typename T>
    inline Quaternion<T>::Quaternion( const T& xx, const T& yy, const T& zz )
      : x(xx), y(yy), z(zz), w(static_cast<T>(1)) {
    }

    template<typename T>
    inline Quaternion<T>::Quaternion( const T& xx, const T& yy, const T& zz, const T& ww )
      : x(xx), y(yy), z(zz), w(ww) {
    }

    template<typename T>
    inline Quaternion<T>::Quaternion( const Vec3<T>& vec )
      : x(vec.x), y(vec.y), z(vec.z), w(static_cast<T>(1)) {
    }

    template<typename T>
    inline Quaternion<T>::Quaternion( const Vec3<T>& vec, const T& ww )
      : x(vec.x), y(vec.y), z(vec.z), w(ww) {
    }

    template<typename T>
    inline T Quaternion<T>::scalar() const {
      return w;
    }

    template<typename T>
    inline Vec3<T> Quaternion<T>::vector() const {
      return Vec3<T>(x, y, z);
    }

    template<typename T>
    inline T Quaternion<T>::length() const {
      return sqrt(w*w + x*x + y*y + z*z);
    }

    template<typename T>
    inline Quaternion<T> Quaternion<T>::conjugate() const {
      return Quaternion(-x, -y, -z, w);
    }

    template<typename T>
    inline T Quaternion<T>::angle() const {
      return static_cast<T>(2) * static_cast<T>(acos(w));
    }

    template<typename T>
    inline Vec3<T> Quaternion<T>::axis() const {
      Vec3<T> v(x, y, z);
      const T m = v.magnitude();
      return (m <= static_cast<T>(EPSILON)) ? Vec3<T>() : v/m;
    }

    template<typename T>
    inline Quaternion<T> Quaternion<T>::rotate( const Quaternion<T>& rhs ) const {
      return *this * rhs * this->conjugate();
    }

    template<typename T>
    inline Vec3<T> Quaternion<T>::rotate( const Vec3<T>& vec ) const {
      const Quaternion<T> t = *this * vec * this->conjugate();
      return t.vector();
    }

    template<typename T>
    inline void Quaternion<T>::normalize() {
      T d = w*w + x*x + y*y + z*z;
      // If a zero-length Quaternion, set to identity and return.
      if( equal<T>(d, static_cast<T>(0)) )
      {
        w = static_cast<T>(1);
        return;
      }
      d = (static_cast<T>(1) / static_cast<T>(sqrt(d)));
      w *= d;
      x *= d;
      y *= d;
      z *= d;
    }

    template<typename T>
    inline Quaternion<T> Quaternion<T>::normalized() const {
      T d = w*w + x*x + y*y + z*z;
      // If a zero-length Quaternion, return identity.
      if( equal<T>(d, static_cast<T>(0)) )
      {
        return Quaternion();
      }
      d = (static_cast<T>(1) / static_cast<T>(sqrt(d)));
    
      return Quaternion(x*d, y*d, z*d, w*d);
    }

    template<typename T>
    Quaternion<T> Quaternion<T>::createFromEulerAngles( const T& x, const T& y, const T& z ) {
      const T roll  = degreesToRadians<T>(x);
      const T pitch = degreesToRadians<T>(y);
      const T yaw   = degreesToRadians<T>(z);
    
      const T cYaw   = static_cast<T>(cos(static_cast<T>(0.5) * yaw));
      const T cPitch = static_cast<T>(cos(static_cast<T>(0.5) * pitch));
      const T cRoll  = static_cast<T>(cos(static_cast<T>(0.5) * roll));
      const T sYaw   = static_cast<T>(sin(static_cast<T>(0.5) * yaw));
      const T sPitch = static_cast<T>(sin(static_cast<T>(0.5) * pitch));
      const T sRoll  = static_cast<T>(sin(static_cast<T>(0.5) * roll));
    
      const T cYawCPitch = cYaw * cPitch;
      const T sYawsPitch = sYaw * sPitch;
      const T cYawSPitch = cYaw * sPitch;
      const T sYawCPitch = sYaw * cPitch;
    
      Quaternion q;
      q.w = cYawCPitch * cRoll + sYawsPitch * sRoll;
      q.x = cYawCPitch * sRoll - sYawsPitch * cRoll;
      q.y = cYawSPitch * cRoll + sYawCPitch * sRoll;
      q.z = sYawCPitch * cRoll - cYawSPitch * sRoll;
      return q;
    }

    template<typename T>
    inline void Quaternion<T>::addScaledVector( const Vec3<T>& vec, const float scale ) {
      Quaternion<T> q(vec.x*scale, vec.y*scale, vec.z*scale, static_cast<T>(0));
      q = q * *this;
    
      w += q.w * static_cast<T>(0.5);
      x += q.x * static_cast<T>(0.5);
      y += q.y * static_cast<T>(0.5);
      z += q.z * static_cast<T>(0.5);
    }

    template<typename T>
    inline Vec3<T> Quaternion<T>::rotateVector( const Vec3<T>& vec ) const {
      Mat4<T> mat;
      mat[0][0] = static_cast<T>(1) - (static_cast<T>(2) * (this->y*this->y)) - (static_cast<T>(2) * (this->z*this->z));
      mat[0][1] = static_cast<T>(2) * (this->x*this->y + this->w*this->z);
      mat[0][2] = static_cast<T>(2) * (this->x*this->z + this->w*this->y);

      mat[1][0] = static_cast<T>(2) * (this->x*this->y + this->w*this->z);
      mat[1][1] = static_cast<T>(1) - (static_cast<T>(2) * (this->x*this->x)) - (static_cast<T>(2) * (this->z*this->z));
      mat[1][2] = static_cast<T>(2) * (this->y*this->z + this->w*this->x);

      mat[2][0] = static_cast<T>(2) * (this->x*this->z + this->w*this->y);
      mat[2][1] = static_cast<T>(2) * (this->y*this->z + this->w*this->x);
      mat[2][2] = static_cast<T>(1) - (static_cast<T>(2) * (this->x*this->x)) - (static_cast<T>(2) * (this->y*this->y));
      const Vec4<T> tmpVec = mat * Vec4<T>(vec.x, vec.y, vec.z, static_cast<T>(1));
      return Vec3<T>(tmpVec.x, tmpVec.y, tmpVec.z);
    }

    template<typename T>
    inline Quaternion<T> Quaternion<T>::inverse() const {
      return this->conjugate() / this->dot(*this);
    }

    template<typename T>
    inline T Quaternion<T>::dot( const Quaternion<T>& rhs ) const {
      return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z + this->w * rhs.w;
    }

    template<typename T>
    Quaternion<T> Quaternion<T>::angleAxis( const Vec3<T>& axis, const T& angle ) {
      const T s = static_cast<T>(sin(angle * static_cast<T>(0.5)));
      Quaternion<T> q;
      q.w = static_cast<T>(cos(angle * static_cast<T>(0.5)));
      q.x = axis.x * s;
      q.y = axis.y * s;
      q.z = axis.z * s;
      return q;
    }

    template<typename T>
    Vec3<T> Quaternion<T>::createEulerAngles( const Quaternion<T>& q ) {
      const T q00 = q.w * q.w;
      const T q11 = q.x * q.x;
      const T q22 = q.y * q.y;
      const T q33 = q.z * q.z;
    
      const T r11 = q00 + q11 - q22 - q33;
      const T r21 = static_cast<T>(2) * (q.x*q.y + q.w*q.z);
      const T r31 = static_cast<T>(2) * (q.x*q.z - q.w*q.y);
      const T r32 = static_cast<T>(2) * (q.y*q.z + q.w*q.x);
      const T r33 = q00 - q11 - q22 + q33;
    
      const T tmp = static_cast<T>(fabs(r31));
      if( tmp > static_cast<T>(0.999999) )
      {
        const T r12 = static_cast<T>(2) * (q.x*q.y - q.w*q.z);
        const T r13 = static_cast<T>(2) * (q.x*q.z + q.w*q.y);
      
        Vec3<T> v;
        v.x = radiansToDegrees<T>(0.0);
        v.y = radiansToDegrees<T>(-(static_cast<T>(PI/2.0)) * r31/tmp);
        v.z = radiansToDegrees<T>(atan2(-r12, (-r31*r13)));
        return v;
      }
    
      Vec3<T> v1;
      v1.x = radiansToDegrees<T>(atan2(r32, r33));
      v1.y = radiansToDegrees<T>(asin(-r31));
      v1.z = radiansToDegrees<T>(atan2(r21, r11));
      return v1;
    }

    template<typename T>
    Quaternion<T> Quaternion<T>::createFromMatrix( const Mat4<T>& m ) {
      const T w  = static_cast<T>(sqrt((static_cast<T>(1) + m[0][0] + m[1][1] + m[2][2])) / static_cast<T>(2));
      const T w4 = (static_cast<T>(4) * w);
    
      Quaternion<T> q;
      q.w = w;
      q.x = (m[2][1] - m[1][2]) / w4;
      q.y = (m[0][2] - m[2][0]) / w4;
      q.z = (m[1][0] - m[0][1]) / w4;
      return q;
    }

    template<typename T>
    Mat4<T> Quaternion<T>::createMatrixFromQuaternion( const Quaternion<T>& q ) {
      const T r = q.w;
      const T x = q.x;
      const T y = q.y;
      const T z = q.z;
      Mat4<T> c;
      c[0][0] = static_cast<T>(1) - static_cast<T>(2)*y*y - static_cast<T>(2)*z*z;
      c[0][1] = static_cast<T>(2)*x*y - static_cast<T>(2)*r*z;
      c[0][2] = static_cast<T>(2)*x*z + static_cast<T>(2)*r*y;
    
      c[1][0] = static_cast<T>(2)*x*y + static_cast<T>(2)*r*z;
      c[1][1] = static_cast<T>(1) - static_cast<T>(2)*x*x - static_cast<T>(2)*z*z;
      c[1][2] = static_cast<T>(2)*y*z - static_cast<T>(2)*r*x;
    
      c[2][0] = static_cast<T>(2)*x*z - static_cast<T>(2)*r*y;
      c[2][1] = static_cast<T>(2)*y*z + static_cast<T>(2)*r*x;
      c[2][2] = static_cast<T>(1) - static_cast<T>(2)*x*x - static_cast<T>(2)*y*y;
    
      return c;
    }

    // Binary arithmetic operators.
    template<typename T>
    inline Quaternion<T> operator+( const Quaternion<T>& lhs, const Quaternion<T>& rhs ) {
      return Quaternion<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
    }
    template<typename T>
    inline Quaternion<T> operator-( const Quaternion<T>& lhs, const Quaternion<T>& rhs ) {
      return Quaternion<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
    }
    template<typename T>
    inline Quaternion<T> operator*( const Quaternion<T>& lhs, const T& rhs ) {
      return Quaternion<T>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
    }
    template<typename T>
    inline Quaternion<T> operator*( const Quaternion<T>& lhs, const Quaternion<T>& rhs ) {
      return Quaternion<T>(lhs.w*rhs.x + lhs.x*rhs.w + lhs.y*rhs.z - lhs.z*rhs.y,
                           lhs.w*rhs.y + lhs.y*rhs.w + lhs.z*rhs.x - lhs.x*rhs.z,
                           lhs.w*rhs.z + lhs.z*rhs.w + lhs.x*rhs.y - lhs.y*rhs.x,
                           lhs.w*rhs.w - lhs.x*rhs.x - lhs.y*rhs.y - lhs.z*rhs.z);
    }
    template<typename T>
    inline Quaternion<T> operator/( const Quaternion<T>& lhs, const T& rhs ) {
      return Quaternion<T>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
    }
    template<typename T>
    inline Vec3<T> operator*( const Quaternion<T>& q, const Vec3<T>& v ) {
		  const T two = static_cast<T>(2);

		  Vec3<T> uv, uuv;
		  Vec3<T> QuatVector(q.x, q.y, q.z);
		  uv = QuatVector.cross(v);
		  uuv = QuatVector.cross(uv);
		  uv *= (two * q.w); 
		  uuv *= two; 

		  return v + uv + uuv;
    }

    template<typename T>
    inline Vec3<T> operator*( const Vec3<T>& v, const Quaternion<T>& q ) {
      return q.inverse() * v;
    }

    template<typename T>
    inline std::ostream& operator<<( std::ostream& os, const Quaternion<T>& q ) {
      os << "x[" << q.x << "], y[" << q.y << "], z[" << q.z << "], w[" << q.w << "]" << std::endl;
      return os;
    }
  } /* math */
} /* cc */
