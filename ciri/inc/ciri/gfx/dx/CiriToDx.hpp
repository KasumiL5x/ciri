#ifndef __ciri_ciritodx__
#define __ciri_ciritodx__

#include <d3d11.h>
#include "../CompareFunction.hpp"

namespace ciri {
	static D3D11_COMPARISON_FUNC ciriToDxFunc( CompareFunction::Function func ) {
		switch( func ) {
			case CompareFunction::Never: {
				return D3D11_COMPARISON_NEVER;
			}

			case CompareFunction::Always: {
				return D3D11_COMPARISON_ALWAYS;
			}

			case CompareFunction::Less: {
				return D3D11_COMPARISON_LESS;
			}

			case CompareFunction::Equal: {
				return D3D11_COMPARISON_EQUAL;
			}

			case CompareFunction::LessEqual: {
				return D3D11_COMPARISON_LESS_EQUAL;
			}

			case CompareFunction::Greater: {
				return D3D11_COMPARISON_GREATER;
			}

			case CompareFunction::GreaterEqual: {
				return D3D11_COMPARISON_GREATER_EQUAL;
			}

			case CompareFunction::NotEqual: {
				return D3D11_COMPARISON_NOT_EQUAL;
			}

			default: {
				return D3D11_COMPARISON_NEVER;
			}
		}
	}
} // ciri

#endif /* __ciri_ciritodx__ */