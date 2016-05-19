#ifndef __test_model__
#define __test_model__

#include <vector>
#include <ciri/Graphics.hpp>
#include "Vertex.hpp"
#include "Transform.hpp"

class Model {
private:
	struct EdgePair {
		int idx[2];
	};

public:
	struct Edge {
		int idx[2]; // a, b
		std::vector<int> faces;
	};

	struct Triangle {
		int idx[3];
		std::vector<int> edges;
	};

public:
	Model();
	~Model();
	Model( const Model& rhs );
	Model operator=( const Model& rhs );

	void addVertex( const Vertex& vertex );
	void addIndex( int index );
	bool addFromObj( const char* file, bool outputErrors=false );
	bool computeNormals();
	bool computeTangents();
	bool build( std::shared_ptr<ciri::IGraphicsDevice> device );
	bool updateBuffers( bool vertex, bool index );

	bool flipNormals( bool shouldUpdateBuffers );

	const std::shared_ptr<ciri::IVertexBuffer>& getVertexBuffer() const;
	const std::shared_ptr<ciri::IIndexBuffer>& getIndexBuffer() const;
	Transform& getXform();
	const std::shared_ptr<ciri::IShader>& getShader() const;
	void setShader( const std::shared_ptr<ciri::IShader>& val );
	void setDynamicity( bool vertex, bool index ); // only call before build

	bool isValid() const;

	std::vector<Vertex>& getVertices();
	std::vector<int>& getIndices();
	const std::vector<Triangle>& getTriangles();
	const std::vector<Edge>& getEdges();

	Model copy() const;

	bool parseExtendedData();

	bool exportToObj( const char* file );

private:
	std::vector<Vertex> _vertices;
	std::vector<int> _indices;
	std::shared_ptr<ciri::IVertexBuffer> _vertexBuffer;
	std::shared_ptr<ciri::IIndexBuffer> _indexBuffer;
	Transform _xform;
	std::shared_ptr<ciri::IShader> _shader;
	bool _dynamicVertex;
	bool _dynamicIndex;
	
	// extended:
	std::vector<Triangle> _triangles;
	std::vector<Edge> _edges;
};

#endif /* __test_model__ */