#include <server/shared_memory.h>
#include <server/server_tcp_connection.h>
#include <server/server_tcp.h>
#include <server/player.h>
#include <set>
#include <iostream>
#include <map>
#include <vector>
#include <common/protocol.h>
#include <cstring>
#include <algorithm>
#include <cmath>

using namespace server;
using namespace std;

common::Mutex SharedMemory::TCPMutex;

SharedMemory::SharedMemory()
	: end(false), start(false)
{
}

void SharedMemory::addPlayer(const string& token, const ServerTCP& server)
{
	int x,y;
	common::Direction direction;
	if (players.empty())
	{
		x = -3;
		y = -3;
		direction = common::NORTH;
	}
	else
	{
		x = 3;
		y = 3;
		direction = common::SOUTH;
	}
	Player playerToAdd(token, server, x, y, direction);

	std::cout << players.size() << std::endl;

	playersMutex.lock();
	players.insert(playerToAdd);
	playersMutex.unlock();

	std::cout << players.size() << std::endl;
}

void SharedMemory::removeAllPlayers()
{
	std::cout << players.size() << std::endl;
	playersMutex.lock();
	for (set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		it->clearActive();
	}
	playersMutex.unlock();
}
void SharedMemory::removePlayer(const string& token, const ServerTCP& server)
{
	Player playerToRemove(token, server);

	std::cout << players.size() << std::endl;

	playersMutex.lock();
	for (set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		if (it->getToken() == token)
		{
			it->clearActive();
			playersMutex.unlock();
			return;
		}
	}
	playersMutex.unlock();
	std::cout << "xxx\n";
	throw 1;

	std::cout << players.size() << std::endl;
}

void SharedMemory::removePlayer(const string& token)
{
	removePlayer(token, getServerTCP(token));
}

void SharedMemory::setDead(const int nr) const
{
	CRASH_INFO crash;
	crash.player_no = nr;

	playersMutex.lock();
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		if (it->getNr() == nr)
		{
			it->clearAlive();
			it->getPosition(crash.move.x, crash.move.y);
			it->getDirection(crash.move.direction);
		}
		
	}
	playersMutex.unlock();


	REQUEST request;
	request.request_type = REQUEST::NEW_CRASH;
	request.length = 1;


	TCPMutex.lock();
	for (std::set<Player>::iterator it2 = players.begin(); it2 != players.end(); it2++)
	{
		if (it2->getActive())
		{
			(it2->getServerTCP()).send(&request, sizeof(REQUEST));
			(it2->getServerTCP()).send(&crash, sizeof(CRASH_INFO));
		}
	}
	TCPMutex.unlock();

}

ServerTCP SharedMemory::getServerTCP(const string& token)
{
	playersMutex.lock();
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		if (it->getToken() == token)
		{
			playersMutex.unlock();
			return it->getServerTCP();
		}
	}
	playersMutex.unlock();
	throw 0; // exceptions !!!
}

void SharedMemory::sendTCPbroadcast(void* msg, size_t size)
{
	playersMutex.lock();
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		if (it->getActive())
		{
			(it->getServerTCP()).send(msg, size);
		}
	}
	playersMutex.unlock();
}

void SharedMemory::addMove(Player player, common::Move move)
{
	playersMutex.lock();
	for (set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		if (*it == player)
		{
			it->setDirection(move.direction);
			break;
		}
	}
	playersMutex.unlock();

	movesMutex.lock();
	moves[player.getNr()].push_back(move);
	movesMutex.unlock();
}

void SharedMemory::getMoves(std::map<int, std::vector<common::Move> >& moves)
{
	movesMutex.lock();
	moves = this->moves;
	movesMutex.unlock();
}

void SharedMemory::setPosition(Player player, float x, float y)
{
	playersMutex.lock();
	for (set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		if (*it == player)
		{
			it->setPosition(x,y);
			break;
		}
	}
	playersMutex.unlock();
}

