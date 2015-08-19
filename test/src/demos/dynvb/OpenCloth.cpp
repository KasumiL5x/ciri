#include "OpenCloth.hpp"

OpenCloth::OpenCloth()
	: _built(false), _device(nullptr), _vertexBuffer(nullptr), _indexBuffer(nullptr) {
	setDivisions(20, 20);
	setSize(7);
	setMass(1.0f);
	setDamping(-0.0125f);
	setSpringParams(500.75f, -0.25f, 500.75f, -0.25f, 500.95f, -0.25f);
	setGravity(cc::Vec3f(0.0f, -9.81f ,0.0f));

	_timeStep = 1.0f / 60.0f;
	_timeAccumulator = _timeStep;
}

OpenCloth::~OpenCloth() {
}

void OpenCloth::setDivisions( int x, int y ) {
	if( _built ) {
		return;
	}

	_divsX = x;
	_divsY = y;
	_totalPoints = (_divsX+1) * (_divsY+1); // must compute this again
}

void OpenCloth::setSize( int size ) {
	if( _built ) {
		return;
	}

	_size = size;
	_horizontalSize = static_cast<float>(_size) * 0.5f; // must compute this again
}

void OpenCloth::setMass( float mass ) {
	if( _built ) {
		return;
	}

	_mass = mass;
}

void OpenCloth::setDamping( float damping ) {
	if( _built ) {
		return;
	}

	_damping = damping;
}

void OpenCloth::setSpringParams( float structKs, float structKd, float shearKs, float shearKd, float bendKs, float bendKd ) {
	if( _built ) {
		return;
	}

	_ksStruct = structKs;
	_kdStruct = structKd;
	_ksShear = shearKs;
	_kdShear = shearKd;
	_ksBend = bendKs;
	_kdBend = bendKd;
}

void OpenCloth::setGravity( const cc::Vec3f& gravity ) {
	_gravity = gravity;
}

void OpenCloth::build( ciri::IGraphicsDevice* device ) {
	if( _built ) {
		return;
	}

	_device = device;

	_positions.resize(_totalPoints);
	_lastPositions.resize(_totalPoints);
	_forces.resize(_totalPoints);

	// fill in positions
	const int u = _divsX+1;
	const int v = _divsY+1;
	int count = 0;
	for( int j = 0; j <= _divsY; ++j ) {
		for( int i = 0; i <= _divsX; ++i ) {
			const float x = ((float(i) / (u-1)) * 2.0f - 1.0f) * _horizontalSize;
			const float y = 0.0f;//_size+1;
			const float z = ((float(j) / (v-1)) * _size);
			_positions[count] = cc::Vec3f(x, y, z);
			_lastPositions[count] = _positions[count];
			count++;
		}
	}

	// horizontal structure springs
	for( int l1 = 0; l1 < v; ++l1 ) {
		for( int l2 = 0; l2 < (u - 1); ++l2 ) {
			const int a = (l1 * u) + l2;
			const int b = (l1 * u) + l2 + 1;
			addSpring(a, b, _ksStruct, _kdStruct);
		}
	}
	// vertical structure springs
	for( int l1 = 0; l1 < u; ++l1 ) {
		for( int l2 = 0; l2 < (v - 1); ++l2 ) {
			const int a = (l2 * u) + l1;
			const int b = ((l2 + 1) * u) + l1;
			addSpring(a, b, _ksStruct, _kdStruct);
		}
	}
	// shear springs
	for( int l1 = 0; l1 < (v - 1); ++l1 ) {
		for( int l2 = 0; l2 < (u - 1); ++l2 ) {
			const int a1 = (l1 * u) + l2;
			const int b1 = ((l1 + 1) * u) + l2 + 1;
			addSpring(a1, b1, _ksShear, _kdShear);
			const int a2 = ((l1 + 1) * u) + l2;
			const int b2 = (l1 * u) + l2 + 1;
			addSpring(a2, b2, _ksShear, _kdShear);
		}
	}
	// bend springs
	for( int l1 = 0; l1 < v; ++l1 ) {
		for( int l2 = 0; l2 < (u - 2); ++l2 ) {
			const int a1 = (l1 * u) + l2;
			const int b1 = (l1 * u) + l2 + 2;
			addSpring(a1, b1, _ksBend, _kdBend);
		}
		const int a2 = (l1 * u) + (u - 3);
		const int b2 = (l1 * u) + (u - 1);
		addSpring(a2, b2, _ksBend, _kdBend);
	}
	for( int l1 = 0; l1 < u; ++l1 ) {
		for( int l2 = 0; l2 < (v - 2); ++l2 ) {
			const int a1 = (l2 * u) + l1;
			const int b1 = ((l2 + 2) * u) + l1;
			addSpring(a1, b1, _ksBend, _kdBend);
		}
		const int a2 = ((v - 3) * u) + l1;
		const int b2 = ((v - 1) * u) + l1;
		addSpring(a2, b2, _ksBend, _kdBend);
	}

	// create gpu stuff
	createGpuBuffers();

	_built = true;

}

