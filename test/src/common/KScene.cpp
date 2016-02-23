#include "KScene.hpp"
#include <cc/MatrixFunc.hpp>
#include <fstream>
#include "Leb128.hpp"

KScene::Mesh::Mesh()
{
	_name = "";
	_vertices = std::vector<Vertex>();
	_indices = std::vector<unsigned int>();
}

const std::string& KScene::Mesh::getName() const
{
	return _name;
}

const std::vector<Vertex>& KScene::Mesh::getVertices() const
{
	return _vertices;
}

const std::vector<unsigned int>& KScene::Mesh::getIndices() const
{
	return _indices;
}

KScene::Xform::Xform()
{
	_name = "";
	_localPosition = cc::Vec3f();
	_localOrient = cc::Quatf();
	_localScale = cc::Vec3f(1.0f);
	_worldPosition = cc::Vec3f();
	_worldOrient = cc::Quatf();
	_worldScale = cc::Vec3f(1.0f);
	_parent = nullptr;
	_mesh = nullptr;
	_children = std::vector<Xform*>();
}

const std::string& KScene::Xform::getName() const
{
	return _name;
}

const cc::Vec3f& KScene::Xform::getPosition( bool worldSpace ) const
{
	if( worldSpace )
	{
		return _worldPosition;
	}
	return _localPosition;
}

const cc::Quatf& KScene::Xform::getOrientation( bool worldSpace ) const
{
	if( worldSpace )
	{
		return _worldOrient;
	}
	return _localOrient;
}

const cc::Vec3f& KScene::Xform::getScale( bool worldSpace ) const
{
	if( worldSpace )
	{
		return _worldScale;
	}
	return _localScale;
}

cc::Mat4f KScene::Xform::getMatrix( bool worldSpace ) const
{
	if( worldSpace && _parent != nullptr )
	{
		return _parent->getMatrix(true) * _matrix;
	}
	return _matrix;
}

const KScene::Mesh* KScene::Xform::getMesh() const
{
	return _mesh;
}

const std::vector<KScene::Xform*>& KScene::Xform::getChildren() const
{
	return _children;
}

void KScene::Xform::decomposeMatrix()
{
	cc::math::decompose(_matrix, &_localPosition, &_localOrient, &_localScale);

	if( _parent == nullptr )
	{
		// World position is the same as local if there's no parent.
		_worldPosition = _localPosition;
		_worldOrient = _localOrient;
		_worldScale = _localScale;
	}
	else
	{
		const cc::Mat4f worldMatrix = getMatrix(true);
		cc::math::decompose(worldMatrix, &_worldPosition, &_worldOrient, &_worldScale);
	}
}

KScene::Light::Light()
{
	_name = "";
	_parent = nullptr;
	_type = Light::kLightTypeInvalid;
	_color = cc::Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
	_intensity = 1.0f;
	_range = 10.0f;
	_innerConeAngle = 10.0f;
	_outerConeAngle = 12.0f;
}

const std::string& KScene::Light::getName() const
{
	return _name;
}

KScene::Xform* KScene::Light::getParent() const
{
	return _parent;
}

KScene::Light::Type KScene::Light::getType() const
{
	return _type;
}

const cc::Vec4f& KScene::Light::getColor() const
{
	return _color;
}

float KScene::Light::getIntensity() const
{
	return _intensity;
}

float KScene::Light::getRange() const
{
	return _range;
}

float KScene::Light::getInnerConeAngle() const
{
	return _innerConeAngle;
}

float KScene::Light::getOuterConeAngle() const
{
	return _outerConeAngle;
}

KScene::KScene()
{
	_meshes = std::vector<Mesh*>();
	_xforms = std::vector<Xform*>();
	_meshIds = std::unordered_map<int, Mesh*>();
	_xformIds = std::unordered_map<int, Xform*>();
	_xformChildIds = std::unordered_map<Xform*, std::vector<int>>();
	_lights = std::vector<Light*>();
	_lightIds = std::unordered_map<int, Light*>();
	_lightNames = std::unordered_map<std::string, Light*>();
	_root = nullptr;
}

KScene::~KScene()
{
	clean();
}

const std::vector<KScene::Mesh*>& KScene::getMeshes() const
{
	return _meshes;
}

const std::vector<KScene::Xform*>& KScene::getXforms() const
{
	return _xforms;
}

const std::vector<KScene::Light*>& KScene::getLights() const
{
	return _lights;
}

KScene::Xform* KScene::getRoot() const
{
	return _root;
}

KScene::Xform* KScene::getXformByName( const std::string& name )
{
	std::unordered_map<std::string, Xform*>::const_iterator existing = _xformNames.find(name);
	if( existing != _xformNames.end() )
	{
		return existing->second;
	}
	return nullptr;
}

