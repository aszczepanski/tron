#ifndef SERVERUDP_H
#define SERVERUDP_H

#include "common/isocket_handler.h"
#include <cstdlib>
#include "common/mutex.h"
#include <netinet/in.h>
#include <string>
#include <stdexcept>

namespace server
{

class ServerUDP
	: public common::ISocketHandler
{
public:
	ServerUDP(const std::string& port);
	ServerUDP(const ServerUDP&);
	ServerUDP& operator=(const ServerUDP&);
	friend void swap(ServerUDP&, ServerUDP&);
	~ServerUDP();
	void send(void*, size_t) const;
	void receive(void*, size_t) const;

	friend bool operator>(const ServerUDP&, const ServerUDP&);
	friend bool operator<(const ServerUDP&, const ServerUDP&);
	friend bool operator==(const ServerUDP&, const ServerUDP&);
private:
	int sock;
	mutable socklen_t length;
	mutable socklen_t fromlen;
	struct sockaddr_in server;
	struct sockaddr_in from;
	std::string port;

	mutable common::Mutex mutex;
};

}

#endif // SERVERUDP_H
