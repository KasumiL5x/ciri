#ifndef __shadows_BoundingFrustum__
#define __shadows_BoundingFrustum__

#include <array>
#include <cc/Mat4.hpp>
#include <cc/Vec3.hpp>
#include <cc/Vec4.hpp>

class BoundingFrustum {
private:
	struct Plane {
		cc::Vec3f normal;
		float d;

		Plane() {
		}

		Plane( const cc::Vec3f& v1, const cc::Vec3f& v2, const cc::Vec3f& v3 ) {
			const cc::Vec3f aux1 = v1 - v2;
			const cc::Vec3f aux2 = v3 - v2;
			normal = (aux2.cross(aux1)).normalized();
			d = -(normal.dot(v2));
		}

		Plane( const cc::Vec3f& nrm, float dd ) {
			normal = nrm;
			d = dd;
		}

		Plane( const cc::Vec4f& vec ) {
			normal = vec.truncated();
			d = vec.w;
		}

		Plane( const Plane& rhs ) {
			normal = rhs.normal;
			d = rhs.d;
		}
		Plane& operator=( const Plane& rhs ) {
			normal = rhs.normal;
			d = rhs.d;
			return *this;
		}
	};

public:
	BoundingFrustum( const cc::Mat4f& val ) {
		_matrix = val;
		createPlanes();
		createCorners();
	}
	BoundingFrustum( float angle, float ratio, float nearD, float farD, const cc::Vec3f& p, const cc::Vec3f& l, const cc::Vec3f& u ) {
		const float tang = tanf(cc::math::DEG_TO_RAD * angle * 0.5f);
		const float nh = nearD * tang;
		const float nw = nh * ratio;
		const float fh = farD * tang;
		const float fw = fh * ratio;

		cc::Vec3f dir, nc, fc, X, Y, Z;
		Z = p - l;
		Z.normalize();
		X = u.cross(Z);
		X.normalize();
		Y = Z.cross(X);
		nc = p - Z * nearD;
		fc = p - Z * farD;
		const auto ntl = nc + Y * nh - X * nw;
		const auto ntr = nc + Y * nh + X * nw;
		const auto nbl = nc - Y * nh - X * nw;
		const auto nbr = nc - Y * nh + X * nw;
		const auto ftl = fc + Y * fh - X * fw;
		const auto ftr = fc + Y * fh + X * fw;
		const auto fbl = fc - Y * fh - X * fw;
		const auto fbr = fc - Y * fh + X * fw;

		_planes[0] = Plane(ntr,ntl,ftl);
		_planes[1] = Plane(nbl,nbr,fbr);
		_planes[2] = Plane(ntl,nbl,fbl);
		_planes[3] = Plane(nbr,ntr,fbr);
		_planes[4] = Plane(ntl,ntr,nbr);
		_planes[5] = Plane(ftr,ftl,fbl);

		_corners[0] = ntl;
		_corners[1] = ntr;
		_corners[2] = nbl;
		_corners[3] = nbr;
		_corners[4] = ftl;
		_corners[5] = ftr;
		_corners[6] = fbl;
		_corners[7] = fbr;
	}

	const std::array<cc::Vec3f, 8>& corners() const {
		return _corners;
	}

