#ifndef __CC_MATH_RANDOM__
#define __CC_MATH_RANDOM__

#include <random>

namespace cc {
	namespace math {
		template <class RealType, class IntType>
		class Random {
		public:
			Random() {
				std::random_device dev;
				_mt = std::mt19937(dev());
			}
			Random( unsigned int seed ) {
				_mt = std::mt19937(seed);
			}
			~Random() {
			}

			/**
			 * Returns a uniformly random RealType in the default range of std::uniform_real_distribution<RealType>.
			 */
			RealType nextReal() {
				std::uniform_real_distribution<RealType> dist;
				return dist(_mt);
			}

			/**
			 * Returns a uniformly random ranged RealType.
			 * Returns a ranged random RealType.
			 * @param[in] min  Minimum possible RealType.
			 * @param[in] max Maximum possible RealType.
			 * @returns RealType in the range [min,max).
			 */
			RealType nextReal( const RealType min, const RealType max ) {
				std::uniform_real_distribution<RealType> dist(min, max);
				return dist(_mt);
			}

			/**
			 * Returns a uniformly random IntType in the default range of std::uniform_int_distribution<IntType>.
			 */
			IntType nextInt() {
				std::uniform_int_distribution<IntType> dist;
				return dist(_mt);
			}

			/**
			 * Returns a uniformly random ranged IntType.
			 * @param[in] min Minimum possible IntType.
			 * @param[in] max Maximum possible IntType.
			 * @returns RealType in the range [min, max].
			 */
			IntType nextInt( const IntType min, const IntType max ) {
				std::uniform_int_distribution<IntType> dist(min, max);
				return dist(_mt);
			}

			/**
			 * Same as Random::nextReal(min, max), but static.
			 */
			static RealType rangedReal( const RealType min, const RealType max ) {
				std::random_device dev;
				std::mt19937 mt(dev());
				std::uniform_real_distribution<RealType> dist(min, max);
				return dist(mt);
			}

			/**
			 * Same as Random::nextInt(min, max), but static.
			 */
			static IntType rangedInt( const IntType min, const IntType max ) {
				std::random_device dev;
				std::mt19937 mt(dev());
				std::uniform_int_distribution<IntType> dist(min, max);
				return dist(mt);
			}

		private:
			std::mt19937 _mt;
		};
	}
}

#endif /* __CC_MATH_RANDOM__ */
