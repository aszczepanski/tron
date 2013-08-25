#ifndef CLIENTTCP_H
#define CLIENTTCP_H

#include "common/isocket_handler.h"
#include <cstdlib>
#include "common/mutex.h"
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
	void closeConnection();
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

#endif // CLIENTTCP_H
