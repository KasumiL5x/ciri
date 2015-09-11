#include "HeightmapTerrain.hpp"

HeightmapTerrain::HeightmapTerrain()
	: _generated(false), _heightData(nullptr), _vertices(nullptr), _indices(nullptr), _vertexBuffer(nullptr), _indexBuffer(nullptr),
		_shader(nullptr), _perFrameConstantBuffer(nullptr), _sampler(nullptr) {
	_textures[0] = _textures[1] = _textures[2] = _textures[3] = nullptr;
}

HeightmapTerrain::~HeightmapTerrain() {
}

bool HeightmapTerrain::generate( const ciri::TGA& heightmap, std::shared_ptr<ciri::IGraphicsDevice> device ) {
	if( _generated ) {
		return false;
	}

	// get pixels from heightmap
	const unsigned char* pixels = heightmap.getPixels();
	if( nullptr == pixels ) {
		return false;
	}

	// get heightmap size
	const int width = heightmap.getWidth();
	const int height = heightmap.getHeight();
	if( width <= 0 || height <= 0 ) {
		return false;
	}
	float minHeight = std::numeric_limits<float>::max();
	float maxHeight = std::numeric_limits<float>::min();
	// create height data array
	const int BPP = (ciri::TGA::RGB == heightmap.getFormat()) ? 3 : 4;
	_heightData = new float[width * height];
	for( int x = 0; x < width; ++x ) {
		for( int y = 0; y < height; ++y ) {
			// get height value from heightmap image
			_heightData[y * width + x] = pixels[(y * width + x) * BPP]; // red component

			// update min and max heights
			minHeight = (_heightData[y * width + x] < minHeight) ? _heightData[y * width + x] : minHeight;
			maxHeight = (_heightData[y * width + x] > maxHeight) ? _heightData[y * width + x] : maxHeight;
		}
	}
	// normalize heights
	const float SCALE = 200.0f;
	const float INV_SCALE = 1.0f / SCALE;
	for( int x = 0; x < width; ++x ) {
		for( int y = 0; y < height; ++y ) {
			_heightData[y * width + x] = (_heightData[y * width + x] - minHeight) / (maxHeight - minHeight) * SCALE;
		}
	}

	// smooth heightmap
	boxFilterHeightData(width, height, _heightData, true);

	// create vertices
	const float halfWidth = static_cast<float>(width) * 0.5f;
	const float invWidth = 1.0f / static_cast<float>(width);
	const float halfHeight = static_cast<float>(-height) * 0.5f;
	const float invHeight =  1.0f / static_cast<float>(-height);
	const int vertexCount = width * height;
	_vertices = new TerrainVertex[vertexCount];
	for( int x = 0; x < width; ++x ) {
		for( int y = 0; y < height; ++y ) {
			const float fx = static_cast<float>(x);
			const float fy = static_cast<float>(-y);

			// position of vertex w/ the center of the terrain as the origin
			const float xpos = ((fx * invWidth) * 2.0f - 1.0f) * halfWidth;
			const float ypos = _heightData[y * width + x];
			const float zpos = ((fy * invHeight) * 2.0f - 1.0f) * halfHeight;
			_vertices[y * width + x].position = cc::Vec3f(xpos, ypos, zpos);

			// texcoords
			_vertices[y * width + x].texcoord.x = fx * INV_SCALE;
			_vertices[y * width + x].texcoord.y = fy * INV_SCALE;

			// texture weighting
			_vertices[y * width + x].texweights.x = cc::math::clamp(1.0f - fabsf(_heightData[y * width + x] - 0.0f) / (SCALE*0.25f), 0.0f, 1.0f);
			_vertices[y * width + x].texweights.y = cc::math::clamp(1.0f - fabsf(_heightData[y * width + x] - (SCALE*0.33f)) / (SCALE*0.2f), 0.0f, 1.0f);
			_vertices[y * width + x].texweights.z = cc::math::clamp(1.0f - fabsf(_heightData[y * width + x] - (SCALE*0.66f)) / (SCALE*0.2f), 0.0f, 1.0f);
			_vertices[y * width + x].texweights.w = cc::math::clamp(1.0f - fabsf(_heightData[y * width + x] - SCALE) / (SCALE*0.2f), 0.0f, 1.0f);
			float totalWeighting = _vertices[y * width + x].texweights.x;
			totalWeighting += _vertices[y * width + x].texweights.y;
			totalWeighting += _vertices[y * width + x].texweights.z;
			totalWeighting += _vertices[y * width + x].texweights.w;
			_vertices[y * width + x].texweights.x /= totalWeighting;
			_vertices[y * width + x].texweights.y /= totalWeighting;
			_vertices[y * width + x].texweights.z /= totalWeighting;
			_vertices[y * width + x].texweights.w /= totalWeighting;

			// for a corner as the origin, use:
			//_vertices[y * width + x].position = cc::Vec3f(static_cast<float>(x), _heightData[y * width + x], static_cast<float>(-y));

			// todo: weights for multitexture
		}
	}
	
	// create indices
	const int indexCount = (width-1) * (height-1) * 6;
	_indices = new int[indexCount];
	int counter = 0;
	for( int y = 0; y < (height-1); ++y ) {
		for( int x = 0; x < (width-1); ++x ) {
			const int lowerLeft = y * width + x;
			const int lowerRight = y * width + (x + 1);
			const int topLeft = (y + 1) * width + x;
			const int topRight = (y + 1) * width + (x + 1);

			// CCW
			_indices[counter++] = topLeft;
			_indices[counter++] = lowerLeft;
			_indices[counter++] = lowerRight;
			//
			_indices[counter++] = topLeft;
			_indices[counter++] = lowerRight;
			_indices[counter++] = topRight;

			// CW
			//_indices[counter++] = topLeft;
			//_indices[counter++] = lowerRight;
			//_indices[counter++] = lowerLeft;
			//
			//_indices[counter++] = topLeft;
			//_indices[counter++] = topRight;
			//_indices[counter++] = lowerRight;
		}
	}

	// calculate vertex normals
	for( int i = 0; i < (indexCount / 3); ++i ) {
		const int index1 = _indices[i * 3];
		const int index2 = _indices[i * 3 + 1];
		const int index3 = _indices[i * 3 + 2];

		const cc::Vec3f side1 = _vertices[index1].position - _vertices[index3].position;
		const cc::Vec3f side2 = _vertices[index1].position - _vertices[index2].position;

		// CCW
		const cc::Vec3f normal = side2.cross(side1);

		// CW
		//const cc::Vec3f normal = side1.cross(side2);

		_vertices[index1].normal += normal;
		_vertices[index2].normal += normal;
		_vertices[index3].normal += normal;
	}
	// normalize vertex normals
	for( int i = 0; i < vertexCount; ++i ) {
		_vertices[i].normal.normalize();
	}

	// create gpu buffers
	_vertexBuffer = device->createVertexBuffer();
	_vertexBuffer->set(_vertices, sizeof(TerrainVertex), vertexCount, false);
	_indexBuffer = device->createIndexBuffer();
	_indexBuffer->set(_indices, indexCount, false);


	// create the shader
	_shader = device->createShader();
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Normal, 0));
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float4, ciri::VertexUsage::Tangent, 0));
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float2, ciri::VertexUsage::Texcoord, 0));
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float4, ciri::VertexUsage::Texcoord, 1)); // wrong ?
	const std::string vsStr = (ciri::GraphicsApiType::OpenGL==device->getApiType()) ? getVertexShaderGl() : getVertexShaderDx();
	const std::string psStr = (ciri::GraphicsApiType::OpenGL==device->getApiType()) ? getPixelShaderGl() : getPixelShaderDx();
	if( ciri::failed(_shader->loadFromMemory(vsStr.c_str(), nullptr, psStr.c_str())) ) {
		for( auto err : _shader->getErrors() ) {
			printf("%s\n", err.msg.c_str());
		}
		clean();
		return false;
	}

	// create constant buffers
	_perFrameConstantBuffer = device->createConstantBuffer();
	_perFrameConstantBuffer->setData(sizeof(PerFrameConstants), &_perFrameConstants);

	// assign constant buffers to shader
	_shader->addConstants(_perFrameConstantBuffer, "PerFrameConstants", ciri::ShaderStage::Vertex);

	// create the sampler
	ciri::SamplerDesc samplerDesc;
	samplerDesc.filter = ciri::SamplerFilter::Linear;
	_sampler = device->createSamplerState(samplerDesc);

	_generated = true;

	return true;
}

