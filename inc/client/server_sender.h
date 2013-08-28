#ifndef SERVER_SENDER_H
#define SERVER_SENDER_H

#include <client/client_tcp.h>
#include <string>

namespace client
{

class ServerSender
{
public:
	ServerSender(const std::string& hostname, const std::string& port);
	void registerClient(std::string& token);
	void endGame();
	void sendTurn();
	void closeSocket();
private:
	ClientTCP client;
};

}

#endif // SERVER_SENDER_H
