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
		bool contains( const Block* block ) const;
		void clear();
		void setLastActive( bool wasItHead );
		Block* setLastActive() const;
		void truncateTo( const Block* block );
		bool remove( Block* block );
		void add( Block* block );
		Block* getLastPlaced() const;
		bool complete() const;
		void setComplete( bool val );
		int length() const;

	private:
		BlockState _state;
		std::vector<Block*> _chain;
		bool _headLastActive;
		bool _isComplete;
	};
}

#endif /* __gridlr_block_chain__ */