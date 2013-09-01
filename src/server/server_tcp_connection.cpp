#include <server/server_tcp_connection.h>
#include <common/protocol.h>
#include <server/server_tcp.h>
#include <common/logger.h>
#include <cstring>
#include <iostream>
#include <ctime>

using namespace server;

ServerTCPConnection::ServerTCPConnection(ServerTCP server, SharedMemory& sharedMemory)
	: server(server), sharedMemory(sharedMemory)
{
}

void* ServerTCPConnection::start_routine()
{
	common::Logger& logger = common::Logger::getInstance();
	logger.log("SERVER TCP Connection start");

	while (true)
	{

		REQUEST request;
		bzero(&request, sizeof(REQUEST));

		server.receive(&request, sizeof(REQUEST));

		if (request.request_type == REQUEST::END_GAME)
		{
			std::cout << "SERVER TCP CONNECTION REQUEST END GAME" << std::endl;
			bzero(&request, sizeof(REQUEST));
			request.request_type = REQUEST::END_GAME;
			request.length = 0;

			// send broadcast
			sharedMemory.sendUDPbroadcast(&request, sizeof(REQUEST));
		}
		else if (request.request_type == REQUEST::LEAVE_GAME)
		{
			logger.debug("SERVER TCP leave request");
			std::string token;
			unsigned char c;
			for (int i=0; i<request.length; i++)
			{
				server.receive(&c, 1);
				token += c;
			}
			logger.debug("SERVER TCP token " + token);
			
			// send broadcast
			sharedMemory.sendUDPbroadcast(&request, sizeof(REQUEST));
			for (int i=0; i<request.length; i++)
			{
				sharedMemory.sendUDPbroadcast(&token[i], 1);
			}

			sharedMemory.removePlayer(token);
		}
		else if (request.request_type == REQUEST::STAGE_INFO)
		{
			unsigned char c;
			server.receive(&c, 1);
			std::cout << "SERVER TCP char '" << c << "'" << std::endl;

			// send broadcast
			bzero(&request, sizeof(REQUEST));
			request.request_type = REQUEST::STAGE_INFO;
			request.length = 1;
			sharedMemory.sendUDPbroadcast(&request, sizeof(REQUEST));
			sharedMemory.sendUDPbroadcast(&c, 1);
		}
		else if (request.request_type == REQUEST::REGISTER_TOKEN)
		{
			bzero(&request, sizeof(REQUEST));
			request.request_type = REQUEST::REGISTER_TOKEN;
			request.length = TOKEN_SIZE;

			server.send(&request, sizeof(REQUEST));

			srand(time(0));

			char token[TOKEN_SIZE];
			for (int i=0; i<TOKEN_SIZE; i++)
			{
				token[i] = 'a' + rand()%26;
			}

			server.send(&token, TOKEN_SIZE);
		}

		//sleep(1);
	}

	server.closeConnection();

}
