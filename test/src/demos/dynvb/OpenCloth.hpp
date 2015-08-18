#ifndef __opencloth__
#define __opencloth__

#include <vector>
#include <cc/Vec3.hpp>
#include <ciri/gfx/IGraphicsDevice.hpp>
#include "../../common/Vertex.hpp"

class OpenCloth {
private:
	struct Spring {
		int p1;
		int p2;
		float restLength;
		float ks;
		float kd;
		int type;
	};

public:
	OpenCloth();
	~OpenCloth();

	void build( ciri::IGraphicsDevice* device );
	void update( float deltaTime );

	ciri::IVertexBuffer* getVertexBuffer() const;
	ciri::IIndexBuffer* getIndexBuffer() const;

private:
	void step( float deltaTime );
	void computeForces( float deltaTime );
	void integrateVerlet( float deltaTime );
	void provotDynamicInverse();
	void addSpring( int a, int b, float ks, float kd, int type );
	__forceinline cc::Vec3f getVerletVelocity( const cc::Vec3f& pos, const cc::Vec3f& lastPos, float deltaTime );
	void updateGpuBuffers();

private:
	ciri::IGraphicsDevice* _device;
	ciri::IVertexBuffer* _vertexBuffer;
	ciri::IIndexBuffer* _indexBuffer;
	std::vector<Vertex> _vertices;
	//
	int _numX;
	int _numY;
	int _totalPoints;
	int _size;
	float _hsize;
	std::vector<int> _indices;
	std::vector<cc::Vec3f> _positions;
	std::vector<cc::Vec3f> _lastPositions;
	std::vector<cc::Vec3f> _forces;
	std::vector<Spring> _springs;
	float _timeStep;
	float _timeAccumulator;
	cc::Vec3f _gravity;
	float _mass;
	float _damping;
};

#endif /* __opencloth__ */