KScene::Mesh* KScene::getMeshByName( const std::string& name )
{
	std::unordered_map<std::string, Mesh*>::const_iterator existing = _meshNames.find(name);
	if( existing != _meshNames.end() )
	{
		return existing->second;
	}
	return nullptr;
}

bool KScene::readBinaryFile( const char* file )
{
	clean();

	std::ifstream in(file, std::ios::in | std::ios::binary);
	if( !in.is_open() )
	{
		return false;
	}
	readModelData(in);
	readXformData(in);
	readLightData(in);
	in.close();

	if( _xforms.size() > 0 )
	{
		buildTree();
	}

	return true;
}

void KScene::printDebugInfo( bool verbose )
{
	printf("----MESH DATA----\n");
	printf("Meshes (%d)\n", _meshes.size());
	for( int currMesh = 0; currMesh < static_cast<int>(_meshes.size()); ++currMesh )
	{
		Mesh* mesh = _meshes[currMesh];
		printf("\t[%d] name (%s)\n", currMesh, mesh->_name.c_str());
		//printf("\t[%d] id (%d)\n", currMesh, mesh->id);
		printf("\t[%d] vertices (%d)\n", currMesh, mesh->_vertices.size());
		if( verbose )
		{
			for( int currVert = 0; currVert < static_cast<int>(mesh->_vertices.size()); ++currVert )
			{
				Vertex* vertex = &mesh->_vertices[currVert];
				printf("\t\t[%d] position (%f, %f, %f)\n", currVert, vertex->position.x, vertex->position.y, vertex->position.z);
				printf("\t\t[%d] normal (%f, %f, %f)\n", currVert, vertex->normal.x, vertex->normal.y, vertex->normal.z);
				printf("\t\t[%d] texcoord (%f, %f)\n", currVert, vertex->texcoord.x, vertex->texcoord.y);
			}
		}
		printf("\t[%d] indices (%d)\n", currMesh, mesh->_indices.size());
		if( verbose )
		{
			for( int currIdx = 0; currIdx < static_cast<int>(mesh->_indices.size()); ++currIdx )
			{
				printf("\t\t[%d] (%d)\n", currIdx, mesh->_indices[currIdx]);
			}
		}
	}

	printf("\n----XFORM TREE----\n");
	printXform(_root, 2, verbose);

	printf("\n----LIGHT DATA----\n");
	printf("Lights (%d)\n", _lights.size());
	for( int currLight = 0; currLight < static_cast<int>(_lights.size()); ++currLight )
	{
		Light* light = _lights[currLight];
		printf("\t[%d] name (%s)\n", currLight, light->_name.c_str());
		if( verbose )
		{
			printf("\t[%d] parent (%s)\n", currLight, light->_parent == nullptr ? "none" : light->_parent->_name.c_str());
			switch( light->_type )
			{
			case Light::kLightTypeInvalid:
				{
					printf("\t[%d] type(%s)\n", currLight, "invalid");
					break;
				}
			case Light::kLightTypeAmbient:
				{
					printf("\t[%d] type(%s)\n", currLight, "ambient");
					break;
				}
			case Light::kLightTypeDirectional:
				{
					printf("\t[%d] type(%s)\n", currLight, "directional");
					break;
				}
			case Light::kLightTypePoint:
				{
					printf("\t[%d] type(%s)\n", currLight, "point");
					break;
				}
			case Light::kLightTypeSpot:
				{
					printf("\t[%d] type(%s)\n", currLight, "spot");
					break;
				}
			}
			printf("\t[%d] color(%f, %f, %f, %f)\n", currLight, light->_color.r, light->_color.g, light->_color.b, light->_color.a);
			printf("\t[%d] intensity(%f)\n", currLight, light->_intensity);
			printf("\t[%d]	inner angle(%f)\n", currLight, light->_innerConeAngle);
			printf("\t[%d] outer angle(%f)\n", currLight, light->_outerConeAngle);
		}
	}
}

void KScene::clean()
{
	// Delete meshes.
	for( int i = 0; i < static_cast<int>(_meshes.size()); ++i )
	{
		if( !_meshes[i] )
		{
			continue;
		}
		delete _meshes[i];
		_meshes[i] = nullptr;
	}
	_meshes.clear();

	// Delete xforms.
	for( int i = 0; i < static_cast<int>(_xforms.size()); ++i )
	{
		if( !_xforms[i] )
		{
			continue;
		}
		delete _xforms[i];
		_xforms[i] = nullptr;
	}
	_xforms.clear();

	// Clear maps.
	_meshIds.clear();
	_xformIds.clear();
	_xformChildIds.clear();
	_xformNames.clear();
	_meshNames.clear();

	// No root object now.
	_root = nullptr;
}

