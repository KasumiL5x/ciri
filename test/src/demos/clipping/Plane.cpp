#include "Plane.hpp"

bool Plane::fitPointsToPlane( const std::vector<cc::Vec3f>& points, float& residualError, Plane& outPlane ) {
	std::vector<cc::Vec4f> weightedPoints(points.size());
	for( unsigned int i = 0; i < static_cast<unsigned int>(points.size()); ++i ) {
		weightedPoints[i] = cc::Vec4f(points[i], 1.0f);
	}
	cc::Vec3f basis1;
	cc::Vec3f basis2;
	float normalEigenValue;
 	return fitPointsToPlane(weightedPoints, basis1, basis2, normalEigenValue, residualError, outPlane);
}

bool Plane::fitPointsToPlane( const std::vector<cc::Vec4f>& points, cc::Vec3f& basis1, cc::Vec3f& basis2, float& normalEigenValue, float& residualError, Plane& outPlane ) {
	// find centroid
	cc::Vec3f centroid(0.0f, 0.0f, 0.0f);
	float totalWeight = 0.0f;
	for( const auto& p : points ) {
		totalWeight += p.w;
		centroid += cc::Vec3f(p.x, p.y, p.z) * p.w;
	}
	centroid /= totalWeight;

	// compute scatter matrix
	float scatterMatrix[3][3];
	int order[3];
	findScatterMatrix(points, centroid, scatterMatrix, order);

	float diagonalMatrix[3];
	float offDiagonalMatrix[3];
	tred2(scatterMatrix, diagonalMatrix, offDiagonalMatrix);
	tqli(diagonalMatrix, offDiagonalMatrix, scatterMatrix);

	// find smallest eigenvalue first
	float min = diagonalMatrix[0];
	float max = diagonalMatrix[0];
	unsigned int minIndex = 0;
	unsigned int middleIndex = 0;
	unsigned int maxIndex = 0;
	for( unsigned int i = 1; i < 3; i++ ) {
		if( diagonalMatrix[i] < min ) {
			min = diagonalMatrix[i];
			minIndex = i;
		}
		if( diagonalMatrix[i] > max ) {
			max = diagonalMatrix[i];
			maxIndex = i;
		}
	}
	for( unsigned int i = 0; i < 3; i++ ) {
		if( minIndex != i && maxIndex != i ) {
			middleIndex = i;
		}
	}

	// the normal of the plane is the smallest eigenvector.
	cc::Vec3f normal;
	for( unsigned int i = 0; i < 3; i++ ) {
		normal[order[i]] = scatterMatrix[i][minIndex];
		basis1[order[i]] = scatterMatrix[i][middleIndex];
		basis2[order[i]] = scatterMatrix[i][maxIndex];
	}
	normalEigenValue = fabsf(diagonalMatrix[minIndex]);
	setVec3Length(basis1, diagonalMatrix[middleIndex]);
	setVec3Length(basis2, diagonalMatrix[maxIndex]);

	if( !isVec3Valid(basis1) || !isVec3Valid(basis2) || !isVec3Valid(normal) ) {
		outPlane = Plane::constructFromPointNormal(centroid, cc::Vec3f(1.0f, 0.0f, 0.0f));
		basis1 = cc::Vec3f(0.0f, 1.0f, 0.0f);
		basis2 = cc::Vec3f(0.0f, 0.0f, 1.0f);
	} else {
		outPlane = Plane::constructFromPointNormal(centroid, normal);
	}

	residualError = 0.0f;
	for( unsigned int i = 0; i < points.size(); i++ ) {
		residualError += outPlane.unsignedDistance(cc::Vec3f(points[i].x, points[i].y, points[i].z));
	}
	residualError /= points.size();

	return true;
}