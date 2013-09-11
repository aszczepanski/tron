#include <client/shared_memory.h>
#include <client/server_sender.h>
#include <string>
#include <iostream>
#include <vector>
#include <common/protocol.h>

using namespace client;
using namespace std;

SharedMemory::SharedMemory()
	: host(false), end(false)
{
}

SharedMemory& SharedMemory::getInstance()
{
	static SharedMemory shm;
	return shm;
}

void SharedMemory::setHost(bool host)
{
	this->host = host;
}

bool SharedMemory::isHost() const
{
	return host;
}

void SharedMemory::setToken(const string& token)
{
	this->token = token;
}

string SharedMemory::getToken() const
{
	return token;
}

void SharedMemory::setServerSender(ServerSender* server)
{
	serverSender = server;
}

void SharedMemory::keyLeft()
{
	serverSender->sendTurn(common::WEST);
}

void SharedMemory::keyRight()
{
	serverSender->sendTurn(common::EAST);
}

void SharedMemory::keyUp()
{
	serverSender->sendTurn(common::NORTH);
}

void SharedMemory::keyDown()
{
	serverSender->sendTurn(common::SOUTH);
}

void SharedMemory::keyEsc()
{
	if (host)
	{
		serverSender->endGame();
	}
	else
	{
		serverSender->leaveGame();
	}
}

void SharedMemory::keyStart()
{
	if (host)
	{
		serverSender->startGame();
	}
}

void SharedMemory::getPositions(std::vector<PLAYER_INFO>& positions) const
{
	mutex.lock();
	positions = this->positions;
	mutex.unlock();
}

void SharedMemory::addPosition(PLAYER_INFO playerInfo)
{
	std::cout << "to add: \t\t" << playerInfo.x << " " << playerInfo.y << std::endl;
	mutex.lock();
	positions.push_back(playerInfo);
	mutex.unlock();
}

void SharedMemory::getTurns(std::vector<TURN_INFO>& turns) const
{
	mutex.lock();
	turns = this->turns;
	mutex.unlock();
}

void SharedMemory::addTurn(TURN_INFO turn)
{
	mutex.lock();
	turns.push_back(turn);
	mutex.unlock();
}

void SharedMemory::getCrashes(std::vector<CRASH_INFO>& crashes) const
{
	mutex.lock();
	crashes = this->crashes;
	mutex.unlock();
}

void SharedMemory::addCrash(CRASH_INFO crash)
{
	mutex.lock();
	crashes.push_back(crash);
	mutex.unlock();
}

bool SharedMemory::setEnd()
{
	mutex.lock();
	end = true;
	mutex.unlock();
}

bool SharedMemory::getEnd() const
{
	mutex.lock();
	bool tmpEnd = end;
	mutex.unlock();

	return tmpEnd;
}

void SharedMemory::setPlayerNr(const int nr)
{
	mutex.lock();
	this->nr = nr;
	mutex.unlock();
}
