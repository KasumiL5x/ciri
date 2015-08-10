#ifndef __ciri_clearflags__
#define __ciri_clearflags__

namespace ciri {
	class ClearFlags {
	public:
		enum Flags {
			Color		=	(1 << 0),
			Depth		=	(1 << 1),
			Stencil	=	(1 << 2)
		};
	};
} // ciri

#endif /* __ciri_clearflags__ */