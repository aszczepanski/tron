#include <server/server_tcp_connection.h>
#include <common/protocol.h>
#include <common/move.h>
#include <server/server_tcp.h>
#include <server/player.h>
#include <common/logger.h>
#include <cstring>
#include <iostream>
#include <ctime>
#include <vector>
#include <common/mutex.h>

using namespace server;

ServerTCPConnection::ServerTCPConnection(ServerTCP server, SharedMemory& sharedMemory)
	: server(server), sharedMemory(sharedMemory)
{
}

void* ServerTCPConnection::start_routine()
{
	common::Logger& logger = common::Logger::getInstance();
	logger.log("SERVER TCP Connection start");

	std::string playerToken;

	while (true)
	{
		REQUEST request;
		memset(&request, 0, sizeof(REQUEST));

		try
		{
			server.receive(&request, sizeof(REQUEST));
		}
		catch (...)
		{
			std::cout << "here\n";
			break;
		}

		if (request.request_type == REQUEST::END_GAME)
		{
			std::cout << "SERVER TCP CONNECTION REQUEST END GAME" << std::endl;
			memset(&request, 0, sizeof(REQUEST));
			request.request_type = REQUEST::END_GAME;
			request.length = 0;

			// send broadcast
			SharedMemory::TCPMutex.lock();
			sharedMemory.sendTCPbroadcast(&request, sizeof(REQUEST));
			SharedMemory::TCPMutex.unlock();

			sharedMemory.setEnd();

			sharedMemory.removeAllPlayers();

			//break; ???
			break;
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

			SharedMemory::TCPMutex.lock();		
	
			// send broadcast
			sharedMemory.sendTCPbroadcast(&request, sizeof(REQUEST));
			for (int i=0; i<request.length; i++)
			{
				sharedMemory.sendTCPbroadcast(&token[i], 1);
			}

			SharedMemory::TCPMutex.unlock();

			sharedMemory.removePlayer(token);

			break;
		}
		else if (request.request_type == REQUEST::NEW_TURN)
		{
			common::Direction direction;
			server.receive(&direction, sizeof(common::Direction));
			std::cout << "SERVER TCP direction " << direction << "\n";

			
			Player player = sharedMemory.getPlayer(playerToken);
			common::Move move;
			player.getPosition(move.x, move.y);
			player.getDirection(move.direction);

                        std::cout << "directions: " << move.direction << " " << direction << std::endl;
                        if( direction == common::WEST) // Turn right
                          move.direction = (common::Direction) ((move.direction + 1) % 4);
                        else if (direction == common::EAST) // Turn left
                          move.direction = (common::Direction) ((move.direction + 3) % 4);
                        sharedMemory.addMove(player, move);

			// send broadcast
			REQUEST turnToSend;
			memset(&turnToSend, 0, sizeof(REQUEST));
			turnToSend.request_type = REQUEST::NEW_TURN;
			turnToSend.length = 1;

			TURN_INFO newTurn;
			newTurn.player_no = player.getNr();
			newTurn.move.x = move.x;
			newTurn.move.y = move.y;
			newTurn.move.direction = direction;

			SharedMemory::TCPMutex.lock();
			sharedMemory.sendTCPbroadcast(&turnToSend, sizeof(REQUEST));
			sharedMemory.sendTCPbroadcast(&newTurn, sizeof(TURN_INFO));
			SharedMemory::TCPMutex.unlock();

		}
		else if (request.request_type == REQUEST::STAGE_INFO)
		{
			std::cout << "SERVER REQUEST::STAGE_INFO\n";
			std::vector<Player> players;
			sharedMemory.getPlayers(players);

			REQUEST stageInfo;
			memset(&stageInfo, 0, sizeof(REQUEST));
			stageInfo.request_type = REQUEST::STAGE_INFO;
			stageInfo.length = players.size();

			SharedMemory::TCPMutex.lock();
			sharedMemory.sendTCPbroadcast(&stageInfo, sizeof(REQUEST));

			for (int i=0; i<stageInfo.length; i++)
			{
				PLAYER_INFO playerInfo;
				memset(&playerInfo, 0, sizeof(PLAYER_INFO));
				playerInfo.player_no = players[i].getNr();
				players[i].getPosition(playerInfo.x, playerInfo.y);
				players[i].getDirection(playerInfo.direction);

				//std::cout << "SERVER position\t\t\t\t" << playerInfo.player_no << " " << playerInfo.x << " " << playerInfo.y << std::endl;

				sharedMemory.sendTCPbroadcast(&playerInfo, sizeof(PLAYER_INFO));
			}
	
			SharedMemory::TCPMutex.unlock();

			std::cout << "SERVER REQUEST::STAGE_INFO sent\n";

		}
		else if (request.request_type == REQUEST::REGISTER_TOKEN)
		{
			memset(&request, 0, sizeof(REQUEST));
			request.request_type = REQUEST::REGISTER_TOKEN;
			request.length = TOKEN_SIZE;


			srand(time(0));

			char token[TOKEN_SIZE];
			playerToken = "";
			for (int i=0; i<TOKEN_SIZE; i++)
			{
				token[i] = 'a' + rand()%26;
				playerToken += token[i];
			}

			SharedMemory::TCPMutex.lock();
			server.send(&request, sizeof(REQUEST));
			server.send(&token, TOKEN_SIZE);
			SharedMemory::TCPMutex.unlock();

			sharedMemory.addPlayer(playerToken, server);
		}
		else if (request.request_type == REQUEST::START_GAME)
		{
			std::cout << "hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n";
			sharedMemory.setStart();			
		}
	}

	sleep(1);

	server.closeConnection();

	if (sharedMemory.getEnd())
	{
		exit(0);
	}
}
