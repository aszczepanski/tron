#ifndef ISOCKET_HANDLER_H
#define ISOCKET_HANDLER_H

#include <cstdlib>

namespace common
{

class ISocketHandler
{
public:
	virtual void send(void*, size_t) const = 0;
	virtual void receive(void*, size_t) const = 0;
	virtual ~ISocketHandler() { }
};

}

#endif // ISOCKET_HANDLER_H
