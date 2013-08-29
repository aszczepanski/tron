#include <server/server_tcp_connection.h>
#include <common/protocol.h>
#include <server/server_tcp.h>
#include <common/logger.h>
#include <cstring>
#include <iostream>

using namespace server;

ServerTCPConnection::ServerTCPConnection(ServerTCP server, SharedMemory& sharedMemory)
	: server(server), sharedMemory(sharedMemory)
{
}

void* ServerTCPConnection::start_routine()
{

	while (true)
	{

		REQUEST request;
		bzero(&request, sizeof(REQUEST));

		server.receive(&request, sizeof(REQUEST));

		if (request.request_type == REQUEST::END_GAME)
		{
			std::cout << "SERVER TCP CONNECTION REQUEST END GAME" << std::endl;
			break;
		}

		bzero(&request, sizeof(REQUEST));
		request.request_type = REQUEST::REGISTER_TOKEN;
		request.length = TOKEN_SIZE;

		server.send(&request, sizeof(REQUEST));

		char token[TOKEN_SIZE];
		for (int i=0; i<TOKEN_SIZE; i++)
		{
			token[i] = 'a' + i;
		}

		server.send(&token, TOKEN_SIZE);

		//sleep(1);
	}

	server.closeConnection();

}
