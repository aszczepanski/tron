#ifndef SERVER_SHARED_MEMORY_H
#define SERVER_SHARED_MEMORY_H

#include <set>
#include <cstdlib>
#include <server/server_udp.h>

namespace server
{

class SharedMemory
{
public:
	void addServerUDP();
	void removeServerUDP();
	void sendUDPbroadcast(void* msg, size_t size);
private:
	std::set<ServerUDP> serversUDP;
};

}

#endif // SERVER_SHARED_MEMORY_H
