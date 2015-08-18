#ifndef __test_model__
#define __test_model__

#include <vector>
#include <ciri/gfx/IGraphicsDevice.hpp>
#include "Vertex.hpp"
#include "Transform.hpp"

class Model {
public:
	Model();
	~Model();

	void addVertex( const Vertex& vertex );
	void addIndex( int index );
	bool addFromObj( const char* file );
	bool build( ciri::IGraphicsDevice* device );

	ciri::IVertexBuffer* getVertexBuffer() const;
	ciri::IIndexBuffer* getIndexBuffer() const;
	Transform& getXform();
	ciri::IShader* getShader() const;
	void setShader( ciri::IShader* val );

private:
	std::vector<Vertex> _vertices;
	std::vector<int> _indices;
	ciri::IVertexBuffer* _vertexBuffer;
	ciri::IIndexBuffer* _indexBuffer;
	Transform _xform;
	ciri::IShader* _shader;
};

#endif /* __test_model__ */