#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include <common/move.h>

struct PLAYER_INFO
{
	unsigned short player_no;
	float x,y;
	common::Direction direction;
};

#endif // PLAYER_DATA_H
