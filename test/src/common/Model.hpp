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
	bool computeTangents();
	bool build( ciri::IGraphicsDevice* device );
	bool updateBuffers( bool vertex, bool index );

	bool flipNormals( bool shouldUpdateBuffers );

	ciri::IVertexBuffer* getVertexBuffer() const;
	ciri::IIndexBuffer* getIndexBuffer() const;
	Transform& getXform();
	ciri::IShader* getShader() const;
	void setShader( ciri::IShader* val );
	void setDynamicity( bool vertex, bool index ); // only call before build

	bool isValid() const;

	std::vector<Vertex>& getVertices();
	std::vector<int>& getIndices();

private:
	std::vector<Vertex> _vertices;
	std::vector<int> _indices;
	ciri::IVertexBuffer* _vertexBuffer;
	ciri::IIndexBuffer* _indexBuffer;
	Transform _xform;
	ciri::IShader* _shader;
	bool _dynamicVertex;
	bool _dynamicIndex;
};

#endif /* __test_model__ */