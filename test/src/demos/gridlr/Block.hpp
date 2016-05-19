#ifndef __gridlr_block__
#define __gridlr_block__

#include "BlockState.hpp"
#include "BlockType.hpp"

namespace gridlr {
	class Block {
	public:
		Block()
			: _id(-1), _x(-1), _y(-1), _state(BlockState::Empty), _type(BlockType::Normal) {
		}

		bool operator==( const Block& rhs ) const {
			return (_x==rhs._x) && (_y==rhs._y) && (_state==rhs._state) && (_type==rhs._type);
		}

		int id() const {
			return _id;
		}

		int x() const {
			return _x;
		}

		int y() const {
			return _y;
		}

		BlockState state() const {
			return _state;
		}

		BlockType type() const {
			return _type;
		}

		void setId( int val ) {
			_id = val;
		}

		void setX( int val ) {
			_x = val;
		}

		void setY( int val ) {
			_y = val;
		}

		void setState( BlockState val ) {
			_state = val;
		}

		void setType( BlockType val ) {
			_type = val;
		}

	private:
		int _id;
		int _x;
		int _y;
		BlockState _state;
		BlockType _type;
	};
}

#endif /* __gridlr_block__ */