void HeightmapTerrain::setTextures( ciri::ITexture2D* tex0, ciri::ITexture2D* tex1, ciri::ITexture2D* tex2, ciri::ITexture2D* tex3 ) {
	_textures[0] = tex0;
	_textures[1] = tex1;
	_textures[2] = tex2;
	_textures[3] = tex3;
}

void HeightmapTerrain::setClippingPlaneActive( bool active ) {
	_perFrameConstants.shouldClip = (active) ? 1 : 0;
}

void HeightmapTerrain::setClippingPlaneParams( float height, const cc::Mat4f& viewProj, bool flip ) {
	ciri::Plane plane = createClippingPlane(height, cc::Vec3f(0, -1, 0), viewProj, flip);
	_perFrameConstants.clippingPlane = cc::Vec4f(plane.getNormal(), plane.getD());
}

void HeightmapTerrain::draw( const cc::Mat4f& viewProj, std::shared_ptr<ciri::IGraphicsDevice> device ) {
	if( !_generated ) {
		return;
	}

	// check valid buffers
	if( nullptr == _vertexBuffer || nullptr == _indexBuffer ) {
		return;
	}

	// check valid textures
	if( nullptr == _textures[0] || nullptr == _textures[1] || nullptr == _textures[2] || nullptr == _textures[3] ) {
		return;
	}

	// check valid sampler
	if( nullptr == _sampler ) {
		return;
	}

	// check valid shader
	if( nullptr == _shader || !_shader->isValid() ) {
		return;
	}

	// apply shader
	device->applyShader(_shader);

	// apply samplers
	device->setSamplerState(0, _sampler, ciri::ShaderStage::Pixel);
	device->setSamplerState(1, _sampler, ciri::ShaderStage::Pixel);
	device->setSamplerState(2, _sampler, ciri::ShaderStage::Pixel);
	device->setSamplerState(3, _sampler, ciri::ShaderStage::Pixel);

	// apply textures
	device->setTexture2D(0, _textures[0], ciri::ShaderStage::Pixel);
	device->setTexture2D(1, _textures[1], ciri::ShaderStage::Pixel);
	device->setTexture2D(2, _textures[2], ciri::ShaderStage::Pixel);
	device->setTexture2D(3, _textures[3], ciri::ShaderStage::Pixel);

	// update constant buffers
	updateConstants(cc::Mat4f(1.0f), viewProj);

	// set buffers and draw
	device->setVertexBuffer(_vertexBuffer);
	device->setIndexBuffer(_indexBuffer);
	device->drawIndexed(ciri::PrimitiveTopology::TriangleList, _indexBuffer->getIndexCount());
}

