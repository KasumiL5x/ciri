#include "BlockChain.hpp"
#include "Block.hpp"
#include <algorithm>

using namespace gridlr;

BlockChain::BlockChain()
	: _state(BlockState::Empty), _headLastActive(true), _isComplete(false) {
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

bool BlockChain::contains( const Block* block ) const {
	if( nullptr == block ) {
		return false;
	}

	for( auto b : _chain ) {
		if( *block == *b ) {
			return true;
		}
	}
	return false;
}

void BlockChain::clear() {
	if( _chain.size() <= 2 ) {
		return;
	}
	std::vector<Block*> toRemove;
	for( auto it : _chain ) {
		if( _chain.front() == it || _chain.back() == it ) {
			continue;
		}
		it->setState(BlockState::Empty);
		toRemove.push_back(it);
	}
	for( auto x : toRemove ) {
		_chain.erase(std::find(_chain.begin(), _chain.end(), x));
	}
	_isComplete = false;
}

void BlockChain::setLastActive( bool wasItHead ) {
	_headLastActive = wasItHead;
}

Block* BlockChain::getLastActive() const {
	return _headLastActive ? head() : tail();
}

void BlockChain::truncateTo( const Block* block ) {
	auto existing = std::find(_chain.begin(), _chain.end(), block);
	if( _chain.end() == existing) {
		return;
	}

	std::vector<Block*> toRemove;
	if( !_headLastActive ) {
		for( auto it = _chain.begin(); it != _chain.end(); ++it ) {
			if( (*it)->type() != BlockType::Normal ) {
				continue;
			}
			if( (*it) == block ) {
				break;
			}
			(*it)->setState(BlockState::Empty);
			toRemove.push_back((*it));
		}
	} else {
		for( auto it = _chain.rbegin(); it != _chain.rend(); ++it ) {
			if( (*it)->type() != BlockType::Normal ) {
				continue;
			}
			if( (*it) == block ) {
				break;
			}
			(*it)->setState(BlockState::Empty);
			toRemove.push_back((*it));
		}
	}
	for( auto x : toRemove ) {
		_chain.erase(std::find(_chain.begin(), _chain.end(), x));
	}
	//std::remove_if(_chain.begin(), _chain.end(), [](Block* it){
	//	return BlockState::Empty == it->state();
	//});

	_isComplete = false;
}

bool BlockChain::remove( Block* block ) {
	auto existing = std::find(_chain.begin(), _chain.end(), block);
	if( _chain.end() == existing ) {
		return false;
	}

	block->setState(BlockState::Empty);
	_chain.erase(existing);
	_isComplete = false;
	return true;
}

void BlockChain::add( Block* block ) {
	if( _headLastActive ) {
		_chain.insert(_chain.end()-1, block);
	} else {
		_chain.insert(_chain.begin()+1, block);
	}
}

Block* BlockChain::getLastPlaced() const {
	if( _chain.size() <= 2 ) {
		return nullptr;
	}
	auto start = _chain[_chain.size()-2];
	auto end = _chain[1];
	return _headLastActive ? start : end;
}

bool BlockChain::complete() const {
	return _isComplete;
}

void BlockChain::setComplete( bool val ) {
	_isComplete = val;
}

int BlockChain::length() const {
	return static_cast<int>(_chain.size());
}