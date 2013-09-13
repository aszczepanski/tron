#include <server/shared_memory.h>
#include <server/server_tcp_connection.h>
#include <server/server_udp.h>
#include <server/player.h>
#include <set>
#include <iostream>
#include <map>
#include <vector>
#include <common/protocol.h>
#include <cstring>
#include <server/geo.h>

using namespace server;
using namespace std;

common::Mutex SharedMemory::UDPMutex;

SharedMemory::SharedMemory()
	: end(false), start(false)
{
}

void SharedMemory::addPlayer(const string& token, const ServerUDP& server)
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

void SharedMemory::removePlayer(const string& token, const ServerUDP& server)
{
	Player playerToRemove(token, server);

	std::cout << players.size() << std::endl;

	playersMutex.lock();
	players.erase(playerToRemove);
	playersMutex.unlock();

	std::cout << players.size() << std::endl;
}

void SharedMemory::removePlayer(const string& token)
{
	Player playerToRemove(token, getServerUDP(token));

	std::cout << players.size() << std::endl;

	playersMutex.lock();
	players.erase(playerToRemove);
	playersMutex.unlock();

	std::cout << players.size() << std::endl;
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


	UDPMutex.lock();
	for (std::set<Player>::iterator it2 = players.begin(); it2 != players.end(); it2++)
	{
		(it2->getServerUDP()).send(&request, sizeof(REQUEST));
		(it2->getServerUDP()).send(&crash, sizeof(CRASH_INFO));
	}
	UDPMutex.unlock();

}

ServerUDP SharedMemory::getServerUDP(const string& token)
{
	playersMutex.lock();
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		std::cout << token << " " << it->getToken() << std::endl;
		if (it->getToken() == token)
		{
			playersMutex.unlock();
			return it->getServerUDP();
		}
	}
	playersMutex.unlock();
	throw 0; // exceptions !!!
}

void SharedMemory::sendUDPbroadcast(void* msg, size_t size)
{
	playersMutex.lock();
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		(it->getServerUDP()).send(msg, size);
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
			std::cout << "found\n";
			std::cout << move.direction << std::endl;
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

void SharedMemory::setPosition(Player player, int x, int y)
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

Player SharedMemory::getPlayer(const ServerUDP& server) const
{
	playersMutex.lock();
	for (std::set<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		if (it->getServerUDP() == server)
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

void SharedMemory::checkIntersections()
{
	/* TODO check collisions */

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
		pkt p1 = { i->second[i->second.size()-2].x, i->second[i->second.size()-2].y };
		pkt p2 = { i->second[i->second.size()-1].x, i->second[i->second.size()-1].y };
		for (map<int,vector<common::Move> >::iterator j = safeMoves.begin(); j != safeMoves.end(); j++)
		{
			for (int k=0; k<(int)j->second.size()-1; k++)
			{
				
				pkt p3 = { j->second[k].x, j->second[k].y };
				pkt p4 = { j->second[k+1].x, j->second[k+1].y };
				cout << p1.x << " " << p1.y << "\t" << p2.x << " " << p2.y << endl;
				cout << p3.x << " " << p3.y << "\t" << p4.x << " " << p4.y << endl;

				pair<int,pktd> w = przec(p1,p2,p3,p4);
				if (w.first == 0 && ((int)w.second.x != p1.x || (int)w.second.y != p1.y)
					&& (p2.x == (int)w.second.x && p2.y == (int)w.second.y))
				{
					cout << "\n\niiiiiiiiiiiiiiinnnnnnnnnnnnnnnntttttttttttttteeeeeeeeeeeeeeerrrrrrrrrrssssssssssseeeeeeeeeeeeccccccccccccttttttttt\n\n";
					cout << i->first << " " << j->first << endl<<endl;
					cout << (int)w.second.x << " " << (int)w.second.y << endl<<endl;

					if (getPlayer(i->first).getAlive())
					{
						setDead(i->first);
					}
				}
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

			UDPMutex.lock();
			it->getServerUDP().send(&request, sizeof(REQUEST));
			it->getServerUDP().send(&startInfo, sizeof(START_INFO));
			
			for (std::set<Player>::iterator it2 = players.begin(); it2 != players.end(); it2++)
			{
				(it2->getServerUDP()).send(&turnToSend, sizeof(REQUEST));
				(it2->getServerUDP()).send(&newTurn, sizeof(TURN_INFO));
			}
			
			UDPMutex.unlock();

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
