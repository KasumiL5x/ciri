#ifndef __opencloth__
#define __opencloth__

#include <vector>
#include <cc/Vec3.hpp>
#include <cc/Mat4.hpp>
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
	};

public:
	_declspec(align(16))
	struct Constants {
		cc::Mat4f world;
		cc::Mat4f xform;
		cc::Vec3f camdir;
	};

public:
	OpenCloth();
	~OpenCloth();

	void setDivisions( int x, int y );
	void setSize( int size );
	void setMass( float mass );
	void setDamping( float damping );
	void setSpringParams( float structKs, float structKd, float shearKs, float shearKd, float bendKs, float bendKd );
	void setGravity( const cc::Vec3f& gravity );
	void build( std::shared_ptr<ciri::IGraphicsDevice> device );
	void update( float deltaTime );
	void clean();

	bool isBuilt() const;

	ciri::IVertexBuffer* getVertexBuffer() const;
	ciri::IIndexBuffer* getIndexBuffer() const;
	ciri::IShader* getShader() const;
	Constants& getConstants();
	bool updateConstants();

private:
	bool createGpuResources();
	void createGpuBuffers();
	void updateGpuVertexBuffer();
	void step( float deltaTime );
	void computeForces( float deltaTime );
	void integrateVerlet( float deltaTime );
	void provotDynamicInverse();
	void computeNormals();
	void addSpring( int* idx, int a, int b, float ks, float kd );
	__forceinline cc::Vec3f getVerletVelocity( const cc::Vec3f& pos, const cc::Vec3f& lastPos, float deltaTime );
	

private:
	bool _built;
	//
	std::shared_ptr<ciri::IGraphicsDevice> _device;
	ciri::IVertexBuffer* _vertexBuffer;
	ciri::IIndexBuffer* _indexBuffer;
	Vertex* _vertices;
	int _indexCount;
	int* _indices;
	//
	int _divsX; // number of divisions in the x axis
	int _divsY; // number of divisions in the y axis
	int _totalPoints; // total number of points in the simulation
	int _size; // scaling value for the points' positions
	float _horizontalSize; // horizontal scaling value for the x coordinate of the points
	cc::Vec3f* _positions; // current positions of all points
	cc::Vec3f* _lastPositions; // previous positions of all points
	cc::Vec3f* _forces; // forces acting upon each point
	int _springCount;
	Spring* _springs; // all springs (structure, shear, bend)
	float _timeStep; // timestep to run the simulation at
	float _timeAccumulator; // accumulator for knowing when to update
	cc::Vec3f _gravity; // vvvvv
	float _mass; // mass assigned to each particle (used for forces and integration)
	float _invMass; // 1/mass for optimization
	float _damping; // damping used to damp velocity
	float _ksStruct; // ks term for structural springs
	float _kdStruct; // kd term for structural springs
	float _ksShear; // ks term for shear springs
	float _kdShear; // kd term for shear springs
	float _ksBend; // ks term for bend springs
	float _kdBend; // kd term for bend springs
	//
	ciri::IShader* _shader;
	Constants _constants;
	ciri::IConstantBuffer* _constantsBuffer;
};

#endif /* __opencloth__ */