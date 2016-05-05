#ifndef __gridlr_grid__
#define __gridlr_grid__

#include <vector>
#include "Cell.hpp"

class GridlrGrid {
public:
	GridlrGrid();
	~GridlrGrid();

	int getWidth() const;
	int getHeight() const;
	bool create( int width, int height );
	Cell* getCell( int x, int y ) const;
	Cell* getNorthOf( const Cell& cell ) const;
	Cell* getSouthOf( const Cell& cell ) const;
	Cell* getEastOf( const Cell& cell ) const;
	Cell* getWestOf( const Cell& cell ) const;
	Cell* getSource( CellState state ) const;
	Cell* getEnd( CellState state ) const;
	void clearAllOf( CellState state );
	bool truncate( const Cell& cell, bool fromStart );
	void print() const;

private:
	void truncateInternal( const Cell* cell, const Cell* endpoint, const Cell* last );
	int getIndex( int x, int y ) const;

private:
	int _width;
	int _height;
	Cell* _cells;
};

#endif /* __gridlr_grid__ */