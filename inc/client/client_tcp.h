#ifndef CLIENTTCP_H
#define CLIENTTCP_H

#include "common/isocket_handler.h"
#include <cstdlib>
#include "common/mutex.h"

namespace client
{

class ClientTCP
	: public common::ISocketHandler
{
public:
	ClientTCP(const char* hostname, const char* port);
	~ClientTCP();
	void send(void*, size_t);
	void receive(void*, size_t);
	void closeConnection();
private:
	const char* hostname;
	const char* port;
	int sockfd;
	bool connectionOpened;

	common::Mutex mutex;
};

}

#endif // CLIENTTCP_H
