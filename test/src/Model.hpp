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
	bool build( ciri::IGraphicsDevice* device );

	ciri::IVertexBuffer* getVertexBuffer() const;
	ciri::IIndexBuffer* getIndexBuffer() const;

	Transform& getXform();

private:
	std::vector<Vertex> _vertices;
	std::vector<int> _indices;
	ciri::IVertexBuffer* _vertexBuffer;
	ciri::IIndexBuffer* _indexBuffer;
	Transform _xform;
};

#endif /* __test_model__ */