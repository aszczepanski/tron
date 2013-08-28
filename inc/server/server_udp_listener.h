#ifndef SERVER_UDP_LISTENER_H
#define SERVER_UDP_LISTENER_H

#include <common/ithread.h>
#include <server/server_udp.h>
#include <string>

namespace server
{

class ServerUDPListener
	: public common::IThread
{
public:
	ServerUDPListener(const std::string& port);
	void* start_routine();
private:
	ServerUDP server;
};

}

#endif // SERVER_UDP_LISTENER_H
