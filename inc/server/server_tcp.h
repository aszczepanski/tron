#ifndef SERVERTCP_H
#define SERVERTCP_H

#include "common/isocket_handler.h"
#include <cstdlib>
#include "common/mutex.h"

namespace server
{

class ServerTCP
	: public common::ISocketHandler
{
public:
	ServerTCP(const char* port);
	ServerTCP(const ServerTCP&);
	ServerTCP& operator=(const ServerTCP&);
	friend void swap(ServerTCP&, ServerTCP&);
	~ServerTCP();
	void send(void*, size_t);
	void receive(void*, size_t);
	void closeConnection();
	void closeMainConnection();
	ServerTCP waitForSocket();
private:
	int sockfd;
	int in_sockfd;
	char* port;
	static const unsigned short MAX_WAITING_SERVERS = 5;

	common::Mutex mutex;
};

}

#endif // SERVERTCP_H
