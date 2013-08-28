#ifndef CLIENT_UDP_H
#define CLIENT_UDP_H

#include "common/isocket_handler.h"
#include <cstdlib>
#include "common/mutex.h"
#include <netinet/in.h>
#include <string>

struct hostent;

namespace client
{

class ClientUDP
	: public common::ISocketHandler
{
public:
	ClientUDP(const std::string& hostname, const std::string& port);
	~ClientUDP();
	void send(void*, size_t) const;
	void receive(void*, size_t) const;
	void closeSocket();
private:
	const std::string hostname;
	const std::string port;
	int sock;
	mutable socklen_t length; 
	struct sockaddr_in server, from;
	struct hostent* hp;

	mutable common::Mutex mutex;
};

}

#endif // CLIENT_UDP_H
