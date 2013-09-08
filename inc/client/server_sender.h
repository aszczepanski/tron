#ifndef SERVER_SENDER_H
#define SERVER_SENDER_H

#include <client/client_tcp.h>
#include <string>
#include <client/shared_memory.h>
#include <common/move.h>

namespace client
{

class ServerSender
{
public:
	ServerSender(const std::string& hostname, const std::string& port, SharedMemory&);
	void registerClient();
	void endGame();
	void leaveGame();
	void sendTurn(common::Direction);
	void getStageInfo();
	void sendByte(unsigned char c);
	void closeSocket();
private:
	ClientTCP client;
	SharedMemory& sharedMemory;
};

}

#endif // SERVER_SENDER_H
