#ifndef __gridlr_block_chain__
#define __gridlr_block_chain__

#include <vector>
#include "BlockState.hpp"

namespace gridlr {
	class Block;

	class BlockChain {
	public:
		BlockChain();

		bool setup( BlockState state, Block* start, Block* end );
		Block* head() const;
		Block* block( int index ) const;
		Block* tail() const;
		const std::vector<Block*>& chain() const;

	private:
		BlockState _state;
		std::vector<Block*> _chain;
	};
}

#endif /* __gridlr_block_chain__ */