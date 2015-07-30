#ifndef __ciri_clearflags__
#define __ciri_clearflags__

namespace ciri {
	class ClearFlags {
	public:
		enum Flags {
			Color					=	(1 << 0),
			Depth					=	(1 << 1),
			Stencil				=	(1 << 2),
			ColorDepth		=	(1 << 3),
			ColorStencil	=	(1 << 4),
			DepthStencil	=	(1 << 5),
			All						=	(1 << 6)
		};
	};
} // ciri

#endif /* __ciri_clearflags__ */