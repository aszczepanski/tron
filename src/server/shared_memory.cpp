#include <server/shared_memory.h>
#include <server/server_tcp_connection.h>
#include <server/server_udp.h>
#include <server/player.h>
#include <set>
#include <iostream>
#include <common/protocol.h>
#include <cstring>

using namespace server;
using namespace std;

common::Mutex SharedMemory::UDPMutex;

SharedMemory::SharedMemory()
	: end(false), start(false)
{
}

void SharedMemory::addPlayer(const string& token, const ServerUDP& server)
{
	int x,y;
	common::Direction direction;
	if (players.empty())
	{
		x = -3;
		y = -3;
		direction = common::NORTH;
	}
	else
	{
		x = 3;
		y = 3;
		direction = common::SOUTH;
	}
	Player playerToAdd(token, server, x, y, direction);

	std::cout << players.size() << std::endl;

	playersMutex.lock();
	players.insert(playerToAdd);
	playersMutex.unlock();

	std::cout << players.size() << std::endl;
}

void SharedMemory::removePlayer(const string& token, const ServerUDP& server)
{
	Player playerToRemove(token, server);

	std::cout << players.size() << std::endl;

	playersMutex.lock();
	players.erase(playerToRemove);
	playersMutex.unlock();

	std::cout << players.size() << std::endl;
}

void SharedMemory::removePlayer(const string& token)
{
	Player playerToRemove(token, getServerUDP(token));

	std::cout << players.size() << std::endl;

	playersMutex.lock();
	players.erase(playerToRemove);
	playersMutex.unlock();

	std::cout << players.size() << std::endl;
}

ServerUDP SharedMemory::getServerUDP(const string& token)
{
	playersMutex.lock();
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		std::cout << token << " " << it->getToken() << std::endl;
		if (it->getToken() == token)
		{
			playersMutex.unlock();
			return it->getServerUDP();
		}
	}
	playersMutex.unlock();
	throw 0; // exceptions !!!
}

void SharedMemory::sendUDPbroadcast(void* msg, size_t size)
{
	playersMutex.lock();
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		(it->getServerUDP()).send(msg, size);
	}
	playersMutex.unlock();
}

void SharedMemory::addMove(Player player, common::Move move)
{
	playersMutex.lock();
	for (set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		if (*it == player)
		{
			std::cout << "found\n";
			std::cout << move.direction << std::endl;
			it->setDirection(move.direction);
			break;
		}
	}
	playersMutex.unlock();

	movesMutex.lock();
	moves.push_back(make_pair(player.getNr(),move));
	movesMutex.unlock();
}

void SharedMemory::getMoves(vector< pair<int,common::Move> >& moves)
{
	movesMutex.lock();
	moves = this->moves;
	movesMutex.unlock();
}

void SharedMemory::setPosition(Player player, int x, int y)
{
	playersMutex.lock();
	for (set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		if (*it == player)
		{
			it->setPosition(x,y);
			break;
		}
	}
	playersMutex.unlock();
}

void SharedMemory::getPlayers(vector<Player>& _players)
{
	playersMutex.lock();
	_players.clear();
	for (set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		_players.push_back(*it);
	}
	playersMutex.unlock();
}

Player SharedMemory::getPlayer(const ServerUDP& server) const
{
	playersMutex.lock();
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		if (it->getServerUDP() == server)
		{
			playersMutex.unlock();
			return *it;
		}
	}
	playersMutex.unlock();
	throw 1; // exceptions !!!
}

Player SharedMemory::getPlayer(const std::string& token) const
{
	playersMutex.lock();
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		if (it->getToken() == token)
		{
			playersMutex.unlock();
			return *it;
		}
	}
	playersMutex.unlock();
	throw 2; // exceptions !!!
}

Player SharedMemory::getPlayer(const int nr) const
{
	playersMutex.lock();
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		if (it->getNr() == nr)
		{
			playersMutex.unlock();
			return *it;
		}
	}
	playersMutex.unlock();
	throw 3; // exceptions !!!
}

void SharedMemory::checkIntersections()
{
	/* TODO check collisions */

	vector<Player> safePlayers;
	getPlayers(safePlayers);

	vector< pair<int,common::Move> > safeMoves;
	getMoves(safeMoves);

	/* processing here */
}

void SharedMemory::updatePositions()
{
	playersMutex.lock();
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		it->updatePosition();
	}
	playersMutex.unlock();

	checkIntersections();
}

bool SharedMemory::getEnd() const
{
	// TODO mutex here
	return end;
}

void SharedMemory::setEnd()
{
	// TODO mutex here
	end = true;
}

bool SharedMemory::getStart() const
{
	startMutex.lock();
	bool tmpStart = start;
	startMutex.unlock();

	return tmpStart;
}

void SharedMemory::setStart()
{
	if (!getStart())
	{
		startMutex.lock();
		start = true;
		startMutex.unlock();

		START_INFO startInfo;
		REQUEST request;
		memset(&request, 0, sizeof(REQUEST));
		request.request_type = REQUEST::START_GAME;
		
		playersMutex.lock();
		for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
		{
			memset(&startInfo, 0, sizeof(START_INFO));
			startInfo.player_no = it->getNr();

			REQUEST turnToSend;
			memset(&turnToSend, 0, sizeof(REQUEST));
			turnToSend.request_type = REQUEST::NEW_TURN;
			turnToSend.length = 1;

			TURN_INFO newTurn;
			newTurn.player_no = it->getNr();
			it->getPosition(newTurn.move.x, newTurn.move.y);
			it->getDirection(newTurn.move.direction);

			UDPMutex.lock();
			it->getServerUDP().send(&request, sizeof(REQUEST));
			it->getServerUDP().send(&startInfo, sizeof(START_INFO));
			
			for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
			{
				(it->getServerUDP()).send(&turnToSend, sizeof(REQUEST));
				(it->getServerUDP()).send(&newTurn, sizeof(TURN_INFO));
			}
			
			UDPMutex.unlock();
		}
		playersMutex.unlock();
	}
}
