#include "GridlrGrid.hpp"
#include <cstdio>

GridlrGrid::GridlrGrid()
	: _width(0), _height(0), _cells(nullptr) {
}

GridlrGrid::~GridlrGrid() {
	if( _cells != nullptr ) {
		delete[] _cells;
		_cells = nullptr;
	}
}

int GridlrGrid::getWidth() const {
	return _width;
}

int GridlrGrid::getHeight() const {
	return _height;
}

bool GridlrGrid::create( int width, int height ) {
	// fail on bad size or existing grid
	if( width <= 0 || height <= 0 || _cells != nullptr ) {
		return false;
	}

	// store size
	_width = width;
	_height = height;

	// initialize grid
	_cells = new Cell[width * height];
	for( int y = 0; y < height; ++y ) {
		for( int x = 0; x < width; ++x ) {
			auto c = getCell(x, y);
			c->setX(x);
			c->setY(y);
		}
	}
	return true;
}

Cell* GridlrGrid::getCell( int x, int y ) const {
	if( x < 0 || x >= _width || y < 0 || y >= _height ) {
		return nullptr;
	}
	return &_cells[getIndex(x, y)];
}

Cell* GridlrGrid::getNorthOf( const Cell& cell ) const {
	return getCell(cell.x(), cell.y()-1);
}

Cell* GridlrGrid::getSouthOf( const Cell& cell ) const {
	return getCell(cell.x(), cell.y()+1);
}

Cell* GridlrGrid::getEastOf( const Cell& cell ) const {
	return getCell(cell.x()+1, cell.y());
}

Cell* GridlrGrid::getWestOf( const Cell& cell ) const {
	return getCell(cell.x()-1, cell.y());
}

Cell* GridlrGrid::getSource( CellState state ) const {
	for( int i = 0; i < _width * _height; ++i ) {
		auto& c = _cells[i];
		if( c.isSource() && c.state() == state ) {
			return &_cells[i];
		}
	}
	return nullptr;
}

Cell* GridlrGrid::getEnd( CellState state ) const {
	for( int i = 0; i < _width * _height; ++i ) {
		auto& c = _cells[i];
		if( c.isEnd() && c.state() == state ) {
			return &_cells[i];
		}
	}
	return nullptr;
}

void GridlrGrid::clearAllOf( CellState state ) {
	for( int i = 0; i < _width * _height; ++i ) {
		auto& c = _cells[i];
		if( !c.isSource() && !c.isEnd() && c.state() == state ) {
			c.setState(CellState::Empty);
		}
	}
}

bool GridlrGrid::truncate( const Cell& cell, bool fromStart ) {
	if( CellState::Empty == cell.state() ) {
		return false;
	}

	auto to = fromStart ? getEnd(cell.state()) : getSource(cell.state());
	if( nullptr == to ) {
		return false;
	}

	truncateInternal(to, &cell, nullptr);
	return true;
}

void GridlrGrid::truncateInternal( const Cell* cell, const Cell* endpoint, const Cell* last ) {
	// if done
	if( cell == endpoint ) {
		return;
	}

	// get north cell
	auto north = getNorthOf(*cell);
	// if not null, not last cell, not endpoint, same type as cell, and not source/end...
	if( (north != nullptr) && (north != last) && (north != endpoint) && (north->state() == cell->state()) && (!north->isSource()) && (!north->isEnd()) ) {
		// repeat for north
		truncateInternal(north, endpoint, cell);
		// change to empty cell
		north->setState(CellState::Empty);
	}

	// south
	auto south = getSouthOf(*cell);
	if( (south != nullptr) && (south != last) && (south != endpoint) && (south->state() == cell->state()) && (!south->isSource()) && (!south->isEnd()) ) {
		truncateInternal(south, endpoint, cell);
		south->setState(CellState::Empty);
	}

	// east
	auto east = getEastOf(*cell);
	if( (east != nullptr) && (east != last) && (east != endpoint) && (east->state() == cell->state()) && (!east->isSource()) && (!east->isEnd()) ) {
		truncateInternal(east, endpoint, cell);
		east->setState(CellState::Empty);
	}

	// west
	auto west = getWestOf(*cell);
	if( (west != nullptr) && (west != last) && (west != endpoint) && (west->state() == cell->state()) && (!west->isSource()) && (!west->isEnd()) ) {
		truncateInternal(west, endpoint, cell);
		west->setState(CellState::Empty);
	}
}

void GridlrGrid::print() const {
	for( int y = 0; y < _height; ++y ) {
		for( int x = 0; x < _width; ++x ) {
			auto cell = getCell(x, y);
			char c;
			switch( cell->state() ) {
				case CellState::Empty: { c = ' '; break; }
				case CellState::Red:   { c = (cell->isSource() || cell->isEnd()) ? 'R' : 'r'; break; }
				case CellState::Green: { c = (cell->isSource() || cell->isEnd()) ? 'G' : 'g'; break; }
				case CellState::Blue:  { c = (cell->isSource() || cell->isEnd()) ? 'B' : 'b'; break; }
			}
			printf("[%c]", c);
		}
		printf("\n");
	}
}

int GridlrGrid::getIndex( int x, int y ) const {
	return y * _width + x;
}