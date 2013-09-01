#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <server/server_udp.h>

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
};

}

#endif // PLAYER_H
