#ifndef SERVER_TCP_LISTENER_H
#define SERVER_TCP_LISTENER_H

#include <common/ithread.h>
#include <server/server_tcp.h>
#include <string>

namespace server
{

class ServerTCPListener
	: public common::IThread
{
public:
	ServerTCPListener(const std::string& port);
	void closeSocket();
private:
	void* start_routine();
	ServerTCP server;
};

}

#endif // SERVER_TCP_LISTENER_H