void HeightmapTerrain::clean() {
	if( !_generated ) {
		return;
	}

	if( _indices != nullptr ) {
		delete[] _indices;
		_indices = nullptr;
	}

	if( _vertices != nullptr ) {
		delete[] _vertices;
		_vertices = nullptr;
	}

	if( _heightData != nullptr ) {
		delete[] _heightData;
		_heightData = nullptr;
	}

	_generated = false;
}

void HeightmapTerrain::updateConstants( const cc::Mat4f& world, const cc::Mat4f& viewProj ) {
	_perFrameConstants.world = world;
	_perFrameConstants.xform = viewProj * _perFrameConstants.world;
	_perFrameConstantBuffer->setData(sizeof(PerFrameConstants), &_perFrameConstants);
}

void HeightmapTerrain::boxFilterHeightData( unsigned long width, unsigned long height, float*& heightMap, bool smoothEdges ) {
	// http://archive.gamedev.net/archive/reference/articles/article2164.html
	// i did not write this, and i do not dare touch it

  // Temporary values for traversing single dimensional arrays
  long x = 0;
  long z = 0;
  
  long  widthClamp = (smoothEdges) ?  width : width  - 1;
  long heightClamp = (smoothEdges) ? height : height - 1;
  
  // [Optimization] Calculate bounds ahead of time
  unsigned int bounds = width * height;
  
  // Validate requirements
  if (!heightMap)
    return;
  
  // Allocate the result
  float* result = new float[bounds];
  
  // Make sure memory was allocated
  if (!result)
    return;
  
  for (z = (smoothEdges) ? 0 : 1; z < heightClamp; ++z)
  {
    for (x = (smoothEdges) ? 0 : 1; x < widthClamp; ++x)
    {
      // Sample a 3x3 filtering grid based on surrounding neighbors
      
      float value = 0.0f;
      float cellAverage = 1.0f;
      
      // Sample top row
      
      if (((x - 1) + (z - 1) * width) >= 0 &&
          ((x - 1) + (z - 1) * width) < bounds)
      {
        value += heightMap[(x - 1) + (z - 1) * width];
        ++cellAverage;
      }
      
      if (((x - 0) + (z - 1) * width) >= 0 &&
          ((x - 0) + (z - 1) * width) < bounds)
      {
        value += heightMap[(x    ) + (z - 1) * width];
        ++cellAverage;
      }
      
      if (((x + 1) + (z - 1) * width) >= 0 &&
          ((x + 1) + (z - 1) * width) < bounds)
      {
        value += heightMap[(x + 1) + (z - 1) * width];
        ++cellAverage;
      }
      
      // Sample middle row
      
      if (((x - 1) + (z - 0) * width) >= 0 &&
          ((x - 1) + (z - 0) * width) < bounds)
      {
        value += heightMap[(x - 1) + (z    ) * width];
        ++cellAverage;
      }
      
      // Sample center point (will always be in bounds)
      value += heightMap[x + z * width];
      
      if (((x + 1) + (z - 0) * width) >= 0 &&
          ((x + 1) + (z - 0) * width) < bounds)
      {
        value += heightMap[(x + 1) + (z    ) * width];
        ++cellAverage;
      }
      
      // Sample bottom row
      
      if (((x - 1) + (z + 1) * width) >= 0 &&
          ((x - 1) + (z + 1) * width) < bounds)
      {
        value += heightMap[(x - 1) + (z + 1) * width];
        ++cellAverage;
      }
      
      if (((x - 0) + (z + 1) * width) >= 0 &&
          ((x - 0) + (z + 1) * width) < bounds)
      {
        value += heightMap[(x    ) + (z + 1) * width];
        ++cellAverage;
      }
      
      if (((x + 1) + (z + 1) * width) >= 0 &&
          ((x + 1) + (z + 1) * width) < bounds)
      {
        value += heightMap[(x + 1) + (z + 1) * width];
        ++cellAverage;
      }
      
      // Store the result
      result[x + z * width] = value / cellAverage;
    }
  }
  
  // Release the old array
  delete [] heightMap;
  
  // Store the new one
  heightMap = result;
}

