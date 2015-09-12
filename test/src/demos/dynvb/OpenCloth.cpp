#include "OpenCloth.hpp"

OpenCloth::OpenCloth()
	: _built(false), _device(nullptr), _vertexBuffer(nullptr), _indexBuffer(nullptr), _vertices(nullptr), _indexCount(0), _indices(nullptr),
		_positions(nullptr), _lastPositions(nullptr), _forces(nullptr), _springCount(0), _springs(nullptr), _shader(nullptr), _constantsBuffer(nullptr) {
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
	clean();
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
	_invMass = 1.0f / mass;
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

void OpenCloth::build( std::shared_ptr<ciri::IGraphicsDevice> device ) {
	if( _built ) {
		return;
	}

	_device = device;

	// gpu resources can fail, so create them first
	if( !createGpuResources() ) {
		return;
	}

	_positions = new cc::Vec3f[_totalPoints];
	_lastPositions = new cc::Vec3f[_totalPoints];
	_forces = new cc::Vec3f[_totalPoints];

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

	// compute number of springs and allocate space
	_springCount = v * (u - 1); // horizontal structure springs
	_springCount += u * (v - 1); // vertical structure springs
	_springCount += ((v - 1) * (u - 1)) * 2; // shear springs
	_springCount += (v * (u - 1)); // bend springs 1
	_springCount += (u * (v - 1)); // bend springs 2
	_springs = new Spring[_springCount];

	// tracks which spring we are modifying; is incremented by addspring()
	int currSpring = 0;

	// horizontal structure springs
	for( int l1 = 0; l1 < v; ++l1 ) {
		for( int l2 = 0; l2 < (u - 1); ++l2 ) {
			const int a = (l1 * u) + l2;
			const int b = (l1 * u) + l2 + 1;
			addSpring(&currSpring, a, b, _ksStruct, _kdStruct);
		}
	}
	// vertical structure springs
	for( int l1 = 0; l1 < u; ++l1 ) {
		for( int l2 = 0; l2 < (v - 1); ++l2 ) {
			const int a = (l2 * u) + l1;
			const int b = ((l2 + 1) * u) + l1;
			addSpring(&currSpring, a, b, _ksStruct, _kdStruct);
		}
	}
	// shear springs
	for( int l1 = 0; l1 < (v - 1); ++l1 ) {
		for( int l2 = 0; l2 < (u - 1); ++l2 ) {
			const int a1 = (l1 * u) + l2;
			const int b1 = ((l1 + 1) * u) + l2 + 1;
			addSpring(&currSpring, a1, b1, _ksShear, _kdShear);
			const int a2 = ((l1 + 1) * u) + l2;
			const int b2 = (l1 * u) + l2 + 1;
			addSpring(&currSpring, a2, b2, _ksShear, _kdShear);
		}
	}
	// bend springs
	for( int l1 = 0; l1 < v; ++l1 ) {
		for( int l2 = 0; l2 < (u - 2); ++l2 ) {
			const int a1 = (l1 * u) + l2;
			const int b1 = (l1 * u) + l2 + 2;
			addSpring(&currSpring, a1, b1, _ksBend, _kdBend);
		}
		const int a2 = (l1 * u) + (u - 3);
		const int b2 = (l1 * u) + (u - 1);
		addSpring(&currSpring, a2, b2, _ksBend, _kdBend);
	}
	for( int l1 = 0; l1 < u; ++l1 ) {
		for( int l2 = 0; l2 < (v - 2); ++l2 ) {
			const int a1 = (l2 * u) + l1;
			const int b1 = ((l2 + 2) * u) + l1;
			addSpring(&currSpring, a1, b1, _ksBend, _kdBend);
		}
		const int a2 = ((v - 3) * u) + l1;
		const int b2 = ((v - 1) * u) + l1;
		addSpring(&currSpring, a2, b2, _ksBend, _kdBend);
	}

	// create gpu stuff
	createGpuBuffers();

	_built = true;

}

void OpenCloth::update( float deltaTime ) {
	if( !_built ) {
		return;
	}

	_timeAccumulator += deltaTime;
	if( _timeAccumulator >= _timeStep ) { // while???
		step(_timeStep);
		_timeAccumulator -= _timeStep;
	}

	computeNormals();
	updateGpuVertexBuffer();
}

void OpenCloth::clean() {
	if( !_built ) {
		return;
	}

	if( _springs != nullptr ) {
		delete[] _springs;
		_springs = nullptr;
	}

	if( _forces != nullptr ) {
		delete[] _forces;
		_forces = nullptr;
	}

	if( _lastPositions != nullptr ) {
		delete[] _lastPositions;
		_lastPositions = nullptr;
	}

	if( _positions != nullptr ) {
		delete[] _positions;
		_positions = nullptr;
	}

	if( _indices != nullptr ) {
		delete[] _indices;
		_indices = nullptr;
	}

	if( _vertices != nullptr ) {
		delete[] _vertices;
		_vertices = nullptr;
	}
}

bool OpenCloth::isBuilt() const {
	return _built;
}

const std::shared_ptr<ciri::IVertexBuffer>& OpenCloth::getVertexBuffer() const {
	return _vertexBuffer;
}

const std::shared_ptr<ciri::IIndexBuffer>& OpenCloth::getIndexBuffer() const {
	return _indexBuffer;
}

const std::shared_ptr<ciri::IShader>& OpenCloth::getShader() const {
	return _shader;
}

OpenCloth::Constants& OpenCloth::getConstants() {
	return _constants;
}

bool OpenCloth::updateConstants() {
	if( !_built ) {
		return false;
	}

	bool success = true;
	if( ciri::failed(_constantsBuffer->setData(sizeof(Constants), &_constants)) ) {
		success = false;
	}
	return success;
}

bool OpenCloth::createGpuResources() {
	// create the shader
	_shader = _device->createShader();
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Normal, 0));
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float4, ciri::VertexUsage::Tangent, 0));
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float2, ciri::VertexUsage::Texcoord, 0));
	const std::string shaderExt = _device->getShaderExt();
	const std::string vsFile = ("dynvb/fabric_plaid_vs" + shaderExt);
	const std::string psFile = ("dynvb/fabric_plaid_ps" + shaderExt);
	if( ciri::failed(_shader->loadFromFile(vsFile.c_str(), nullptr, psFile.c_str())) ) {
		printf("Failed to build fabric shader:\n");
		for( unsigned int i = 0; i < _shader->getErrors().size(); ++i ) {
			printf("%s\n", _shader->getErrors()[i].msg.c_str());
		}
		return false;
	}

	// create constant buffers
	_constantsBuffer = _device->createConstantBuffer();
	if( ciri::failed(_constantsBuffer->setData(sizeof(Constants), &_constants)) ) {
		return false;
	}

	// assign constant buffers to shader
	if( ciri::failed(_shader->addConstants(_constantsBuffer, "Constants", ciri::ShaderStage::Vertex)) ) {
		return false;
	}

	return true;
}

