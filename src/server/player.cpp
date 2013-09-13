#include <server/player.h>
#include <server/server_udp.h>
#include <string>
#include <iostream>

using namespace server;
using namespace std;

unsigned int Player::highestPlayerNr = 0;

Player::Player(const std::string& token, const ServerUDP& server, int x, int y, common::Direction direction)
	: token(token), serverUDP(server), nr(highestPlayerNr), x(x), y(y), direction(direction), alive(true)
{
	highestPlayerNr++;
}

Player::Player(const Player& player)
	: token(player.getToken()), serverUDP(player.getServerUDP()), nr(player.getNr())
{
	player.getPosition(x,y);
	player.getDirection(direction);
	alive = player.getAlive();
}

Player& Player::operator=(const Player& player)
{
	Player tmp = Player(player);
	// TODO swap here
	
	cout << "cccccccccccccccccoooooooooooooooooooppppppppppppppppppppyyyyyyyyyyyy\n";
		
	return *this;
}

ServerUDP Player::getServerUDP() const
{
	mutex.lock();
	ServerUDP tmpServerUDP = serverUDP;
	mutex.unlock();

	return tmpServerUDP;
}

void Player::clearAlive() const
{
	mutex.lock();
	alive = false;
	mutex.unlock();
}

bool Player::getAlive() const
{
	mutex.lock();
	bool tmpAlive = alive;
	mutex.unlock();
	return tmpAlive;
}

string Player::getToken() const
{
	mutex.lock();
	string tmpToken = token;
	mutex.unlock();

	return tmpToken;
}

unsigned int Player::getNr() const
{
	mutex.lock();
	int tmpNr = nr;
	mutex.unlock();

	return tmpNr;
}

void Player::getPosition(int& x, int& y) const
{
	mutex.lock();
	x = this->x;
	y = this->y;
	mutex.unlock();
}

void Player::setPosition(int x, int y) const
{
	mutex.lock();
	this->x = x;
	this->y = y;
	mutex.unlock();
}

void Player::getDirection(common::Direction& direction) const
{
	mutex.lock();
	direction = this->direction;
	mutex.unlock();
}

void Player::setDirection(common::Direction direction) const
{
	mutex.lock();
	this->direction = direction;
	mutex.unlock();
}

void Player::updatePosition() const
{
	using namespace common;

	mutex.lock();

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

	mutex.unlock();
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
	bool result;
	return a.token == b.token;
//	return a.serverUDP == b.serverUDP;
}

}
