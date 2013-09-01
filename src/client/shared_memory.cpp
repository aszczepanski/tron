#include <client/shared_memory.h>
#include <client/server_sender.h>
#include <string>

using namespace client;
using namespace std;

SharedMemory::SharedMemory()
	: host(false)
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
	serverSender->sendByte('l');
}

void SharedMemory::keyRight()
{
	serverSender->sendByte('r');
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
