#ifndef SERVER_SHARED_MEMORY_H
#define SERVER_SHARED_MEMORY_H

#include <set>
#include <vector>
#include <map>
#include <cstdlib>
#include <server/server_udp.h>
#include <server/player.h>
#include <common/move.h>

namespace server
{

class SharedMemory
{
public:
	SharedMemory();

	void addPlayer(const std::string& token, const ServerUDP&);
	void removePlayer(const std::string& token);
	void removePlayer(const std::string& token, const ServerUDP&);

	void sendUDPbroadcast(void* msg, size_t size);

	void addMove(Player player, common::Move move); 
	void setPosition(Player player, int x, int y);
	void getPlayers(std::vector<Player>& players);

	Player getPlayer(const ServerUDP&) const;
	Player getPlayer(const std::string& token) const;

	void updatePositions();

	bool getEnd() const;
	void setEnd();
private:
	ServerUDP getServerUDP(const std::string& token);
	std::set<Player> players;
	
	std::vector< std::pair<Player, common::Move> > moves;

	bool end;
};

}

#endif // SERVER_SHARED_MEMORY_H