void SharedMemory::getPlayers(vector<Player>& _players)
{
	playersMutex.lock();
	_players.clear();
	for (set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		_players.push_back(*it);
	}
	playersMutex.unlock();
}

Player SharedMemory::getPlayer(const ServerTCP& server) const
{
	playersMutex.lock();
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		if (it->getServerTCP() == server)
		{
			playersMutex.unlock();
			return *it;
		}
	}
	playersMutex.unlock();
	throw 1; // exceptions !!!
}

Player SharedMemory::getPlayer(const std::string& token) const
{
	playersMutex.lock();
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		if (it->getToken() == token)
		{
			playersMutex.unlock();
			return *it;
		}
	}
	playersMutex.unlock();
	throw 2; // exceptions !!!
}

Player SharedMemory::getPlayer(const int nr) const
{
	playersMutex.lock();
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		if (it->getNr() == nr)
		{
			playersMutex.unlock();
			return *it;
		}
	}
	playersMutex.unlock();
	throw 3; // exceptions !!!
}

static const float eps = 0.01f;

static bool checkPerpendicularIntersections(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float& x, float& y)
{
	if ((fabs(y1 - y2) >= eps) && (fabs(y3 - y4) <= eps))
	{
		std::swap(y1,y3);
		std::swap(x1,x3);
		std::swap(y2,y4);
		std::swap(x2,x4);
	}
	else if (fabs(y1 - y2) >= eps)
	{
		return false;
	}

	if (fabs(x3 - x4) >= eps)
	{
		return false;
	}

	if (x2 < x1)
	{
		std::swap(x1,x2);
		std::swap(y1,y2);
	}
	if (y4 < y3)
	{
		std::swap(y3,y4);
		std::swap(x3,x4);
	}

	if (y1 >= y3-eps && y1 <= y4+eps && x3 >= x1-eps && x3 <= x2+eps)
	{
		x = x3;
		y = y1;
		return true;
	}

	return false;
}

void SharedMemory::checkIntersections()
{
	map<int, vector<common::Move> > safeMoves;
	getMoves(safeMoves);

	vector<Player> safePlayers;
	getPlayers(safePlayers);
	for (vector<Player>::iterator it = safePlayers.begin(); it != safePlayers.end(); it++)
	{
		common::Move lastMove;
		it->getPosition(lastMove.x, lastMove.y);
		it->getDirection(lastMove.direction);
		safeMoves[it->getNr()].push_back(lastMove);
	}

	/* processing here */

	for (map<int,vector<common::Move> >::iterator i = safeMoves.begin(); i != safeMoves.end(); i++)
	{
		if (i->second.size() < 2)
		{
			continue; // TODO degenerates to point
		}
		if (!getPlayer(i->first).getAlive())
		{
			continue;
		}
		float headX1 = i->second[i->second.size()-2].x;
		float headY1 = i->second[i->second.size()-2].y;
		float headX2 = i->second[i->second.size()-1].x;
		float headY2 = i->second[i->second.size()-1].y;
		for (map<int,vector<common::Move> >::iterator j = safeMoves.begin(); j != safeMoves.end(); j++)
		{
			for (int k=0; k<(int)j->second.size()-1; k++)
			{
				float X1 = j->second[k].x;
				float Y1 = j->second[k].y;
				float X2 = j->second[k+1].x;
				float Y2 = j->second[k+1].y;

				float crossX, crossY;
				if (checkPerpendicularIntersections(headX1, headY1, headX2, headY2, X1, Y1, X2, Y2, crossX, crossY))
				{
					//cout << headX1 << " " << headY1 << " " << headX2 << " " << headY2 << " " << X1 << " " << Y1 << " " << X2 << " " << Y2 << "\n";
					//cout << crossX << " " << crossY << "\n";
					//cout << getPlayer(j->first).getDistance(crossX, crossY) << std::endl;

					if (((fabs(crossX - headX1) > eps) || (fabs(crossY != headY1) > eps)) && getPlayer(i->first).getAlive()
						&& getPlayer(i->first).getDistance(crossX, crossY) <= getPlayer(j->first).getDistance(crossX,crossY))
					{
						setDead(i->first);
					}
				}
			}
		}

		float crossX, crossY;
		if (checkPerpendicularIntersections(headX1, headY1, headX2, headY2, -FIELD_SIZE/2.0f, -FIELD_SIZE/2.0f, -FIELD_SIZE/2.0f, FIELD_SIZE/2.0f, crossX, crossY))
		{
			if (getPlayer(i->first).getAlive())
			{
				setDead(i->first);
			}
		}
		if (checkPerpendicularIntersections(headX1, headY1, headX2, headY2, -FIELD_SIZE/2.0f, FIELD_SIZE/2.0f, FIELD_SIZE/2.0f, FIELD_SIZE/2.0f, crossX, crossY))
		{
			if (getPlayer(i->first).getAlive())
			{
				setDead(i->first);
			}
		}
		if (checkPerpendicularIntersections(headX1, headY1, headX2, headY2, FIELD_SIZE/2.0f, FIELD_SIZE/2.0f, FIELD_SIZE/2.0f, -FIELD_SIZE/2.0f, crossX, crossY))
		{
			if (getPlayer(i->first).getAlive())
			{
				setDead(i->first);
			}
		}
		if (checkPerpendicularIntersections(headX1, headY1, headX2, headY2, FIELD_SIZE/2.0f, -FIELD_SIZE/2.0f, -FIELD_SIZE/2.0f, -FIELD_SIZE/2.0f, crossX, crossY))
		{
			if (getPlayer(i->first).getAlive())
			{
				setDead(i->first);
			}
		}
	}
}