void OpenCloth::update( float deltaTime ) {
	_timeAccumulator += deltaTime;
	if( _timeAccumulator >= _timeStep ) { // while???
		step(_timeStep);
		_timeAccumulator -= _timeStep;
	}

	updateGpuVertexBuffer();
}

ciri::IVertexBuffer* OpenCloth::getVertexBuffer() const {
	return _vertexBuffer;
}

ciri::IIndexBuffer* OpenCloth::getIndexBuffer() const {
	return _indexBuffer;
}

void OpenCloth::createGpuBuffers() {
	//  create the vertex buffer, allocate raw vertices, upload vertices to gpu
	_vertexBuffer = _device->createVertexBuffer();
	_vertices.resize(_totalPoints);
	updateGpuVertexBuffer();

	// create index buffer, allocate and compute indices, upload indices to gpu
	_indexBuffer = _device->createIndexBuffer();
	_indices.resize(_divsX * _divsY * 2 * 3);
	int* id = &_indices[0];
	for( int i = 0; i < _divsY; ++i ) {
		for( int j = 0; j < _divsX; ++j ) {
			const int i0 = i * (_divsX+1) + j;
			const int i1 = i0 + 1;
			const int i2 = i0 + (_divsX+1);
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
	_indexBuffer->set(_indices.data(), _indices.size(), false);
}

void OpenCloth::updateGpuVertexBuffer() {
	for( int i = 0; i < _totalPoints; ++i ) {
		_vertices[i].position = _positions[i];
	}
	_vertexBuffer->set(_vertices.data(), sizeof(Vertex), _vertices.size(), true);
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
		if( (i != 0) && (i != _divsX) ) {
			_forces[i] += _gravity * _mass;
		}
		_forces[i] += _damping * velocity;
	}

	for( unsigned int i = 0; i < _springs.size(); ++i ) {
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

		if( _springs[i].p1 != 0 && _springs[i].p1 != _divsX ) {
			_forces[_springs[i].p1] += springForce;
		}
		if( _springs[i].p2 != 0 && _springs[i].p2 != _divsX ) {
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
	}
}

void OpenCloth::provotDynamicInverse() {
	for( unsigned int i = 0; i < _springs.size(); ++i ) {
		const cc::Vec3f p1 = _positions[_springs[i].p1];
		const cc::Vec3f p2 = _positions[_springs[i].p2];
		cc::Vec3f deltaP = p1 - p2;
		float dist = deltaP.magnitude();
		if( dist > _springs[i].restLength ) {
			dist -= (_springs[i].restLength);
			dist /= 2.0f;
			deltaP.normalize();
			deltaP *= dist;
			if( 0 == _springs[i].p1 || _divsX == _springs[i].p1 ) {
				_positions[_springs[i].p2] += deltaP;
			} else if( 0 == _springs[i].p2 || _divsX == _springs[i].p2 ) {
				_positions[_springs[i].p1] -= deltaP;
			} else {
				_positions[_springs[i].p1] -= deltaP;
				_positions[_springs[i].p2] += deltaP;
			}
		}
	}
}

void OpenCloth::addSpring( int a, int b, float ks, float kd ) {
	Spring spring;
	spring.p1 = a;
	spring.p2 = b;
	spring.ks = ks;
	spring.kd = kd;
	const cc::Vec3f deltaP = _positions[a] - _positions[b];
	spring.restLength = sqrtf(deltaP.dot(deltaP));
	_springs.push_back(spring);
}

cc::Vec3f OpenCloth::getVerletVelocity( const cc::Vec3f& pos, const cc::Vec3f& lastPos, float deltaTime ) {
	return (pos - lastPos) / deltaTime;
}
