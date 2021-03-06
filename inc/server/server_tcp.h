#ifndef SERVER_TCP_H
#define SERVER_TCP_H

#include <common/isocket_handler.h>
#include <cstdlib>
#include <common/mutex.h>
#include <string>
#include <stdexcept>

namespace server
{

class ServerTCP
	: public common::ISocketHandler
{
public:
	ServerTCP(const std::string& port);
	ServerTCP(const ServerTCP&);
	ServerTCP& operator=(const ServerTCP&);
	friend void swap(ServerTCP&, ServerTCP&);
	~ServerTCP();
	void send(void*, size_t) const;
	void receive(void*, size_t) const;
	void closeSocket();
	void closeConnection();
	ServerTCP waitForSocket();

	friend bool operator>(const ServerTCP&, const ServerTCP&);
	friend bool operator<(const ServerTCP&, const ServerTCP&);
	friend bool operator==(const ServerTCP&, const ServerTCP&);
private:
	int sockfd;
	int in_sockfd;
	std::string port;
	static const unsigned short MAX_WAITING_SERVERS = 5;

	mutable common::Mutex mutex;
};

}

#endif // SERVER_TCP_H
