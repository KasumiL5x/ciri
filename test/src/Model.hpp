#ifndef __test_model__
#define __test_model__

#include <vector>
#include "Vertex.hpp"
#include <ciri/gfx/IGraphicsDevice.hpp>

class Model {
public:
	Model();
	~Model();

	void addVertex( const Vertex& vertex );
	void addIndex( int index );
	bool build( ciri::IGraphicsDevice* device );

	ciri::IVertexBuffer* getVertexBuffer() const;
	ciri::IIndexBuffer* getIndexBuffer() const;

private:
	std::vector<Vertex> _vertices;
	std::vector<int> _indices;
	ciri::IVertexBuffer* _vertexBuffer;
	ciri::IIndexBuffer* _indexBuffer;
};

#endif /* __test_model__ */