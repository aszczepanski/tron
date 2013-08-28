#ifndef ISOCKET_HANDLER_H
#define ISOCKET_HANDLER_H

#include <cstdlib>
#include <stdexcept>

namespace common
{

class ISocketHandler
{
public:
	virtual void send(void*, size_t) const = 0;
	virtual void receive(void*, size_t) const = 0;
	virtual ~ISocketHandler() { }

	class SocketError : public std::exception { };
	class BindError : public std::exception { };
	class ListenError : public std::exception { };
	class AcceptError : public std::exception { };
	class WriteError : public std::exception { };
	class ReadError : public std::exception { };
	class AtonError : public std::exception { };
	class ConnectError : public std::exception { };
	class UnknownHostError : public std::exception { };
};

}

#endif // ISOCKET_HANDLER_H
