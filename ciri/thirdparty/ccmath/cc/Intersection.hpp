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

#ifndef __CC_MATH_INTERSECTION__
#define	__CC_MATH_INTERSECTION__

#include <vector>
#include "Vec3.hpp"

namespace cc {
  namespace math {
    /**
     * Test if a point is inside a triangle.
     * @param[in] p  Point to test.
     * @param[in] t0 First vertex of the triangle.
     * @param[in] t1 Second vertex of the triangle.
     * @param[in] t2 Third vertex of the triangle.
     * @return True if the point is inside the triangle; false otherwise.
     */
    template<typename T>
    inline bool pointInTriangle( const Vec3<T>& p, const Vec3<T>& t0, const Vec3<T>& t1, const Vec3<T>& t2 ) {
      const Vec3<T> u = t1 - t0;
      const Vec3<T> v = t2 - t0;
      const Vec3<T> w = p - t0;

      const Vec3<T> vw = v.cross(w);
      const Vec3<T> vu = v.cross(u);
      if( vw.dot(vu) < static_cast<T>(0) ) {
        return false;
      }

      const Vec3<T> uw = u.cross(w);
      const Vec3<T> uv = u.cross(v);
      if( uw.dot(uv) < static_cast<T>(0) ) {
        return false;
      }

      const T denom = uv.magnitude();
      const T r = vw.magnitude() / denom;
      const T t = uw.magnitude() / denom;

      return (r <= static_cast<T>(1) && t <= static_cast<T>(1) && r + t <= static_cast<T>(1));
    }
    
    /**
     * Test if a sphere is intersecting a plane.
     * @param[in] spherePos    Position of the sphere.
     * @param[in] sphereRadius Radius of the sphere.
     * @param[in] planePos     A point on the plane.
     * @param[in] planeNormal  Normal of the plane.
     * @return True if the sphere is intersecting the plane; false otherwise.
     */
    template<typename T>
    inline bool sphereInPlane( const Vec3<T>& spherePos, const T& sphereRadius, const Vec3<T>& planePos, const Vec3<T>& planeNormal ) {
      // Vector from point on plane to center of the sphere.
      const Vec3<T> planeToSphere = spherePos - planePos;
      // Distance between the two (DP of the above vector w/ the plane normal).
      const T dist = planeToSphere.dot(planeNormal);
      // If dist <= radius, it's colliding.
      return (dist <= sphereRadius);
    }
    
    // Test if a sphere is behind (not intersecting) a plane.
    /**
     * Test if a sphere is behind a plane.
     * @param[in] spherePos    Position of the sphere.
     * @param[in] sphereRadius Radius of the sphere.
     * @param[in] planePos     A point on the plane.
     * @param[in] planeNormal  Normal of the plane.
     * @return True if the sphere is behind the plane; false otherwise.
     */
    template<typename T>
    inline bool sphereBehindPlane( const Vec3<T>& spherePos, const T& sphereRadius, const Vec3<T>& planePos, const Vec3<T>& planeNormal ) {
      // Vector from point onk plane to center of the sphere.
      const Vec3<T> planeToSphere = spherePos - planePos;
      // Distance between the two (DP of the above vector w/ the plane normal).
      const T dist = planeToSphere.dot(planeNormal);
      // Behind if the distance is less than the negative radius.
      return dist < -sphereRadius;
    }
    
    // Find the indices of the most separated points on an AABB of a vector of points.
    /**
     * Find the indices of the most separated points on an AABB constructed by a list of points.
     * @param[in]  points    Vector of points.
     * @param[out] outMinIdx Index of the minimum extents in the vector along the picked axis.
     * @param[out] outMaxIdx Index of the maximum extents in the vector along the picked axis.
     */
    template<typename T>
    inline void mostSeparatedPointsOnAabb( const std::vector<Vec3<T> >& points, int* outMinIdx, int* outMaxIdx ) {
      // Safety check.
      if( outMinIdx == nullptr || outMaxIdx == nullptr ) {
        return;
      }
      
      // Find the most extreme points along three major axis (X,Y,Z).
      int minX = 0; int maxX = 0;
      int minY = 0; int maxY = 0;
      int minZ = 0; int maxZ = 0;
      for( unsigned int i = 1; i < points.size(); ++i ) // Start at 1 'cause zero is the default for all of the values.
      {
        // Current point.
        const Vec3<T>* const pt = &points[i];

        // Update min/max.
        minX = (pt->x < points[minX].x) ? i : minX;
        maxX = (pt->x > points[maxX].x) ? i : maxX;
        minY = (pt->y < points[minY].y) ? i : minY;
        maxY = (pt->y > points[maxY].y) ? i : maxY;
        minZ = (pt->z < points[minZ].z) ? i : minZ;
        maxZ = (pt->z > points[maxZ].z) ? i : maxZ;
      }

      // Compute squared distances for the three pairs of points.
      const float dist2x = (points[maxX] - points[minX]).dot(points[maxX] - points[minX]);
      const float dist2y = (points[maxY] - points[minY]).dot(points[maxY] - points[minY]);
      const float dist2z = (points[maxZ] - points[minZ]).dot(points[maxZ] - points[minZ]);

      // Pick the pair that are the most distant.
      *outMinIdx = minX;
      *outMaxIdx = maxX;
      if( dist2y > dist2x && dist2y > dist2z )
      {
        *outMinIdx = minY;
        *outMaxIdx = maxY;
      }
      if( dist2z > dist2x && dist2z > dist2y )
      {
        *outMinIdx = minZ;
        *outMaxIdx = maxZ;
      }
    }
    
    /**
     * Create a sphere from a vector of points.
     * @param[in]  points          Vector of points.
     * @param[out] outSpherePos    Position of the center of the resulting sphere.
     * @param[out] outSphereRadius Radius of the resulting sphere.
     */
    template<typename T>
    inline void createSphereFromPoints( const std::vector<Vec3<T> >& points, Vec3<T>* outSpherePos, T* outSphereRadius ) {
      // Safety check.
      if( outSpherePos == nullptr || outSphereRadius == nullptr ) {
        return;
      }
      
      // Find the most separated points on the encompasing AABB of the points.
      int min = -1; int max = -1;
      mostSeparatedPointsOnAabb<T>(points, &min, &max);

      // Compute the center point and the radius.
      *outSpherePos = (points[min] + points[max]) * 0.5f;
      *outSphereRadius = (points[max] - *outSpherePos).dot(points[max] - *outSpherePos);
      *outSphereRadius = sqrt(*outSphereRadius);
    }
    
    // Test if a point is in a sphere.
    /**
     * Test if a point is in a sphere.
     * @param[in]  point          Point to test.
     * @param[in]  spherePos      Position of the sphere.
     * @param[in]  sphereRadius   Radius of the sphere.
     * @param[out] outPenetration Distance of the point from the surface of the sphere.
     * @return True if the point is inside the sphere; false otherwise.
     */
    template<typename T>
    inline bool pointInSphere( const Vec3<T>& point, const Vec3<T>& spherePos, const T& sphereRadius, T* outPenetration ) {
      const T mag = (spherePos - point).magnitude();
      
      const bool intersects = mag <= sphereRadius;
      
      if( outPenetration != nullptr ) {
        *outPenetration = mag - sphereRadius;
      }
      
      return intersects;
    }
  } /* math */
} /* cc */

#endif	/* __CC_MATH_INTERSECTION__ */

