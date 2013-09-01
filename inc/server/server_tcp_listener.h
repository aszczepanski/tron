#ifndef SERVER_TCP_LISTENER_H
#define SERVER_TCP_LISTENER_H

#include <common/ithread.h>
#include <server/server_tcp.h>
#include <server/shared_memory.h>
#include <string>

namespace server
{

class ServerTCPListener
	: public common::IThread
{
public:
	ServerTCPListener(const std::string& port, SharedMemory& sharedMemory);
	void closeSocket();
private:
	void* start_routine();
	ServerTCP server;
	SharedMemory& sharedMemory;
};

}

#endif // SERVER_TCP_LISTENER_H