void KScene::readModelData( std::ifstream& is )
{
	// Number of meshes.
	int meshCount = 0;
	is.read(reinterpret_cast<char*>(&meshCount), sizeof(int));

	for( int currMesh = 0; currMesh < meshCount; ++currMesh )
	{
		Mesh* mesh = new Mesh();
		_meshes.push_back(mesh);

		// Decode the size of the name string.
		int nameSize = leb128::decodeStream(is);
		// Read the name.
		char* tmpName = new char[nameSize+1];
		memset(tmpName, '\0', nameSize+1); // Append null terminator (hence +1).
		is.read(reinterpret_cast<char*>(tmpName), nameSize);
		mesh->_name = std::string(tmpName);
		delete tmpName;
		tmpName = nullptr;

		// Add the mesh by name to the mesh name map.
		_meshNames[mesh->_name] = mesh;

		// Read id.
		int id = -1;
		is.read(reinterpret_cast<char*>(&id), sizeof(int));
		// Set the id in the map to point to this mesh.
		_meshIds[id] = mesh;

		// Read vertex count.
		int vertexCount = 0;
		is.read(reinterpret_cast<char*>(&vertexCount), sizeof(int));
		// Reserve size for vertices.
		mesh->_vertices = std::vector<Vertex>(vertexCount);
		// Read vertices.
		for( int currVert = 0; currVert < vertexCount; ++currVert )
		{
			Vertex* vertex = &mesh->_vertices[currVert];
			// Read position.
			is.read(reinterpret_cast<char*>(&vertex->position.x), sizeof(float));
			is.read(reinterpret_cast<char*>(&vertex->position.y), sizeof(float));
			is.read(reinterpret_cast<char*>(&vertex->position.z), sizeof(float));
			// Read normal.
			is.read(reinterpret_cast<char*>(&vertex->normal.x), sizeof(float));
			is.read(reinterpret_cast<char*>(&vertex->normal.y), sizeof(float));
			is.read(reinterpret_cast<char*>(&vertex->normal.z), sizeof(float));
			// Read texcoord.
			is.read(reinterpret_cast<char*>(&vertex->texcoord.x), sizeof(float));
			is.read(reinterpret_cast<char*>(&vertex->texcoord.y), sizeof(float));
		}

		// Read index count.
		int indexCount = 0;
		is.read(reinterpret_cast<char*>(&indexCount), sizeof(int));
		// Reserve indices.
		mesh->_indices = std::vector<unsigned int>(indexCount);
		// Read indices.
		for( int currIdx = 0; currIdx < indexCount; ++currIdx )
		{
			unsigned int idx = 0;
			is.read(reinterpret_cast<char*>(&idx), sizeof(unsigned int));
			mesh->_indices[currIdx] = idx;
		}
	}
}

void KScene::readXformData( std::ifstream& is )
{
	// Read xform count.
	int xformCount = 0;
	is.read(reinterpret_cast<char*>(&xformCount), sizeof(int));
	for( int currXform = 0; currXform < xformCount; ++currXform )
	{
		Xform* xform = new Xform();
		_xforms.push_back(xform);

		// Decode the size of the name string.
		int nameSize = leb128::decodeStream(is);
		// Read the name.
		char* tmpName = new char[nameSize+1];
		memset(tmpName, '\0', nameSize+1); // Append null terminator (hence +1).
		is.read(reinterpret_cast<char*>(tmpName), nameSize);
		xform->_name = std::string(tmpName);
		delete tmpName;
		tmpName = nullptr;

		// Insert into the xform name map by the read name.
		_xformNames[xform->_name] = xform;

		// Read id.
		int id = -1;
		is.read(reinterpret_cast<char*>(&id), sizeof(int));
		// Set the id in the map to point to this xform.
		_xformIds[id] = xform;

		// Read parent's ID (if not -1, will exist above this node).
		int parentId = -1;
		is.read(reinterpret_cast<char*>(&parentId), sizeof(int));
		if( parentId != -1 )
		{
			xform->_parent = _xformIds[parentId];
		}

		// Read matrix.
		for( int x = 0; x < 4; ++x )
		{
			for( int y = 0; y < 4; ++y )
			{
				is.read(reinterpret_cast<char*>(&xform->_matrix[x][y]), sizeof(float));
			}
		}

		// Decompose the matrix read in into both local and world space.
		xform->decomposeMatrix();

		// Read if the xform is a source xform.
		bool isSourceXform = false;
		is.read(reinterpret_cast<char*>(&isSourceXform), sizeof(bool));

		// Read the mesh id (can be "null" (-1)).
		int meshId = -1;
		is.read(reinterpret_cast<char*>(&meshId), sizeof(int));
		// Set the mesh pointer based on the id.
		if( meshId != -1 )
		{
			xform->_mesh = _meshIds[meshId];
		}

		// Read the number of chid nodes.
		int childCount = 0;
		is.read(reinterpret_cast<char*>(&childCount), sizeof(int));
		// Reserve space.
		for( int currChild = 0; currChild < childCount; ++currChild )
		{
			int childId = -1;
			is.read(reinterpret_cast<char*>(&childId), sizeof(int));
			_xformChildIds[xform].push_back(childId);
		}
	}
}

