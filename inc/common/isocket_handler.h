#ifndef ISOCKETHANDLER_H
#define ISOCKETHANDLER_H

#include <cstdlib>

namespace common
{

class ISocketHandler
{
public:
	virtual void send(void*, size_t) = 0;
	virtual void receive(void*, size_t) = 0;
	virtual ~ISocketHandler() { }
};

}

#endif // ISOCKETHANDLER_H
