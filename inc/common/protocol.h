#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstdlib>
#include <common/move.h>

#define TOKEN_SIZE 20

struct REQUEST
{
	enum
	{
		START_GAME,
		LEAVE_GAME,
		END_GAME,
		NEW_TURN,
		STAGE_INFO,
		REGISTER_TOKEN,
		REGISTER_LISTENER
	} request_type;
	size_t length;
};

struct START_INFO
{
	unsigned short player_no;
};

struct TURN_INFO
{
	unsigned short player_no;
	common::Move move;
};

struct PLAYER_INFO
{
	unsigned short player_no;
	int x,y;
	common::Direction direction;
};

#endif // PROTOCOL_H
