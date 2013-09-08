#include <server/player.h>
#include <server/server_udp.h>
#include <string>

using namespace server;
using namespace std;

unsigned int Player::highestPlayerNr = 0;

Player::Player(const std::string& token, const ServerUDP& server)
	: token(token), serverUDP(server), nr(highestPlayerNr), x(0), y(0), direction(common::NORTH)
{
	highestPlayerNr++;
}

Player::Player(const Player& player)
	: token(player.token), serverUDP(player.serverUDP), nr(player.nr), x(player.x), y(player.y), direction(player.direction)
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

unsigned int Player::getNr() const
{
	return nr;
}

void Player::getPosition(int& x, int& y) const
{
	x = this->x;
	y = this->y;
}

void Player::setPosition(int x, int y) const
{
	this->x = x;
	this->y = y;
}

void Player::getDirection(common::Direction& direction) const
{
	direction = this->direction;
}

void Player::setDirection(common::Direction direction) const
{
	this->direction = direction;
}

void Player::updatePosition() const
{
	using namespace common;

	switch(direction)
	{
	case NORTH:
		y++;
		break;
	case SOUTH:
		y--;
		break;
	case EAST:
		x--;
		break;
	case WEST:
		x++;
		break;
	}
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
