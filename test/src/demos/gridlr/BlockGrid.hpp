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

		/**
		 * Call when starting to drag to configure internal states.
		 */
		bool startDrag( const Block& block );

		/**
		 * Call when already dragging to update blocks.
		 */
		bool dragMove( Block& block );

		/**
		 * Call when stopping dragging to clear internal states.
		 */
		bool stopDrag( const Block& block );

		bool isValidMove( const Block& from, const Block& to ) const;

		void print();

		bool isComplete() const;

	private:
		int getIndex( int x, int y ) const;
		BlockChain* getChain( BlockState state );

	private:
		int _width;
		int _height;
		Block* _blocks;
		std::unordered_map<BlockState, BlockChain> _chains;
		//
		bool _isDragging;
		BlockState _dragState;
	};
}

#endif /* __gridlr_block_grid__ */