#ifndef __shadows_BoundingBox__
#define __shadows_BoundingBox__

#include <vector>
#include <cc/Vec3.hpp>

class BoundingBox {
public:
	BoundingBox() {
	}
	BoundingBox( const cc::Vec3f& min, const cc::Vec3f& max ) {
		Min = min;
		Max = max;
	}

	static BoundingBox createFromPoints( const std::vector<cc::Vec3f>& points ) {
		bool empty = true;
		cc::Vec3f min(std::numeric_limits<float>::max());
		cc::Vec3f max(std::numeric_limits<float>::min());
		for( const auto& p : points ) {
			min.x = (min.x < p.x) ? min.x : p.x;
			min.y = (min.y < p.y) ? min.y : p.y;
			min.z = (min.z < p.z) ? min.z : p.z;
			max.x = (max.x > p.x) ? max.x : p.x;
			max.y = (max.y > p.y) ? max.y : p.y;
			max.z = (max.z > p.z) ? max.z : p.z;
			empty = false;
		}
		if( empty ) {
			throw;
		}
		return BoundingBox(min, max);
	}

public:
	cc::Vec3f Min;
	cc::Vec3f Max;
};

#endif