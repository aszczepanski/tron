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
	// TODO test only
	ServerTCP newServer = server.waitForSocket();

	REQUEST request;
	bzero(&request, sizeof(REQUEST));
	request.request_type = REQUEST::REGISTER_TOKEN;
	request.length = TOKEN_SIZE;

	newServer.send(&request, sizeof(REQUEST));

	char token[TOKEN_SIZE];
	for (int i=0; i<TOKEN_SIZE; i++)
	{
		token[i] = 'a' + i;
	}

	newServer.send(&token, TOKEN_SIZE);

	unsigned char c;

	newServer.receive(&c, 1);
	
	common::Logger::getInstance().debug(static_cast<int>(c));

//	sleep(1);

	newServer.closeConnection();

	server.closeSocket();

	return NULL;
}

void ServerTCPListener::closeSocket()
{
	server.closeSocket();
}
