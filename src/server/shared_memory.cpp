#include <server/shared_memory.h>
#include <server/server_udp.h>
#include <server/player.h>
#include <set>
#include <iostream>

using namespace server;
using namespace std;

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

