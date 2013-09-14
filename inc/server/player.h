#ifndef PLAYER_H
#define PLAYER_H

#include <string>
//#include <server/server_udp.h>
#include <server/server_tcp.h>
#include <common/move.h>
#include <common/player_data.h>
#include <common/mutex.h>

namespace server
{

class Player
{
public:
	Player(const std::string& token, const ServerTCP&, int x=0, int y=0, common::Direction direction=common::NORTH);
	Player(const Player&);
	Player& operator=(const Player&);

	ServerTCP getServerTCP() const;
	std::string getToken() const;

	void clearAlive() const;
	bool getAlive() const;

	unsigned int getNr() const;

	void getPosition(int& x, int& y) const;
	void setPosition(int x, int y) const;
	void getDirection(common::Direction& direction) const;
	void setDirection(common::Direction direction) const;

	void updatePosition() const;

	friend bool operator>(const Player&, const Player&);
	friend bool operator>(const Player&, const ServerTCP&);
	friend bool operator<(const Player&, const Player&);
	friend bool operator<(const Player&, const ServerTCP&);
	friend bool operator==(const Player&, const Player&);
	friend bool operator==(const Player&, const ServerTCP&);
private:
	// token has to be unique !!!
	std::string token;
	ServerTCP serverTCP;

	unsigned int nr;

	mutable bool alive;

	static unsigned int highestPlayerNr;

	mutable int x,y;
	mutable common::Direction direction;

	mutable common::Mutex mutex;
};

}

#endif // PLAYER_H
