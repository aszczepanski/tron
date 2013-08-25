#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstdlib>

struct REQUEST
{
	enum
	{
		START_GAME,
		END_GAME,
		STAGE_INFO
	} request_type;
	size_t length;
};

struct TURN_INFO
{
	unsigned short player_no;
	unsigned long long move_no;
	struct
	{
		int x;
		int y;
	} point;
};

#endif // PROTOCOL_H
