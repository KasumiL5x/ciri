#ifndef __ciri_common__
#define __ciri_common__

namespace ciri {
	#define	CIRI_QUOTE_ME(x) #x

	#if defined(WIN)
	  // builds include path for windows files
		#define CIRI_INCLUDE_PS(file) CIRI_QUOTE_ME(win/file.hpp)
		#if defined(DX)
			// builds include path for windows directx files
			#define CIRI_INCLUDE_GFX_PS(file) CIRI_QUOTE_ME(win/dx/file.hpp)
		#elif defined(GL)
			// builds include path for windows opengl files
			#define CIRI_INCLUDE_GFX_PS(file) CIRI_QUOTE_ME(win/gl/file.hpp)
		#else
			#error Graphics library required!
		#endif
	#else
		#error Platform required!  Duh!
	#endif
} // ciri

#endif /* __ciri_common__ */