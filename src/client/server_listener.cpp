#include <client/server_listener.h>
#include <client/client_udp.h>
#include <common/logger.h>
#include <common/protocol.h>
#include <cstddef>
#include <string>
#include <cstring>

using namespace client;
using namespace std;

ServerListener::ServerListener(const string& hostname, const string& port)
	: IThread(), client(hostname, port)
{
}

void* ServerListener::start_routine()
{
	common::Logger::getInstance().log("CLIENT UDP listener start");

	// TODO test only
	REQUEST request;
	bzero(&request, sizeof(REQUEST));
	request.request_type = REQUEST::REGISTER_LISTENER;
	request.length = 0;

	client.send(&request, sizeof(REQUEST));

	unsigned char c;
	client.receive(&c, 1);

	common::Logger::getInstance().debug(static_cast<int>(c));

	client.closeSocket();

	return NULL;
}
