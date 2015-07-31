#include <ciri/gfx/ObjModel.hpp>
#include <fstream>
#include <cmath> // not ccmath ;)
#include <ciri/util/StrUtil.hpp>

namespace ciri {
	ObjModel::ObjModel() {
	}

	ObjModel::~ObjModel() {
	}

	bool ObjModel::parse( const char* file ) {
		reset();

		std::ifstream in(file, std::ios::in);
		if( !in.is_open() ) {
			return false;
		}

		bool success = true;
		std::string line;
		while( std::getline(in, line) ) {
			if( !parseLine(line) ) {
				success = false;
				break;
			}
		}
		in.close();

		return success;
	}

	void ObjModel::reset() {
		_positions.clear();
		_texcoords.clear();
		_normals.clear();
		_vertices.clear();
		_firstSplit.clear();
		_secondSplit.clear();
		_split.clear();
	}

	const std::vector<cc::Vec3f>& ObjModel::getPositions() const {
		return _positions;
	}

	const std::vector<cc::Vec2f>& ObjModel::getTexcoords() const {
		return _texcoords;
	}
	const std::vector<cc::Vec3f>& ObjModel::getNormals() const {
		return _normals;
	}

	const std::vector<ObjModel::ObjVertex>& ObjModel::getVertices() const {
		return _vertices;
	}

	bool ObjModel::parseLine( const std::string& line ) {
		if( line.length() < 1 ) {
			return true; // blank lines can trigger this
		}

		switch( line[0] ) {
			// vertex
			case 'v': {
				return parseVertex(line);
			}

			// face
			case 'f': {
				return parseFace(line);
			}

			default: {
				return true;
			}
		}
	}

	bool ObjModel::parseVertex( const std::string& line ) {
		if( line.length() < 2 ) {
			return false;
		}

		switch( line[1] ) {
			// position
			case ' ': {
				return parsePosition(line);
			}

			// texcoord
			case 't': {
				return parseTexcoord(line);
			}

			// normal
			case 'n': {
				return parseNormal(line);
			}

			default: {
				return true;
			}
		}
	}

	bool ObjModel::parseFace( const std::string& line ) {
		_firstSplit.clear();
		if( strutil::split(line.c_str(), ' ', &_firstSplit)->size() < 4 ) {
			return false;
		}

		// for all vertices (assuming 3)
		for( int i = 1; i < 4; ++i ) {
			_secondSplit.clear();
			strutil::split(_firstSplit[i].c_str(), '/', &_secondSplit);

			ObjVertex vertex;
			const int totalSlashes = strutil::countCharactersInString(_firstSplit[i].c_str(), '/', 0);
			if( 0 == totalSlashes ) {
				// only position
				vertex.position = atoi(_secondSplit[0].c_str())-1;
			} else if( 1 == totalSlashes ) {
				// position and texcoord
				vertex.position = atoi(_secondSplit[0].c_str())-1;
				vertex.texcoord = atoi(_secondSplit[1].c_str())-1;
			} else if( 2 == totalSlashes ) {
				// position and normal
				if( 0 == _secondSplit[1].size() ) { // [1] is empty in this case
					vertex.position = atoi(_secondSplit[0].c_str())-1;
					vertex.normal = atoi(_secondSplit[2].c_str())-1;
				} else {
					// position, texcoord, and normal
					vertex.position = atoi(_secondSplit[0].c_str())-1;
					vertex.texcoord = atoi(_secondSplit[1].c_str())-1;
					vertex.normal = atoi(_secondSplit[2].c_str())-1;
				}
			}

			_vertices.push_back(vertex);
		}

		return true;
	}

	bool ObjModel::parsePosition( const std::string& line ) {
		_split.clear();
		if( strutil::split(line.c_str(), ' ', &_split)->size() < 4 ) {
			return false;
		}

		// 3ds max is for heretics
		// i could remove spaces here but meh
		const int idx = (0 == _split[1].size()) ? 2 : 1;

		cc::Vec3f position;
		position.x = static_cast<float>(atof(_split[idx].c_str()));
		position.y = static_cast<float>(atof(_split[idx+1].c_str()));
		position.z = static_cast<float>(atof(_split[idx+2].c_str()));
		_positions.push_back(position);

		return true;
	}

	bool ObjModel::parseTexcoord( const std::string& line ) {
		_split.clear();
		if( strutil::split(line.c_str(), ' ', &_split)->size() < 3 ) {
			return false;
		}

		cc::Vec2f texcoord;
		texcoord.x = static_cast<float>(atof(_split[1].c_str()));
		texcoord.y = static_cast<float>(atof(_split[2].c_str()));
		_texcoords.push_back(texcoord);

		return true;
	}
	bool ObjModel::parseNormal( const std::string& line ) {
		_split.clear();
		if( strutil::split(line.c_str(), ' ', &_split)->size() < 4 ) {
			return false;
		}

		cc::Vec3f normal;
		normal.x = static_cast<float>(atof(_split[1].c_str()));
		normal.y = static_cast<float>(atof(_split[2].c_str()));
		normal.z = static_cast<float>(atof(_split[3].c_str()));
		_normals.push_back(normal);

		return true;
	}
} // ciri