void KScene::readLightData( std::ifstream& is )
{
	int lightCount = 0;
	is.read(reinterpret_cast<char*>(&lightCount), sizeof(int));
	for( int currLight = 0; currLight < lightCount; ++currLight )
	{
		Light* light = new Light();
		_lights.push_back(light);

		// Read the name.
		int nameSize = leb128::decodeStream(is);
		char* tmpName = new char[nameSize+1];
		memset(tmpName, '\0', nameSize+1);
		is.read(reinterpret_cast<char*>(tmpName), nameSize);
		light->_name = std::string(tmpName);
		delete tmpName;
		tmpName = nullptr;

		// Insert into name->light map.
		_lightNames[light->_name] = light;

		// Read id.
		int id = -1;
		is.read(reinterpret_cast<char*>(&id), sizeof(int));
		_lightIds[id] = light;

		// Read xform id and set it up.
		int parentId = -1;
		is.read(reinterpret_cast<char*>(&parentId), sizeof(int));
		if( parentId != -1 )
		{
			light->_parent = _xformIds[parentId];
		}

		// Read light type (as an int, casted to enum).
		int type = 0;
		is.read(reinterpret_cast<char*>(&type), sizeof(int));
		light->_type = static_cast<Light::Type>(type);

		// Read color.
		is.read(reinterpret_cast<char*>(&light->_color.r), sizeof(float));
		is.read(reinterpret_cast<char*>(&light->_color.g), sizeof(float));
		is.read(reinterpret_cast<char*>(&light->_color.b), sizeof(float));
		is.read(reinterpret_cast<char*>(&light->_color.a), sizeof(float));

		// Read intensity.
		is.read(reinterpret_cast<char*>(&light->_intensity), sizeof(float));

		if( light->_type == Light::kLightTypePoint )
		{
			is.read(reinterpret_cast<char*>(&light->_range), sizeof(float));
		}
		else if( light->_type == Light::kLightTypeSpot )
		{
			is.read(reinterpret_cast<char*>(&light->_range), sizeof(float));
			is.read(reinterpret_cast<char*>(&light->_innerConeAngle), sizeof(float));
			is.read(reinterpret_cast<char*>(&light->_outerConeAngle), sizeof(float));
		}
	}
}

void KScene::buildTree()
{
	_root = _xforms[0];

	for( int currXform = 0; currXform < static_cast<int>(_xforms.size()); ++currXform )
	{
		Xform* xform = _xforms[currXform];
		std::vector<int>& childIds = _xformChildIds[xform];

		// Set child pointers.
		xform->_children = std::vector<Xform*>(childIds.size());
		for( int currChild = 0; currChild < static_cast<int>(childIds.size()); ++currChild )
		{
			xform->_children[currChild] = _xforms[childIds[currChild]];
		}
	}
}

void KScene::printXform( Xform* xform, int spacing, bool verbose )
{
	for( int i=0; i < spacing; ++i ){ printf("-"); }
	printf("(%s)\n", xform->_name.c_str());

	if( verbose )
	{
		for( int i=0; i < spacing; ++i ){ printf("-"); }
		//printf("pos (%f, %f, %f)\n", xform->_position.x, xform->_position.y, xform->_position.z);

		for( int i=0; i < spacing; ++i ){ printf("-"); }
		//printf("ori (%f, %f, %f, %f)\n", xform->_orientation._x, xform->_orientation._y, xform->_orientation._z, xform->_orientation._w);

		for( int i=0; i < spacing; ++i ){ printf("-"); }
		//printf("scale (%f, %f, %f)\n", xform->_scale.x, xform->_scale.y, xform->_scale.z);

		for( int i=0; i < spacing; ++i ){ printf("-"); }
		printf("mesh (%s)\n", (xform->_mesh==nullptr) ? "none" : xform->_mesh->_name.c_str());
	}

	for( int currChild = 0; currChild < static_cast<int>(xform->_children.size()); ++currChild )
	{
		printXform(xform->_children[currChild], spacing+spacing, verbose);
	}
}