#include <server/server_tcp_connection.h>
#include <common/protocol.h>
#include <common/move.h>
#include <server/server_tcp.h>
#include <server/player.h>
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

	std::string playerToken;

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

			sharedMemory.setEnd();
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
		else if (request.request_type == REQUEST::NEW_TURN)
		{
			common::Direction direction;
			server.receive(&direction, sizeof(common::Direction));
			std::cout << "SERVER TCP direction " << direction << "\n";

			
			Player player = sharedMemory.getPlayer(playerToken);
			common::Move move;
			player.getPosition(move.x, move.y);
			player.getDirection(move.direction);
			if (move.direction != direction)
			{
				std::cout << "directions: " << move.direction << " " << direction << std::endl;
				move.direction = direction;
				sharedMemory.addMove(player, move);

			}
			

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

			sharedMemory.sendUDPbroadcast(&turnToSend, sizeof(REQUEST));
			sharedMemory.sendUDPbroadcast(&newTurn, sizeof(TURN_INFO));

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

			sharedMemory.sendUDPbroadcast(&stageInfo, sizeof(REQUEST));

			for (int i=0; i<stageInfo.length; i++)
			{
				PLAYER_INFO playerInfo;
				playerInfo.player_no = players[i].getNr();
				players[i].getPosition(playerInfo.x, playerInfo.y);
				players[i].getDirection(playerInfo.direction);

				sharedMemory.sendUDPbroadcast(&playerInfo, sizeof(PLAYER_INFO));
			}

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

			playerToken = token;
		}

		//sleep(1);
	}

	server.closeConnection();

}
