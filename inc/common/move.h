#ifndef MOVE_H
#define MOVE_H

namespace common
{

enum Direction
{
	NORTH,
	SOUTH,
	EAST,
	WEST
};

struct Move
{
	int x, y;
	Direction direction;	
};

}

#endif // MOVE_H
