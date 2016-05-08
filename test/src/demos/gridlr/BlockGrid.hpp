#ifndef __gridlr_block_grid__
#define __gridlr_block_grid__

#include <unordered_map>
#include "Block.hpp"
#include "BlockChain.hpp"

namespace gridlr {
	class BlockGrid {
	public:
		BlockGrid();
		~BlockGrid();

		bool create( int width, int height );

		int width() const;
		int height() const;
		Block* get( int x, int y ) const;
		Block* getNorth( const Block& block ) const;
		Block* getSouth( const Block& block ) const;
		Block* getEast( const Block& block ) const;
		Block* getWest( const Block& block ) const;

		bool enableChain( BlockState state, Block& start, Block& end );
		bool disableChain( BlockState state );

		// void startDrag(...); => handles initial clicks and can prepare for dragging
		// void dragMove(...);  => handles cells when dragging is true
		// void stopDrag(...);  => handles post-drag state (clearing states etc.)


		void print();

	private:
		int getIndex( int x, int y ) const;

	private:
		int _width;
		int _height;
		Block* _blocks;
		std::unordered_map<BlockState, BlockChain> _chains;
	};
}

#endif /* __gridlr_block_grid__ */