std::string HeightmapTerrain::getVertexShaderGl() const {
	return
		"#version 420\n"
		""
		"layout (location = 0) in vec3 in_position;\n"
		"layout (location = 1) in vec3 in_normal;\n"
		"layout (location = 2) in vec4 in_tangent;\n"
		"layout (location = 3) in vec2 in_texcoord;\n"
		"layout (location = 4) in vec4 in_texweights;\n"
		""
		"layout (std140) uniform PerFrameConstants {\n"
		"	mat4 world;\n"
		"	mat4 xform;\n"
		"	vec4 clippingPlane;\n"
		"	int shouldClip;\n"
		"};\n"
		""
		"out vec3 vo_position;\n"
		"out vec3 vo_normal;\n"
		"out vec2 vo_texcoord;\n"
		"out vec4 vo_texweights;\n"
		"out float vo_clippingPlane;\n"
		"flat out int vo_shouldClip;\n"
		""
		"void main() {\n"
		"	gl_Position = xform * vec4(in_position, 1.0f);\n"
		"	vo_position = (world * vec4(in_position, 1.0f)).xyz;\n"
		"	vo_normal = (world * vec4(in_normal, 0.0f)).xyz;\n"
		"	vo_texcoord = in_texcoord;\n"
		"	vo_texweights = in_texweights;\n"
		"	vo_clippingPlane = dot(gl_Position, clippingPlane);\n"
		"	vo_shouldClip = shouldClip;\n"
		"}";
}

