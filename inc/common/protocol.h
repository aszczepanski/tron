#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstdlib>
#include <common/move.h>
#include <common/player_data.h>

#define TOKEN_SIZE 20

const float FIELD_SIZE = 100.0f;

struct REQUEST
{
	enum
	{
		START_GAME,
		LEAVE_GAME,
		END_GAME,
		NEW_TURN,
		NEW_CRASH,
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

struct CRASH_INFO
{
	unsigned short player_no;
	common::Move move;
};


#endif // PROTOCOL_H
