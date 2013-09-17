#ifndef SERVER_SHARED_MEMORY_H
#define SERVER_SHARED_MEMORY_H

#include <set>
#include <vector>
#include <map>
#include <cstdlib>
//#include <server/server_udp.h>
#include <server/server_tcp.h>
#include <server/player.h>
#include <common/move.h>
#include <common/mutex.h>

namespace server
{

class SharedMemory
{
public:
	SharedMemory();

	void addPlayer(const std::string& token, const ServerTCP&);
	void removePlayer(const std::string& token);
	void removePlayer(const std::string& token, const ServerTCP&);
	void removeAllPlayers();

	//void sendUDPbroadcast(void* msg, size_t size);
	void sendTCPbroadcast(void* msg, size_t size);

	void addMove(Player player, common::Move move);
	void getMoves(std::map<int, std::vector<common::Move> >&);

	void setPosition(Player player, int x, int y);
	void getPlayers(std::vector<Player>& players);

	//Player getPlayer(const ServerUDP&) const;
	Player getPlayer(const ServerTCP&) const;
	Player getPlayer(const std::string& token) const;
	Player getPlayer(const int nr) const;

	void updatePositions();

	bool getEnd() const;
	void setEnd();

	bool getStart() const;
	void setStart();

	void setDead(const int nr) const;

	//static common::Mutex UDPMutex;
	static common::Mutex TCPMutex;
private:
	//ServerUDP getServerUDP(const std::string& token);
	ServerTCP getServerTCP(const std::string& token);
	std::set<Player> players;
	
	std::map<int, std::vector<common::Move> > moves;

	mutable common::Mutex startMutex;
	mutable common::Mutex playersMutex;
	mutable common::Mutex movesMutex;

	bool end;
	bool start;

	void checkIntersections();
};

}

#endif // SERVER_SHARED_MEMORY_H