std::string HeightmapTerrain::getVertexShaderDx() const {
	return
		"cbuffer PerFrameConstants : register(b0) {\n"
		"	float4x4 world;\n"
		"	float4x4 xform;\n"
		"	float4 clippingPlane;\n"
		"	int shouldClip;\n"
		"};\n"
		""
		"struct Output {\n"
		"	float4 hpos : SV_POSITION;\n"
		"	float3 wpos : TEXCOORD2;\n"
		"	float3 nrm : NORMAL0;\n"
		"	float2 tex : TEXCOORD0;\n"
		"	float4 weights : TEXCOORD1;\n"
		"	float clipping : TEXCOORD3;\n"
		"	int shouldClip : TEXCOORD4;\n"
		"};\n"
		""
		"Output main( float3 pos : POSITION, float3 nrm : NORMAL, float4 tan : TANGENT, float2 tex : TEXCOORD0, float4 weights : TEXCOORD1 ) {\n"
		"	Output OUT;\n"
		"	OUT.hpos = mul(xform, float4(pos, 1.0f));\n"
		"	OUT.wpos = mul(world, float4(pos, 1.0f)).xyz;\n"
		"	OUT.nrm = mul(world, float4(nrm, 0.0f)).xyz;\n"
		"	OUT.tex = tex;\n"
		"	OUT.weights = weights;\n"
		"	OUT.clipping = dot(OUT.hpos, clippingPlane);\n"
		"	OUT.shouldClip = shouldClip;\n"
		"	return OUT;\n"
		"}";
}

std::string HeightmapTerrain::getPixelShaderGl() const {
	return
		"#version 420\n"
		""
		"in vec3 vo_position;\n"
		"in vec3 vo_normal;\n"
		"in vec2 vo_texcoord;\n"
		"in vec4 vo_texweights;\n"
		"in float vo_clippingPlane;\n"
		"flat in int vo_shouldClip;\n"
		""
		"out vec4 out_color;\n"
		""
		"layout(binding=0) uniform sampler2D Texture0;\n"
		"layout(binding=1) uniform sampler2D Texture1;\n"
		"layout(binding=2) uniform sampler2D Texture2;\n"
		"layout(binding=3) uniform sampler2D Texture3;\n"
		""
		"vec3 AmbientLightColor = vec3(0.05333332f, 0.09882354f, 0.1819608f);\n"
		"vec3 LightDirection = vec3(-0.5265408f, -0.5735765f,-0.6275069f);\n"
		"vec3 LightColor = vec3(1.0f, 0.9607844f, 0.8078432f);\n"
		""
		"vec3 lambert( vec3 L, vec3 N, vec3 lightColor, float lightIntensity ) {\n"
		"	return (max(dot(L, N), 0.0f) * lightColor) * lightIntensity;\n"
		"}\n"
		""
		"void main() {\n"
		" if( (vo_shouldClip != 0) && (vo_clippingPlane < 0.0f) ) {\n"
		"		discard;\n"
		"	}\n"
		""
		"	vec3 L = -LightDirection;\n"
		"	vec3 N = normalize(vo_normal);\n"
		"	vec3 lighting = AmbientLightColor + lambert(L, N, LightColor, 1.0f);\n"
		""
		"	// sample weighted textures\n"
		"	vec4 color = texture2D(Texture0, vo_texcoord) * vo_texweights.x;\n"
		"	color += texture2D(Texture1, vo_texcoord) * vo_texweights.y;\n"
		"	color += texture2D(Texture2, vo_texcoord) * vo_texweights.z;\n"
		"	color += texture2D(Texture3, vo_texcoord) * vo_texweights.w;\n"
		""
		" // sampling with colors\n"
		"	//vec3 color = vec3(1,0,1)*vo_texweights.x;\n"
		"	//color += vec3(1,0,0)*vo_texweights.y;\n"
		"	//color += vec3(0,1,0)*vo_texweights.z;\n"
		"	//color += vec3(0,0,1)*vo_texweights.w;\n"
		""
		"	out_color = vec4(color.xyz * lighting, 1.0f);\n"
		"	//out_color = vec4(color.xyz, 1.0f);\n"
		"}";
}

