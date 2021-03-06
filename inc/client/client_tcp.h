#ifndef CLIENT_TCP_H
#define CLIENT_TCP_H

#include <common/isocket_handler.h>
#include <cstdlib>
#include <common/mutex.h>
#include <string>

namespace client
{

class ClientTCP
	: public common::ISocketHandler
{
public:
	ClientTCP(const std::string& hostname, const std::string& port);
	~ClientTCP();
	void send(void*, size_t) const;
	void receive(void*, size_t) const;
	void closeSocket();
private:
	ClientTCP(const ClientTCP&);
	ClientTCP& operator=(const ClientTCP&);
	const std::string hostname;
	const std::string port;
	int sockfd;
	bool connectionOpened;

	mutable common::Mutex mutex;
};

}

#endif // CLIENT_TCP_H
