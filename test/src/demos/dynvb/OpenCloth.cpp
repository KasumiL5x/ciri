#include "OpenCloth.hpp"

OpenCloth::OpenCloth()
	: _device(nullptr), _vertexBuffer(nullptr), _indexBuffer(nullptr),
		_numX(20), _numY(20), _totalPoints(0), _size(7), _hsize(0.0f) {
	_totalPoints = (_numX+1) * (_numY+1);
	_hsize = (float)_size * 0.5f;
	_timeStep = 1.0f / 60.0f;
	_timeAccumulator = _timeStep;
	_gravity = cc::Vec3f(0.0f, -9.81f ,0.0f);
	_mass = 1.0f;
	_damping = -0.0125f;
}

OpenCloth::~OpenCloth() {
}

void OpenCloth::build( ciri::IGraphicsDevice* device ) {
	_device = device;

	_indices.resize(_numX * _numY * 2 * 3);
	_positions.resize(_totalPoints);
	_lastPositions.resize(_totalPoints);
	_forces.resize(_totalPoints);

	// fill in positions
	const int u = _numX+1;
	const int v = _numY+1;
	int count = 0;
	for( int j = 0; j <= _numY; ++j ) {
		for( int i = 0; i <= _numX; ++i ) {
			const float x = ((float(i) / (u-1)) * 2.0f - 1.0f) * _hsize;
			const float y = 0.0f;//_size+1;
			const float z = ((float(j) / (v-1)) * _size);
			_positions[count] = cc::Vec3f(x, y, z);
			_lastPositions[count] = _positions[count];
			count++;
		}
	}

	// fill in indices
	int* id = &_indices[0];
	for( int i = 0; i < _numY; ++i ) {
		for( int j = 0; j < _numX; ++j ) {
			const int i0 = i * (_numX+1) + j;
			const int i1 = i0 + 1;
			const int i2 = i0 + (_numX+1);
			const int i3 = i2 + 1;
			if( (j+2) % 2 ) {
				*id++= i0; *id++= i2; *id++= i1;
				*id++= i1; *id++= i2; *id++= i3;
			} else {
				*id++= i0; *id++= i2; *id++= i3;
				*id++= i0; *id++= i3; *id++= i1;
			}
		}
	}

	// spring types
	const int STRUCT_SPRING = 0;
	const int SHEAR_SPRING = 1;
	const int BEND_SPRING = 2;

	// ks and kd for spring types
	const float KS_STRUCT = 500.75f; // 50...
	const float KD_STRUCT = -0.25f;
	const float KS_SHEAR = 500.75f; // 50...
	const float KD_SHEAR = -0.25f;
	const float KS_BEND = 500.95f; // 50...
	const float KD_BEND = -0.25f;

	// horizontal structure springs
	for( int l1 = 0; l1 < v; ++l1 ) {
		for( int l2 = 0; l2 < (u - 1); ++l2 ) {
			const int a = (l1 * u) + l2;
			const int b = (l1 * u) + l2 + 1;
			addSpring(a, b, KS_STRUCT, KD_STRUCT, STRUCT_SPRING);
		}
	}
	// vertical structure springs
	for( int l1 = 0; l1 < u; ++l1 ) {
		for( int l2 = 0; l2 < (v - 1); ++l2 ) {
			const int a = (l2 * u) + l1;
			const int b = ((l2 + 1) * u) + l1;
			addSpring(a, b, KS_STRUCT, KD_STRUCT, STRUCT_SPRING);
		}
	}
	// shear springs
	for( int l1 = 0; l1 < (v - 1); ++l1 ) {
		for( int l2 = 0; l2 < (u - 1); ++l2 ) {
			const int a1 = (l1 * u) + l2;
			const int b1 = ((l1 + 1) * u) + l2 + 1;
			addSpring(a1, b1, KS_SHEAR, KD_SHEAR, SHEAR_SPRING);
			const int a2 = ((l1 + 1) * u) + l2;
			const int b2 = (l1 * u) + l2 + 1;
			addSpring(a2, b2, KS_SHEAR, KD_SHEAR, SHEAR_SPRING);
		}
	}
	// bend springs
	for( int l1 = 0; l1 < v; ++l1 ) {
		for( int l2 = 0; l2 < (u - 2); ++l2 ) {
			const int a1 = (l1 * u) + l2;
			const int b1 = (l1 * u) + l2 + 2;
			addSpring(a1, b1, KS_BEND, KD_BEND, BEND_SPRING);
		}
		const int a2 = (l1 * u) + (u - 3);
		const int b2 = (l1 * u) + (u - 1);
		addSpring(a2, b2, KS_BEND, KD_BEND, BEND_SPRING);
	}
	for( int l1 = 0; l1 < u; ++l1 ) {
		for( int l2 = 0; l2 < (v - 2); ++l2 ) {
			const int a1 = (l2 * u) + l1;
			const int b1 = ((l2 + 2) * u) + l1;
			addSpring(a1, b1, KS_BEND, KD_BEND, BEND_SPRING);
		}
		const int a2 = ((v - 3) * u) + l1;
		const int b2 = ((v - 1) * u) + l1;
		addSpring(a2, b2, KS_BEND, KD_BEND, BEND_SPRING);
	}

	// create gpu stuff
	_vertexBuffer = device->createVertexBuffer();
	_vertices.resize(_totalPoints);
	updateGpuBuffers();
	_indexBuffer = device->createIndexBuffer();
	_indexBuffer->set(_indices.data(), _indices.size(), false);

}

