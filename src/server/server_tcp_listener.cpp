#include <common/ithread.h>
#include <server/server_tcp_listener.h>
#include <server/server_tcp.h>
#include <cstddef>
#include <common/logger.h>
#include <common/protocol.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

using namespace server;
using namespace std;

ServerTCPListener::ServerTCPListener(const string& port)
	: common::IThread(), server(port)
{
}

void* ServerTCPListener::start_routine()
{
	common::Logger::getInstance().log("SERVER TCP Listener start");

	while (true)
	{

	// TODO test only
	ServerTCP newServer = server.waitForSocket();
	}

	server.closeSocket();

	return NULL;
}

void ServerTCPListener::closeSocket()
{
	server.closeSocket();
}
