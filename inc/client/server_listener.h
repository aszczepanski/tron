#ifndef SERVER_LISTENER_H
#define SERVER_LISTENER_H

#include <common/ithread.h>
#include <client/client_tcp.h>
#include <string>

namespace client
{

class ServerListener
	: public common::IThread
{
public:
	ServerListener(const std::string& hostname, const std::string& port);
private:
	void* start_routine();
	ClientTCP client;
};

}

#endif // SERVER_LISTENER_H
