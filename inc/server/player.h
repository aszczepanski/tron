#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <server/server_udp.h>
#include <common/move.h>

namespace server
{

class Player
{
public:
	Player(const std::string& token, const ServerUDP&);
	Player(const Player&);
	Player& operator=(const Player&);

	ServerUDP getServerUDP() const;
	std::string getToken() const;

	unsigned int getNr() const;

	void getPosition(int& x, int& y) const;
	void setPosition(int x, int y) const;
	void getDirection(common::Direction& direction) const;
	void setDirection(common::Direction direction) const;

	void updatePosition() const;

	friend bool operator>(const Player&, const Player&);
	friend bool operator>(const Player&, const ServerUDP&);
	friend bool operator<(const Player&, const Player&);
	friend bool operator<(const Player&, const ServerUDP&);
	friend bool operator==(const Player&, const Player&);
	friend bool operator==(const Player&, const ServerUDP&);
private:
	// token has to be unique !!!
	std::string token;
	ServerUDP serverUDP;

	unsigned int nr;

	static unsigned int highestPlayerNr;

	mutable int x,y;
	mutable common::Direction direction;
};

}

#endif // PLAYER_H
