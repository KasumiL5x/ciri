#include "BlockChain.hpp"

using namespace gridlr;

BlockChain::BlockChain()
	: _state(BlockState::Empty) {
}

bool BlockChain::setup( BlockState state, Block* start, Block* end ) {
	if( nullptr == start || nullptr == end || BlockState::Empty == state || !_chain.empty() ) {
		return false;
	}

	_state = state;

	_chain.push_back(start);
	_chain.push_back(end);
	return true;
}

Block* BlockChain::head() const {
	return _chain.front();
}

Block* BlockChain::block( int index ) const {
	if( index < 0 || index >= static_cast<int>(_chain.size()) ) {
		return nullptr;
	}
	return _chain[index];
}

Block* BlockChain::tail() const {
	return _chain.back();
}

const std::vector<Block*>& BlockChain::chain() const {
	return _chain;
}