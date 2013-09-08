#include <server/shared_memory.h>
#include <server/server_udp.h>
#include <server/player.h>
#include <set>
#include <iostream>

using namespace server;
using namespace std;

SharedMemory::SharedMemory()
	: end(false)
{
}

void SharedMemory::addPlayer(const string& token, const ServerUDP& server)
{
	Player playerToAdd(token, server);

	std::cout << players.size() << std::endl;
	players.insert(playerToAdd);
	std::cout << players.size() << std::endl;
}

void SharedMemory::removePlayer(const string& token, const ServerUDP& server)
{
	Player playerToRemove(token, server);

	std::cout << players.size() << std::endl;
	players.erase(playerToRemove);
	std::cout << players.size() << std::endl;
}

void SharedMemory::removePlayer(const string& token)
{
	Player playerToRemove(token, getServerUDP(token));

	std::cout << players.size() << std::endl;
	players.erase(playerToRemove);
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
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		(it->getServerUDP()).send(msg, size);
	}
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

void SharedMemory::updatePositions()
{
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		it->updatePosition();
	}	
}

bool SharedMemory::getEnd() const
{
	return end;
}

void SharedMemory::setEnd()
{
	end = true;
}
