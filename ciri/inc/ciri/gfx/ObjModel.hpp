#ifndef __ciri_objmodel__
#define __ciri_objmodel__

#include <string>
#include <vector>
#include <cc/Vec3.hpp>
#include <cc/Vec2.hpp>

namespace ciri {
	class ObjModel {
	public:
		struct ObjVertex {
			int position;
			int texcoord;
			int normal;

			ObjVertex()
				: position(-1), texcoord(-1), normal(-1) {
			}
		};

	public:
		ObjModel();
		~ObjModel();

		bool parse( const char* file );
		void reset();

		const std::vector<cc::Vec3f>& getPositions() const;
		const std::vector<cc::Vec2f>& getTexcoords() const;
		const std::vector<cc::Vec3f>& getNormals() const;
		const std::vector<ObjVertex>& getVertices() const;

	private:
		bool parseLine( const std::string& line );
		bool parseVertex( const std::string& line );
		bool parseFace( const std::string& line );
		bool parsePosition( const std::string& line );
		bool parseTexcoord( const std::string& line );
		bool parseNormal( const std::string& line );

	private:
		std::vector<cc::Vec3f> _positions;
		std::vector<cc::Vec2f> _texcoords;
		std::vector<cc::Vec3f> _normals;
		std::vector<ObjVertex> _vertices;
		std::vector<std::string> _firstSplit;
		std::vector<std::string> _secondSplit;
		std::vector<std::string> _split;
	};
} // ciri

#endif /* __ciri_objmodel__ */