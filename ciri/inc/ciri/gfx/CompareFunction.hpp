#ifndef __ciri_comparefunction__
#define __ciri_comparefunction__

namespace ciri {
	struct CompareFunction {
		/**
		 * Comparison functions.
		 */
		enum Function {
			Always,       /**< Always pass. */
			Never,        /**< Never pass. */
			Less,         /**< Pass if source data is less than destination data. */
			LessEqual,    /**< Pass if source data is less than or equal to the destination data. */
			Equal,        /**< Pass is the source data is equal to the destination data. */
			GreaterEqual, /**< Pass if the source data is greater than or equal to the destination data. */
			Greater,      /**< Pass if the source data is greater than the destination data. */
			NotEqual      /**< Pass if the source data is not equal to the destination data. */
		};
	};
} // ciri

#endif /* __ciri_comparefunction__ */