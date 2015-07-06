#ifndef __ciri_common__
#define __ciri_common__

namespace ciri {
	#define	CIRI_QUOTE_ME(x) #x

	#if defined WIN	
		#define CIRI_INCLUDE_PS(file) CIRI_QUOTE_ME(win/file.hpp)
	#else
		#error Platform required!  Duh!
	#endif
} // ciri

#endif /* __ciri_common__ */