void SharedMemory::updatePositions()
{
	playersMutex.lock();
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{	
		if (it->getAlive())
		{
			it->updatePosition();
		}
	}
	playersMutex.unlock();

	checkIntersections();
}

bool SharedMemory::getEnd() const
{
	// TODO mutex here
	return end;
}

void SharedMemory::setEnd()
{
	// TODO mutex here
	end = true;
}

bool SharedMemory::getStart() const
{
	startMutex.lock();
	bool tmpStart = start;
	startMutex.unlock();

	return tmpStart;
}

void SharedMemory::setStart()
{
	if (!getStart())
	{
		startMutex.lock();
		start = true;
		startMutex.unlock();

		START_INFO startInfo;
		REQUEST request;
		memset(&request, 0, sizeof(REQUEST));
		request.request_type = REQUEST::START_GAME;
		
		playersMutex.lock();
		for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
		{
			memset(&startInfo, 0, sizeof(START_INFO));
			startInfo.player_no = it->getNr();

			REQUEST turnToSend;
			memset(&turnToSend, 0, sizeof(REQUEST));
			turnToSend.request_type = REQUEST::NEW_TURN;
			turnToSend.length = 1;

			TURN_INFO newTurn;
			newTurn.player_no = it->getNr();
			it->getPosition(newTurn.move.x, newTurn.move.y);
			it->getDirection(newTurn.move.direction);

			TCPMutex.lock();
			if (it->getActive())
			{
				it->getServerTCP().send(&request, sizeof(REQUEST));
				it->getServerTCP().send(&startInfo, sizeof(START_INFO));
				
				for (std::set<Player>::iterator it2 = players.begin(); it2 != players.end(); it2++)
				{
					(it2->getServerTCP()).send(&turnToSend, sizeof(REQUEST));
					(it2->getServerTCP()).send(&newTurn, sizeof(TURN_INFO));
				}
			}
			TCPMutex.unlock();

			for (set<Player>::iterator it3 = players.begin(); it3 != players.end(); it3++)
			{
				if (*it3 == *it)
				{
					std::cout << "found\n";
					break;
				}
			}

			movesMutex.lock();
			moves[it->getNr()].push_back(newTurn.move);
			movesMutex.unlock();

		}
		playersMutex.unlock();

	}
}
