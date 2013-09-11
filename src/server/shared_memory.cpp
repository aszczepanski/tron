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
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		std::cout << token << " " << it->getToken() << std::endl;
		if (it->getToken() == token)
		{
			return it->getServerUDP();
		}
	}
	throw 0; // exceptions !!!
}

void SharedMemory::sendUDPbroadcast(void* msg, size_t size)
{
	broadcastMutex.lock();
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		(it->getServerUDP()).send(msg, size);
	}
	broadcastMutex.unlock();
}

void SharedMemory::addMove(Player player, common::Move move)
{
	/* TODO check if possible etc... */

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

	moves.push_back(make_pair(player,move));
	std::cout << "moves.size(): " << moves.size() << std::endl;
}

void SharedMemory::setPosition(Player player, int x, int y)
{
	for (set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		if (*it == player)
		{
			it->setPosition(x,y);
			break;
		}
	}
}

void SharedMemory::getPlayers(vector<Player>& _players)
{
	_players.clear();
	for (set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		_players.push_back(*it);
	}
}

Player SharedMemory::getPlayer(const ServerUDP& server) const
{
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		if (it->getServerUDP() == server)
		{
			return *it;
		}
	}
	throw 1; // exceptions !!!
}

Player SharedMemory::getPlayer(const std::string& token) const
{
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		if (it->getToken() == token)
		{
			return *it;
		}
	}
	throw 2; // exceptions !!!
}

void SharedMemory::checkIntersections()
{
	/* TODO check collisions */
}

void SharedMemory::updatePositions()
{
	broadcastMutex.lock();
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		it->updatePosition();
	}
	broadcastMutex.unlock();

	checkIntersections();
}

bool SharedMemory::getEnd() const
{
	return end;
}

void SharedMemory::setEnd()
{
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
		
		broadcastMutex.lock();
		for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
		{
			memset(&startInfo, 0, sizeof(START_INFO));
			startInfo.player_no = it->getNr();
/*
			REQUEST turnToSend;
			memset(&turnToSend, 0, sizeof(REQUEST));
			turnToSend.request_type = REQUEST::NEW_TURN;
			turnToSend.length = 1;

			TURN_INFO newTurn;
			newTurn.player_no = it->getNr();
			it->getPosition(newTurn.move.x, newTurn.move.y);
			it->getDirection(newTurn.move.direction);
*/
			UDPMutex.lock();
			it->getServerUDP().send(&request, sizeof(REQUEST));
			it->getServerUDP().send(&startInfo, sizeof(START_INFO));
/*			
			for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
			{
				(it->getServerUDP()).send(&turnToSend, sizeof(REQUEST));
				(it->getServerUDP()).send(&newTurn, sizeof(TURN_INFO));
			}
*/			
			UDPMutex.unlock();
		}
		broadcastMutex.unlock();
	}
}
