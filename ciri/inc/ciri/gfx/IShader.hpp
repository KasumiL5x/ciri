#ifndef __ciri_ishader__
#define __ciri_ishader__

#include "VertexElement.hpp"
#include "../ErrorCodes.hpp"

namespace ciri {
	class IConstantBuffer;

	class IShader {
	public:
		IShader() {
		}
		virtual ~IShader() {
		}

		virtual void addVertexShader( const char* filename )=0;
		virtual void addGeometryShader( const char* filename )=0;
		virtual void addPixelShader( const char* filename )=0;
		virtual void addInputElement( const VertexElement& element )=0;
		virtual err::ErrorCode addConstants( IConstantBuffer* buffer, const char* name, int shaderTypeFlags )=0;
		virtual err::ErrorCode build()=0;
		virtual err::ErrorCode rebuild()=0;
		virtual void destroy()=0;
		virtual const char* getLastError() const=0;
		virtual bool isValid() const=0;
	};
} // ciri

#endif /* __ciri_ishader__ */