std::string HeightmapTerrain::getPixelShaderDx() const {
	return
		"Texture2D Texture0 : register(t0);\n"
		"Texture2D Texture1 : register(t1);\n"
		"Texture2D Texture2 : register(t2);\n"
		"Texture2D Texture3 : register(t3);\n"
		"SamplerState Sampler0 : register(s0);\n"
		"SamplerState Sampler1 : register(s1);\n"
		"SamplerState Sampler2 : register(s2);\n"
		"SamplerState Sampler3 : register(s3);\n"
		""
		"struct Input {\n"
		"	float4 hpos : SV_POSITION;\n"
		"	float3 wpos : TEXCOORD2;\n"
		"	float3 nrm : NORMAL0;\n"
		"	float2 tex : TEXCOORD0;\n"
		"	float4 weights : TEXCOORD1;\n"
		"	float clipping : TEXCOORD3;\n"
		"	int shouldClip : TEXCOORD4;\n"
		"};\n"
		""
		"static float3 AmbientLightColor = float3(0.05333332f, 0.09882354f, 0.1819608f);\n"
		"static float3 LightDirection = float3(-0.5265408f, -0.5735765f,-0.6275069f);\n"
		"static float3 LightColor = float3(1.0f, 0.9607844f, 0.8078432f);\n"
		""
		"float3 lambert( float3 L, float3 N, float3 lightColor, float lightIntensity ) {\n"
		"	return (max(dot(L, N), 0.0f) * lightColor) * lightIntensity;\n"
		"}\n"
		""
		"float4 main( Input input ) : SV_Target {\n"
		"	if( input.shouldClip != 0 ) {\n"
		"		clip(input.clipping);\n"
		"	}\n"
		""
		"	float3 L = -LightDirection;\n"
		"	float3 N = normalize(input.nrm);\n"
		"	float3 lighting = AmbientLightColor + lambert(L, N, LightColor, 1.0f);\n"
		""
		"	// sample weighted textures\n"
		"	float4 color = Texture0.Sample(Sampler0, input.tex) * input.weights.x;\n"
		"	color += Texture1.Sample(Sampler1, input.tex) * input.weights.y;\n"
		"	color += Texture2.Sample(Sampler2, input.tex) * input.weights.z;\n"
		"	color += Texture3.Sample(Sampler3, input.tex) * input.weights.w;\n"
		""
		"	return float4(color.xyz * lighting, 1.0f);\n"
		"}";
}

ciri::Plane HeightmapTerrain::createClippingPlane( float height, const cc::Vec3f& normal, const cc::Mat4f& viewProj, bool flip ) const {
	cc::Vec4f coeffs = cc::Vec4f(normal, height) * (flip ? -1.0f : 1.0f);
	cc::Mat4f myViewProj = viewProj;
	myViewProj.invert();
	coeffs = (coeffs * myViewProj);
	return ciri::Plane(coeffs);
}
