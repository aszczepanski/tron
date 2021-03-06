#ifndef SERVER_SENDER_H
#define SERVER_SENDER_H

#include <client/client_tcp.h>
#include <string>
#include <client/shared_memory.h>
#include <common/move.h>
#include <common/mutex.h>

namespace client
{

class ServerSender
{
public:
	ServerSender(SharedMemory&, ClientTCP&);
	void registerClient();
	void endGame();
	void leaveGame();
	void startGame();
	void sendTurn(common::Direction);
	void getStageInfo();
	void closeSocket();

	static common::Mutex mutex;
private:
	ClientTCP& client;
	SharedMemory& sharedMemory;
};

}

#endif // SERVER_SENDER_H
