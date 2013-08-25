#include <client/server_listener.h>
#include <common/logger.h>
#include <cstddef>
#include <string>

using namespace client;
using namespace std;

ServerListener::ServerListener(const string& hostname, const string& port)
	: IThread(), client(hostname, port)
{
}

void* ServerListener::start_routine()
{
	// TODO test only
	unsigned char c;
	client.receive(&c, 1);

	common::Logger::getInstance().debug(static_cast<int>(c));

	return NULL;
}
