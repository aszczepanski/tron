#include <common/ithread.h>
#include <server/server_tcp_listener.h>
#include <server/server_tcp.h>
#include <server/shared_memory.h>
#include <server/server_tcp_connection.h>
#include <cstddef>
#include <common/logger.h>
#include <common/protocol.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <iostream>

using namespace server;
using namespace std;

ServerTCPListener::ServerTCPListener(const string& port, SharedMemory& sharedMemory)
	: common::IThread(), server(port), sharedMemory(sharedMemory)
{
}

void* ServerTCPListener::start_routine()
{
	common::Logger::getInstance().log("SERVER TCP Listener start");

	while (true)
	{

		// TODO test only
		ServerTCP newServer = server.waitForSocket();
		ServerTCPConnection* newConnection = new ServerTCPConnection(newServer, sharedMemory);
		newConnection->run();
//		newConnection->wait();
		cout << "SERVER TCP Listener connection closed" << endl;
	}

	server.closeSocket();

	return NULL;
}

void ServerTCPListener::closeSocket()
{
	server.closeSocket();
}