	const std::array<Plane, 6>& planes() const {
		return _planes;
	}

private:
	void createPlanes() {
		// i think these are wrong :P
		//const float M13 = _matrix[0][2];
		//const float M23 = _matrix[1][2];
		//const float M33 = _matrix[2][2];
		//const float M43 = _matrix[3][2];
		//const float M14 = _matrix[0][3];
		//const float M24 = _matrix[1][3];
		//const float M34 = _matrix[2][3];
		//const float M44 = _matrix[3][3];
		//const float M11 = _matrix[0][0];
		//const float M21 = _matrix[1][0];
		//const float M31 = _matrix[2][0];
		//const float M41 = _matrix[3][0];
		//const float M12 = _matrix[0][1];
		//const float M22 = _matrix[1][1];
		//const float M32 = _matrix[2][1];
		//const float M42 = _matrix[3][1];
		////_planes[0] = Plane(cc::Vec3f(-M13, -M23, -M33), -M43);
		////_planes[1] = Plane(cc::Vec3f(M13 - M14, M23 - M24, M33 - M34), M43 - M44);
		////_planes[2] = Plane(cc::Vec3f(-M14 - M11, -M24 - M21, -M34 - M31), -M44 - M41);
		////_planes[3] = Plane(cc::Vec3f(M11 - M14, M21 - M24, M31 - M34), M41 - M44);
		////_planes[4] = Plane(cc::Vec3f(M12 - M14, M22 - M24, M32 - M34), M42 - M44);
		////_planes[5] = Plane(cc::Vec3f(-M14 - M12, -M24 - M22, -M34 - M32), -M44 - M42);

		//// left
		//_planes[0].normal.x = _matrix[0][3] + _matrix[0][0];
		//_planes[0].normal.y = _matrix[1][3] + _matrix[1][0];
		//_planes[0].normal.z = _matrix[2][3] + _matrix[2][0];
		//_planes[0].d        = _matrix[3][3] + _matrix[3][0];
		//// right
		//_planes[1].normal.x = _matrix[0][3] - _matrix[0][0];
		//_planes[1].normal.y = _matrix[1][3] - _matrix[1][0];
		//_planes[1].normal.z = _matrix[2][3] - _matrix[2][0];
		//_planes[1].d        = _matrix[3][3] - _matrix[3][0];
		//// bottom
		//_planes[2].normal.x = _matrix[0][3] + _matrix[0][1];
		//_planes[2].normal.y = _matrix[1][3] + _matrix[1][1];
		//_planes[2].normal.z = _matrix[2][3] + _matrix[2][1];
		//_planes[2].d        = _matrix[3][3] + _matrix[3][1];
		//// top
		//_planes[3].normal.x = _matrix[0][3] - _matrix[0][1];
		//_planes[3].normal.y = _matrix[1][3] - _matrix[1][1];
		//_planes[3].normal.z = _matrix[2][3] - _matrix[2][1];
		//_planes[3].d        = _matrix[3][3] - _matrix[3][1];
		//// near
		//_planes[4].normal.x = _matrix[0][3] + _matrix[0][2];
		//_planes[4].normal.y = _matrix[1][3] + _matrix[1][2];
		//_planes[4].normal.z = _matrix[2][3] + _matrix[2][2];
		//_planes[4].d        = _matrix[3][3] + _matrix[3][2];
		//// far
		//_planes[5].normal.x = _matrix[0][3] - _matrix[0][2];
		//_planes[5].normal.y = _matrix[1][3] - _matrix[1][2];
		//_planes[5].normal.z = _matrix[2][3] - _matrix[2][2];
		//_planes[5].d        = _matrix[3][3] - _matrix[3][2];

		//const float M11 = _matrix(0, 0);
		//const float M12 = _matrix(1, 0);
		//const float M13 = _matrix(2, 0);
		//const float M14 = _matrix(3, 0);
		//const float M21 = _matrix(0, 1);
		//const float M22 = _matrix(1, 1);
		//const float M23 = _matrix(2, 1);
		//const float M24 = _matrix(3, 1);
		//const float M31 = _matrix(0, 2);
		//const float M32 = _matrix(1, 2);
		//const float M33 = _matrix(2, 2);
		//const float M34 = _matrix(3, 2);
		//const float M41 = _matrix(0, 3);
		//const float M42 = _matrix(1, 3);
		//const float M43 = _matrix(2, 3);
		//const float M44 = _matrix(3, 3);
		//_planes[0] = Plane(cc::Vec3f(M41+M11, M42+M12, M43+M13), M44+M14); // left
		//_planes[1] = Plane(cc::Vec3f(M41-M11, M42-M12, M43-M13), M44-M14); // right
		//_planes[2] = Plane(cc::Vec3f(M41+M21, M42+M22, M43+M23), M44+M24); // bottom
		//_planes[3] = Plane(cc::Vec3f(M41-M21, M42-M22, M43-M23), M44-M24); // top
		//_planes[4] = Plane(cc::Vec3f(M41+M31, M42+M32, M43+M33), M44+M34); // near
		//_planes[5] = Plane(cc::Vec3f(M41-M31, M42-M32, M43-M33), M44-M34); // far

		const auto& row1 = cc::Vec4f(_matrix(0, 0), _matrix(1, 0), _matrix(2, 0), _matrix(3, 0));
		const auto& row2 = cc::Vec4f(_matrix(1, 0), _matrix(1, 1), _matrix(1, 2), _matrix(1, 3));
		const auto& row3 = cc::Vec4f(_matrix(2, 0), _matrix(2, 1), _matrix(2, 2), _matrix(2, 3));
		const auto& row4 = cc::Vec4f(_matrix(3, 0), _matrix(3, 1), _matrix(3, 2), _matrix(3, 3));
		_planes[0] = Plane(row4 + row1);
		_planes[1] = Plane(row4 - row1);
		_planes[2] = Plane(row4 + row2);
		_planes[3] = Plane(row4 - row2);
		_planes[4] = Plane(row4 + row3);
		_planes[5] = Plane(row4 - row3);


		normalizePlane(_planes[0]);
		normalizePlane(_planes[1]);
		normalizePlane(_planes[2]);
		normalizePlane(_planes[3]);
		normalizePlane(_planes[4]);
		normalizePlane(_planes[5]);
	}

	void createCorners() {
		intersectionPoint(_planes[0], _planes[2], _planes[4], _corners[0]);
		intersectionPoint(_planes[0], _planes[3], _planes[4], _corners[1]);
		intersectionPoint(_planes[0], _planes[3], _planes[5], _corners[2]);
		intersectionPoint(_planes[0], _planes[2], _planes[5], _corners[3]);
		intersectionPoint(_planes[1], _planes[2], _planes[4], _corners[4]);
		intersectionPoint(_planes[1], _planes[3], _planes[4], _corners[5]);
		intersectionPoint(_planes[1], _planes[3], _planes[5], _corners[6]);
		intersectionPoint(_planes[1], _planes[2], _planes[5], _corners[7]);
	}

	void normalizePlane( Plane& p ) {
		const float factor = 1.0f / p.normal.magnitude();
    p.normal.x *= factor;
    p.normal.y *= factor;
    p.normal.z *= factor;
    p.d *= factor;
	}

	void intersectionPoint( const Plane& p1, const Plane& p2, const Plane& p3, cc::Vec3f& result ) {
		// real-time collision detection pp. 213
		const cc::Vec3f u = p2.normal.cross(p3.normal);
		const float denom = p1.normal.dot(u);
		if( fabsf(denom) < 0.0001f ) {
			return;
			//throw; // planes don't intersect in a point
		}
		result = (p1.d * u + p1.normal.cross(p3.d * p2.normal - p2.d * p3.normal)) / denom;
	}

private:
	cc::Mat4f _matrix;
	std::array<cc::Vec3f, 8> _corners;
	std::array<Plane, 6> _planes;
};

#endif