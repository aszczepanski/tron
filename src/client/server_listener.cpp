#include <client/server_listener.h>
#include <client/client_udp.h>
#include <common/logger.h>
#include <common/protocol.h>
#include <cstddef>
#include <string>
#include <cstring>
#include <iostream>
#include <client/shared_memory.h>

using namespace client;
using namespace std;

ServerListener::ServerListener(const string& hostname, const string& port, SharedMemory& sharedMemory)
	: IThread(), client(hostname, port), sharedMemory(sharedMemory)
{
}

void* ServerListener::start_routine()
{
	common::Logger::getInstance().log("CLIENT UDP listener start");

	// TODO test only
	REQUEST request;
	bzero(&request, sizeof(REQUEST));
	request.request_type = REQUEST::REGISTER_LISTENER;
	request.length = TOKEN_SIZE;

	client.send(&request, sizeof(REQUEST));

	for (int i=0; i<TOKEN_SIZE; i++)
	{
		client.send(&sharedMemory.getToken()[i], 1);
	}

	unsigned char c;
	client.receive(&c, 1);

	common::Logger::getInstance().debug(static_cast<int>(c));

	while (true)
	{
		bzero(&request, sizeof(REQUEST));
		client.receive(&request, sizeof(REQUEST));

		if (request.request_type == REQUEST::END_GAME)
		{
			common::Logger::getInstance().debug("CLIENT UDP request end");
			break;
		}
		else if (request.request_type == REQUEST::LEAVE_GAME)
		{
			common::Logger::getInstance().debug("CLIENT UDP request leave");
			string token;
			unsigned char c;
			for (int i=0; i<request.length; i++)
			{
				client.receive(&c, 1);
				token += c;
			}
			if (token == sharedMemory.getToken())
			{
				cout << "genuine token: " << token << endl;
				break;
			}
		}
		else if (request.request_type == REQUEST::STAGE_INFO)
		{
			unsigned char c;
			client.receive(&c, 1);
			std::cout << c << " received\n";
		}
	}

	client.closeSocket();

	return NULL;
}
