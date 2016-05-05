#ifndef __gridlr_cell__
#define __gridlr_cell__

#include "CellState.hpp"

class Cell {
public:
	Cell()
		: _x(-1), _y(-1), _state(CellState::Empty), _isSource(false), _isEnd(false) {
	}
	
	int x() const {
		return _x;
	}
	
	int y() const {
		return _y;
	}
	
	CellState state() const {
		return _state;
	}
	
	bool isSource() const {
		return _isSource;
	}
	
	bool isEnd() {
		return _isEnd;
	}
	
	void setX( int val ) {
		_x = val;
	}
	
	void setY( int val ) {
		_y = val;
	}
	
	void setState( CellState val ) {
		_state = val;
	}
	
	void setIsSource( bool val ) {
		_isSource = val;
	}
	
	void setIsEnd( bool val ) {
		_isEnd = val;
	}
	
private:
	int _x;
	int _y;
	CellState _state;
	bool _isSource;
	bool _isEnd;
};

#endif /* __gridlr_cell__ */