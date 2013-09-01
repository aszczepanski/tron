#ifndef SERVER_SHARED_MEMORY_H
#define SERVER_SHARED_MEMORY_H

#include <set>
#include <cstdlib>
#include <server/server_udp.h>
#include <server/player.h>

namespace server
{

class SharedMemory
{
public:
	void addPlayer(const std::string& token, const ServerUDP&);
	void removePlayer(const std::string& token);
	void removePlayer(const std::string& token, const ServerUDP&);
	void sendUDPbroadcast(void* msg, size_t size);
private:
	ServerUDP getServerUDP(const std::string& token);
	std::set<Player> players;
};

}

#endif // SERVER_SHARED_MEMORY_H