void OpenCloth::update( float deltaTime ) {
	_timeAccumulator += deltaTime;
	if( _timeAccumulator >= _timeStep ) { // while???
		step(_timeStep);
		_timeAccumulator -= _timeStep;
	}

	updateGpuBuffers();
}

ciri::IVertexBuffer* OpenCloth::getVertexBuffer() const {
	return _vertexBuffer;
}

ciri::IIndexBuffer* OpenCloth::getIndexBuffer() const {
	return _indexBuffer;
}

void OpenCloth::step( float deltaTime ) {
	computeForces(deltaTime);
	integrateVerlet(deltaTime);
	provotDynamicInverse();
}

void OpenCloth::computeForces( float deltaTime ) {
	for( int i = 0; i < _totalPoints; ++i ) {
		_forces[i] = cc::Vec3f::zero();
		const cc::Vec3f velocity = getVerletVelocity(_positions[i], _lastPositions[i], deltaTime);
		if( (i != 0) && (i != _numX) ) {
			_forces[i] += _gravity * _mass;
		}
		_forces[i] += _damping * velocity;
	}

	for( int i = 0; i < _springs.size(); ++i ) {
		const cc::Vec3f p1 = _positions[_springs[i].p1];
		const cc::Vec3f p1Last = _lastPositions[_springs[i].p1];
		const cc::Vec3f p2 = _positions[_springs[i].p2];
		const cc::Vec3f p2Last = _lastPositions[_springs[i].p2];

		const cc::Vec3f v1 = getVerletVelocity(p1, p1Last, deltaTime);
		const cc::Vec3f v2 = getVerletVelocity(p2, p2Last, deltaTime);

		const cc::Vec3f deltaP = p1 - p2;
		const cc::Vec3f deltaV = v1 - v2;
		const float dist = deltaP.magnitude();

		const float leftTerm = -_springs[i].ks * (dist - _springs[i].restLength);
		const float rightTerm = _springs[i].kd * (deltaV.dot(deltaP) / dist);
		const cc::Vec3f springForce = (leftTerm + rightTerm) * deltaP.normalized();

		if( _springs[i].p1 != 0 && _springs[i].p1 != _numX ) {
			_forces[_springs[i].p1] += springForce;
		}
		if( _springs[i].p2 != 0 && _springs[i].p2 != _numX ) {
			_forces[_springs[i].p2] -= springForce;
		}
	}
}

void OpenCloth::integrateVerlet( float deltaTime ) {
	const float deltaTime2Mass = (deltaTime * deltaTime) / _mass;

	for( int i = 0; i < _totalPoints; ++i ) {
		const cc::Vec3f buffer = _positions[i];
		_positions[i] = _positions[i] + (_positions[i] - _lastPositions[i]) + deltaTime2Mass * _forces[i];
		_lastPositions[i] = buffer;

		//if( _positions[i].y < 0.0f ) {
			//_positions[i].y = 0.0f;
		//}
	}
}

void OpenCloth::provotDynamicInverse() {
	for( int i = 0; i < _springs.size(); ++i ) {
		const cc::Vec3f p1 = _positions[_springs[i].p1];
		const cc::Vec3f p2 = _positions[_springs[i].p2];
		cc::Vec3f deltaP = p1 - p2;
		float dist = deltaP.magnitude();
		if( dist > _springs[i].restLength ) {
			dist -= (_springs[i].restLength);
			dist /= 2.0f;
			deltaP.normalize();
			deltaP *= dist;
			if( 0 == _springs[i].p1 || _numX == _springs[i].p1 ) {
				_positions[_springs[i].p2] += deltaP;
			} else if( 0 == _springs[i].p2 || _numX == _springs[i].p2 ) {
				_positions[_springs[i].p1] -= deltaP;
			} else {
				_positions[_springs[i].p1] -= deltaP;
				_positions[_springs[i].p2] += deltaP;
			}
		}
	}
}

void OpenCloth::addSpring( int a, int b, float ks, float kd, int type ) {
	Spring spring;
	spring.p1 = a;
	spring.p2 = b;
	spring.ks = ks;
	spring.kd = kd;
	spring.type = type;
	const cc::Vec3f deltaP = _positions[a] - _positions[b];
	spring.restLength = sqrtf(deltaP.dot(deltaP));
	_springs.push_back(spring);
}

cc::Vec3f OpenCloth::getVerletVelocity( const cc::Vec3f& pos, const cc::Vec3f& lastPos, float deltaTime ) {
	return (pos - lastPos) / deltaTime;
}

void OpenCloth::updateGpuBuffers() {
	for( int i = 0; i < _totalPoints; ++i ) {
		_vertices[i].position = _positions[i];
	}
	_vertexBuffer->set(_vertices.data(), sizeof(Vertex), _vertices.size(), true);
}