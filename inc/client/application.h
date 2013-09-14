#ifndef CLIENT_APPLICATION_H
#define CLIENT_APPLICATION_H

#include <client/shared_memory.h>
#include <common/ithread.h>
#include <client/server_sender.h>
#include <client/server_listener.h>
#include <string>
#include <client/client_tcp.h>

namespace client
{

class Application
	: public common::IThread
{
public:
	Application(bool host, const std::string& hostName = /*"192.168.43.238"*/"127.0.0.1", const std::string& portName = "6060");
private:
	void* start_routine();
	bool host;
	SharedMemory& sharedMemory;
	ClientTCP client;
	ServerSender serverSender;
	ServerListener serverListener;
};

}

#endif // CLIENT_APPLICATION_H
