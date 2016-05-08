#include "BlockGrid.hpp"
#include <stdio.h>

using namespace gridlr;

BlockGrid::BlockGrid()
	: _width(0), _height(0), _blocks(nullptr) {
}

BlockGrid::~BlockGrid() {
	if( _blocks != nullptr ) {
		delete[] _blocks;
		_blocks = nullptr;
	}
}

bool BlockGrid::create( int width, int height ) {
	// fail on bad size or existing grid
	if( width <= 0 || height <= 0 || _blocks != nullptr ) {
		return false;
	}

	// store size
	_width = width;
	_height = height;

	// initialize grid
	_blocks = new Block[width * height];
	for( int y = 0; y < height; ++y ) {
		for( int x = 0; x < width; ++x ) {
			auto block = get(x, y);
			block->setX(x);
			block->setY(y);
		}
	}

	return true;
}

int BlockGrid::width() const {
	return _width;
}

int BlockGrid::height() const {
	return _height;
}

Block* BlockGrid::get( int x, int y ) const {
	if( x < 0 || x >= _width || y < 0 || y >= _height || nullptr == _blocks) {
		return nullptr;
	}
	return &_blocks[getIndex(x, y)];
}

Block* BlockGrid::getNorth( const Block& block ) const {
	return get(block.x(), block.y()-1);
}

Block* BlockGrid::getSouth( const Block& block ) const {
	return get(block.x(), block.y()+1);
}

Block* BlockGrid::getEast( const Block& block ) const {
	return get(block.x()+1, block.y());
}

Block* BlockGrid::getWest( const Block& block ) const {
	return get(block.x()-1, block.y());
}

bool BlockGrid::enableChain( BlockState state, Block& start, Block& end ) {
	// cannot be empty state
	if( BlockState::Empty == state ) {
		printf("BlockGrid::enableChain failed: [state] was Empty.\n");
		return false;
	}

	// fail if the chain already exists
	auto existing = _chains.find(state);
	if( existing != _chains.end() ) {
		printf("BlockGrid::enableChain failed: Chain of [state] already exists.\n");
		return false;
	}

	// start/end must not be a start/end already and for simplicity must be an empty state currently
	if( start.type() != BlockType::Normal || end.type() != BlockType::Normal || start.state() != BlockState::Empty || end.state() != BlockState::Empty ) {
		printf("BlockGrid::enableChain failed: A given block was invalid.\n");
		return false;
	}

	_chains[state] = BlockChain();
	if( !_chains[state].setup(state, &start, &end) ) {
		_chains.erase(state);
		printf("BlockGrid::enableChain failed: Could not set up chain.\n");
		return false;
	}
	start.setState(state);
	start.setType(BlockType::Start);
	end.setState(state);
	end.setType(BlockType::End);

	printf("BlockGrid::enableChain succeeded.\n");
	
	return true;
}

bool BlockGrid::disableChain( BlockState state ) {
	auto existing = _chains.find(state);
	if( _chains.end() == existing ) {
		return false;
	}

	// disable all blocks in the chain
	for( auto block : existing->second.chain() ) {
		block->setState(BlockState::Empty);
	}

	// erase from chain map
	_chains.erase(state);

	return true;
}

void BlockGrid::print() {
	for( int y = 0; y < _height; ++y ) {
		for( int x = 0; x < _width; ++x ) {
			auto block = get(x, y);
			char c;
			switch( block->state() ) {
				case BlockState::Empty: { c = ' '; break; }
				case BlockState::Zero:  { c = '0'; break; }
				case BlockState::One:   { c = '1'; break; }
				case BlockState::Two:   { c = '2'; break; }
				case BlockState::Three: { c = '3'; break; }
				case BlockState::Four:  { c = '4'; break; }
				case BlockState::Five:  { c = '5'; break; }
				case BlockState::Six:   { c = '6'; break; }
				case BlockState::Seven: { c = '7'; break; }
				case BlockState::Eight: { c = '8'; break; }
				case BlockState::Nine:  { c = '9'; break; }
			}
			printf("[%c]", c);
		}
		printf("\n");
	}
}

int BlockGrid::getIndex( int x, int y ) const {
	return y  * _width + x;
}