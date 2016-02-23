#ifndef __kscene__
#define __kscene__

#include <string>
#include <vector>
#include <unordered_map>
#include "Vertex.hpp"
#include <cc/Quaternion.hpp>
#include <cc/Mat4.hpp>

class KScene
{
public:
	class Mesh
	{
		friend class KScene;

	public:
		Mesh();

		const std::string& getName() const;
		const std::vector<Vertex>& getVertices() const;
		const std::vector<unsigned int>& getIndices() const;

	private:
		std::string _name;
		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indices;
	};

	class Xform
	{
		friend class KScene;

	public:
		Xform();

		const std::string& getName() const;
		const cc::Vec3f& getPosition( bool worldSpace=false ) const;
		const cc::Quatf& getOrientation( bool worldSpace=false ) const;
		const cc::Vec3f& getScale( bool worldSpace=false ) const;
		cc::Mat4f getMatrix( bool worldSpace=false ) const;
		const Mesh* getMesh() const;
		const std::vector<Xform*>& getChildren() const;

	private:
		void decomposeMatrix();

		std::string _name;
		cc::Mat4f _matrix;
		cc::Vec3f _localPosition;
		cc::Quatf _localOrient;
		cc::Vec3f _localScale;
		cc::Vec3f _worldPosition;
		cc::Quatf _worldOrient;
		cc::Vec3f _worldScale;
		Xform* _parent;
		Mesh* _mesh;
		std::vector<Xform*> _children;
	};

	class Light
	{
		friend class KScene;

	public:
		enum Type
		{
			kLightTypeInvalid = 0,
			kLightTypeAmbient = 1,
			kLightTypeDirectional = 2,
			kLightTypePoint = 3,
			kLightTypeSpot = 4
		};

	public:
		Light();

		const std::string& getName() const;
		Xform* getParent() const;
		Type getType() const;
		const cc::Vec4f& getColor() const;
		float getIntensity() const;
		float getRange() const;
		float getInnerConeAngle() const;
		float getOuterConeAngle() const;

	private:
		std::string _name;
		Xform* _parent;
		Type _type;
		cc::Vec4f _color;
		float _intensity;
		float _range;
		float _innerConeAngle;
		float _outerConeAngle;
	};

public:
	KScene();
	~KScene();

	const std::vector<Mesh*>& getMeshes() const;
	const std::vector<Xform*>& getXforms() const;
	const std::vector<Light*>& getLights() const;
	Xform* getRoot() const;
	Xform* getXformByName( const std::string& name );
	Mesh* getMeshByName( const std::string& name );

	bool readBinaryFile( const char* file );
	void printDebugInfo( bool verbose );
	void clean();

private:
	void readModelData( std::ifstream& is );
	void readXformData( std::ifstream& is );
	void readLightData( std::ifstream& is );
	void buildTree();
	void printXform( Xform* xform, int spacing, bool verbose );

private:
	std::vector<Mesh*> _meshes;
	std::vector<Xform*> _xforms;
	std::unordered_map<int, Mesh*> _meshIds;
	std::unordered_map<int, Xform*> _xformIds;
	std::unordered_map<Xform*, std::vector<int>> _xformChildIds;
	std::unordered_map<std::string, Xform*> _xformNames;
	std::unordered_map<std::string, Mesh*> _meshNames;
	std::vector<Light*> _lights;
	std::unordered_map<int, Light*> _lightIds;
	std::unordered_map<std::string, Light*> _lightNames;
	Xform* _root;
};

#endif /*  */