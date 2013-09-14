#ifndef SERVER_LISTENER_H
#define SERVER_LISTENER_H

#include <common/ithread.h>
#include <client/client_tcp.h>
//#include <client/client_udp.h>
#include <string>
#include <client/shared_memory.h>

namespace client
{

class ServerListener
	: public common::IThread
{
public:
	ServerListener(SharedMemory&, ClientTCP&);
private:
	void* start_routine();
	ClientTCP& client;
	//ClientUDP client;
	SharedMemory& sharedMemory;
};

}

#endif // SERVER_LISTENER_H
