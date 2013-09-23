#include <server/player.h>
#include <server/server_tcp.h>
#include <string>
#include <iostream>
#include <cmath>

using namespace server;
using namespace std;

unsigned int Player::highestPlayerNr = 0;

Player::Player(const std::string& token, const ServerTCP& server, float x, float y, common::Direction direction)
	: token(token), serverTCP(server), nr(highestPlayerNr), x(x), y(y), direction(direction), alive(true), active(true)
{
	highestPlayerNr++;
}

Player::Player(const Player& player)
	: token(player.getToken()), serverTCP(player.getServerTCP()), nr(player.getNr())
{
	player.getPosition(x,y);
	player.getDirection(direction);
	alive = player.getAlive();
	active = player.getActive();
}

Player& Player::operator=(const Player& player)
{
	Player tmp = Player(player);
	// TODO swap here
	
	cout << "cccccccccccccccccoooooooooooooooooooppppppppppppppppppppyyyyyyyyyyyy\n";
		
	return *this;
}

ServerTCP Player::getServerTCP() const
{
	mutex.lock();
	ServerTCP tmpServerTCP = serverTCP;
	mutex.unlock();

	return tmpServerTCP;
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

void Player::clearActive() const
{
	mutex.lock();
	active = false;
	mutex.unlock();
}

bool Player::getActive() const
{
	mutex.lock();
	bool tmpActive = active;
	mutex.unlock();
	return tmpActive;
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

void Player::getPosition(float& x, float& y) const
{
	mutex.lock();
	x = this->x;
	y = this->y;
	mutex.unlock();
}

void Player::setPosition(float x, float y) const
{
	mutex.lock();
	this->x = x;
	this->y = y;
	mutex.unlock();
}

float Player::getDistance(float xx, float yy)
{
	float curX, curY;
	getPosition(curX, curY);

	return fabs(xx-curX) + fabs(yy-curY);
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

	const float dif = 0.4;

	mutex.lock();

	switch(direction)
	{
	case NORTH:
		y += dif;
		break;
	case SOUTH:
		y -= dif;
		break;
	case EAST:
		x -= dif;
		break;
	case WEST:
		x += dif;
		break;
	}

	mutex.unlock();
}

namespace server
{
bool operator>(const Player& a, const Player& b)
{
	return a.token > b.token;
}

bool operator<(const Player& a, const Player& b)
{
	return a.token < b.token;
}

bool operator==(const Player& a, const Player& b)
{
	bool result;
	return a.token == b.token;
}

}