void OpenCloth::createGpuBuffers() {
	//  create the vertex buffer, allocate raw vertices, upload vertices to gpu
	_vertexBuffer = _device->createVertexBuffer();
	_vertices = new Vertex[_totalPoints];
	updateGpuVertexBuffer();

	// create index buffer, allocate and compute indices, upload indices to gpu
	_indexBuffer = _device->createIndexBuffer();
	_indexCount = _divsX * _divsY * 2 * 3;
	_indices = new int[_indexCount];
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
	_indexBuffer->set(_indices, _indexCount, false);
}

void OpenCloth::updateGpuVertexBuffer() {
	for( int i = 0; i < _totalPoints; ++i ) {
		_vertices[i].position = _positions[i];
	}
	_vertexBuffer->set(_vertices, sizeof(Vertex), _totalPoints, true);
}

void OpenCloth::step( float deltaTime ) {
	computeForces(deltaTime);
	integrateVerlet(deltaTime);
	provotDynamicInverse();
}

void OpenCloth::computeForces( float deltaTime ) {
	for( int i = 0; i < _totalPoints; ++i ) {
		_forces[i].x = _forces[i].y = _forces[i].z = 0.0f;
		const cc::Vec3f velocity = getVerletVelocity(_positions[i], _lastPositions[i], deltaTime);
		if( (i != 0) && (i != _divsX) ) {
			_forces[i] += _gravity * _mass;
		}
		_forces[i] += _damping * velocity;
	}

	for( int i = 0; i < _springCount; ++i ) {
		const cc::Vec3f& p1 = _positions[_springs[i].p1];
		const cc::Vec3f& p1Last = _lastPositions[_springs[i].p1];
		const cc::Vec3f& p2 = _positions[_springs[i].p2];
		const cc::Vec3f& p2Last = _lastPositions[_springs[i].p2];

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
	const float deltaTime2Mass = (deltaTime * deltaTime) * _invMass;

	for( int i = 0; i < _totalPoints; ++i ) {
		const cc::Vec3f tmp = _positions[i]; // copy
		_positions[i] = _positions[i] + (_positions[i] - _lastPositions[i]) + deltaTime2Mass * _forces[i];
		_lastPositions[i] = tmp;
	}
}

void OpenCloth::provotDynamicInverse() {
	for( int i = 0; i < _springCount; ++i ) {
		const cc::Vec3f& p1 = _positions[_springs[i].p1];
		const cc::Vec3f& p2 = _positions[_springs[i].p2];
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

void OpenCloth::computeNormals() {
	for( int i = 0; i < (_indexCount - 3); i += 3 ) {
		const cc::Vec3f& p0 = _positions[_indices[i]];
		const cc::Vec3f& p1 = _positions[_indices[i+1]];
		const cc::Vec3f& p2 = _positions[_indices[i+2]];
		const cc::Vec3f nrm = (p1 - p0).cross(p2 - p0);
		_vertices[_indices[i]].normal += nrm;
		_vertices[_indices[i+1]].normal += nrm;
		_vertices[_indices[i+2]].normal += nrm;
	}

	for( int i = 0; i < _totalPoints; ++i ) {
		_vertices[i].normal.normalize();
	}
}

void OpenCloth::addSpring( int* idx, int a, int b, float ks, float kd ) {
	_springs[*idx].p1 = a;
	_springs[*idx].p2 = b;
	_springs[*idx].ks = ks;
	_springs[*idx].kd = kd;
	const cc::Vec3f deltaP = _positions[a] - _positions[b];
	_springs[*idx].restLength = sqrtf(deltaP.dot(deltaP));

	*idx += 1;
}

cc::Vec3f OpenCloth::getVerletVelocity( const cc::Vec3f& pos, const cc::Vec3f& lastPos, float deltaTime ) {
	return (pos - lastPos) / deltaTime;
}
