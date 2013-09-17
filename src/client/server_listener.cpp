#include <client/server_listener.h>
#include <client/client_tcp.h>
#include <common/logger.h>
#include <common/protocol.h>
#include <cstddef>
#include <string>
#include <cstring>
#include <iostream>
#include <client/shared_memory.h>

using namespace client;
using namespace std;

ServerListener::ServerListener(SharedMemory& sharedMemory, ClientTCP& client)
	: IThread(), client(client), sharedMemory(sharedMemory)
{
}

void* ServerListener::start_routine()
{
	common::Logger::getInstance().log("CLIENT TCP listener start");

	REQUEST request;
	while (true)
	{
		memset(&request, 0, sizeof(REQUEST));
		client.receive(&request, sizeof(REQUEST));
		//cout << request.length << endl;

		if (request.request_type == REQUEST::END_GAME)
		{
			common::Logger::getInstance().debug("CLIENT TCP request end");

			sharedMemory.setEnd();
			std::cout << "\t\t" << sharedMemory.getEnd() << std::endl;

			break;
		}
		else if (request.request_type == REQUEST::LEAVE_GAME)
		{
			common::Logger::getInstance().debug("CLIENT TCP request leave");
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
				sharedMemory.setEnd();
				std::cout << "\t\t" << sharedMemory.getEnd() << std::endl;

				break;
			}
		}
		else if (request.request_type == REQUEST::STAGE_INFO)
		{
			//cout << "CLIENT stage info received\n";

			for (int i=0; i<request.length; i++)
			{
				PLAYER_INFO playerInfo;
				memset(&playerInfo, 0, sizeof(PLAYER_INFO));
				client.receive(&playerInfo, sizeof(PLAYER_INFO));

				//cout << "playerInfo.player_no: " << playerInfo.player_no << std::endl;
				//cout << playerInfo.x << " " << playerInfo.y << endl;

				sharedMemory.addPosition(playerInfo);
			}
			
		}
		else if (request.request_type == REQUEST::NEW_TURN)
		{
			cout << "request.length: " << request.length << endl;
			for (int i=0; i<request.length; i++)
			{
				TURN_INFO newTurn;
				memset(&newTurn, 0, sizeof(TURN_INFO));
				client.receive(&newTurn, sizeof(TURN_INFO));
			
				std::cout << "player: " << newTurn.player_no << std::endl;
				cout << newTurn.move.x << " " << newTurn.move.y << endl;
				std::cout << "direction: " << newTurn.move.direction << std::endl;

				sharedMemory.addTurn(newTurn);
			}
		}
		else if (request.request_type == REQUEST::REGISTER_TOKEN)
		{
			string token;

			char byte;
			for (int i=0; i<request.length; i++)
			{
				client.receive(&byte, 1);
				token += byte;
			}

			std::cout << "\t\t" << token << std::endl;

			sharedMemory.setToken(token);

		}
		else if (request.request_type == REQUEST::NEW_CRASH)
		{
			for (int i=0; i<request.length; i++)
			{
				CRASH_INFO newCrash;
				memset(&newCrash, 0, sizeof(CRASH_INFO));
				client.receive(&newCrash, sizeof(CRASH_INFO));
			
				std::cout << "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbplayer: " << newCrash.player_no << std::endl;
				cout << newCrash.move.x << " " << newCrash.move.y << endl;
				std::cout << "direction: " << newCrash.move.direction << std::endl;

				sharedMemory.addCrash(newCrash);
			}
		}
		else if (request.request_type == REQUEST::START_GAME)
		{
			START_INFO startInfo;
			memset(&startInfo, 0, sizeof(START_INFO));
			client.receive(&startInfo, sizeof(START_INFO));

			sharedMemory.setPlayerNr(startInfo.player_no);
		}
	}

	client.closeSocket();

	std::cout << "CLIENT socket closed\n";

	return NULL;
}
