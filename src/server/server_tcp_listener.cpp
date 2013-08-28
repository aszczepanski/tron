#include <common/ithread.h>
#include <server/server_tcp_listener.h>
#include <server/server_tcp.h>
#include <cstddef>
#include <common/logger.h>
#include <string>

using namespace server;
using namespace std;

ServerTCPListener::ServerTCPListener(const string& port)
	: common::IThread(), server(port)
{
}

void* ServerTCPListener::start_routine()
{
	common::Logger::getInstance().log("SERVER TCP Listener start");
	// TODO test only
	ServerTCP newServer = server.waitForSocket();

	unsigned char c;

	newServer.receive(&c, 1);
	
	common::Logger::getInstance().debug(static_cast<int>(c));

	newServer.closeConnection();

	server.closeSocket();

	return NULL;
}

