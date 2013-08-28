#include <server/server_udp_listener.h>
#include <common/isocket_handler.h>
#include <common/ithread.h>
#include <common/logger.h>
#include <common/protocol.h>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
using namespace std;
using namespace server;

ServerUDPListener::ServerUDPListener(const string& port)
	: IThread(), server(port)
{
}

void* ServerUDPListener::start_routine()
{
	common::Logger& logger = common::Logger::getInstance();
	logger.log("SERVER UDP Listener start");

	{
		REQUEST request;
		bzero(&request, sizeof(REQUEST));
		server.receive(&request, sizeof(REQUEST));

		// possible - request END

		logger.debug(static_cast<int>(request.request_type));

		ServerUDP newServer = server;

		if (request.request_type == REQUEST::REGISTER_LISTENER)
		{
			logger.log("SERVER UDP request REGISTER_LISTENER");
		}
		else
		{
			logger.error("SERVER UDP unknown request");
		}

		unsigned char c = 'a';
		newServer.send(&c, 1);

	}

	pthread_exit(NULL);
	return NULL;
}

