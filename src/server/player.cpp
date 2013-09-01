#include <server/player.h>
#include <server/server_udp.h>
#include <string>

using namespace server;
using namespace std;

Player::Player(const std::string& token, const ServerUDP& server)
	: token(token), serverUDP(server)
{
}

Player::Player(const Player& player)
	: token(player.token), serverUDP(player.serverUDP)
{
}

Player& Player::operator=(const Player& player)
{
	Player tmp = Player(player);
	// TODO swap here
	return *this;
}

ServerUDP Player::getServerUDP() const
{
	return serverUDP;
}

string Player::getToken() const
{
	return token;
}

namespace server
{
bool operator>(const Player& a, const Player& b)
{
	return a.token > b.token;
//	return a.serverUDP > b.serverUDP;
}

bool operator<(const Player& a, const Player& b)
{
	return a.token < b.token;
//	return a.serverUDP < b.serverUDP;
}

bool operator==(const Player& a, const Player& b)
{
	return a.token == b.token;
//	return a.serverUDP == b.serverUDP;
}

}
