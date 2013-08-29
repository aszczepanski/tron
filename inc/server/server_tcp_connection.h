#ifndef SERVER_TCP_CONNECTION_H
#define SERVER_TCP_CONNECTION_H

#include <common/ithread.h>
#include <server/shared_memory.h>
#include <server/server_tcp.h>

namespace server
{

class ServerTCPConnection
	: public common::IThread
{
public:
	ServerTCPConnection(ServerTCP server, SharedMemory& sharedMemory);
private:
	void* start_routine();
	ServerTCP server;
	SharedMemory& sharedMemory;
};

}

#endif // SERVER_TCP_CONNECTION_H
