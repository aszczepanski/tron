#include <server/server_udp_listener.h>
#include <common/isocket_handler.h>
#include <common/ithread.h>
#include <common/logger.h>
#include <common/protocol.h>
#include <server/shared_memory.h>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
using namespace std;
using namespace server;

ServerUDPListener::ServerUDPListener(const string& port, SharedMemory& sharedMemory)
	: IThread(), server(port), sharedMemory(sharedMemory)
{
}

void* ServerUDPListener::start_routine()
{
	common::Logger& logger = common::Logger::getInstance();
	logger.log("SERVER UDP Listener start");

	while (true)
	{
		REQUEST request;
		bzero(&request, sizeof(REQUEST));
		server.receive(&request, sizeof(REQUEST));

		if (request.request_type == REQUEST::END_GAME)
		{
			logger.debug("SERVER UDP end request");
			break;
		}

		logger.debug(static_cast<int>(request.request_type));

		ServerUDP newServer = server;

		unsigned char c;
		string token;
		for (int i=0; i<request.length; i++)
		{
			newServer.receive(&c, 1);
			token += c;
		}

		sharedMemory.addPlayer(token, newServer);

		if (request.request_type == REQUEST::REGISTER_LISTENER)
		{
			logger.log("SERVER UDP request REGISTER_LISTENER");
		}
		else
		{
			logger.error("SERVER UDP unknown request");
		}

		c = 'a';
		newServer.send(&c, 1);

	}

	pthread_exit(NULL);
	return NULL;
}

