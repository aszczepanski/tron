#ifndef MOVE_H
#define MOVE_H

namespace common
{

enum Direction
{
	NORTH = 0,
	EAST = 1,
	SOUTH = 2,
	WEST = 3
};

struct Move
{
	float x, y;
	Direction direction;
};

}

#endif // MOVE_H
