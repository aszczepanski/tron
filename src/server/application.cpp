#include <server/application.h>
#include <server/shared_memory.h>
#include <common/logger.h>
#include <server/server_tcp_listener.h>
#include <server/server_udp_listener.h>
#include <iostream>

using namespace server;

void* Application::start_routine()
{
	int n;
	try
	{
		SharedMemory sharedMemory;

		// TCP test
		server::ServerTCPListener cl("6060", sharedMemory);
		cl.run();

		// UDP test
		server::ServerUDPListener sl("6060", sharedMemory);
		sl.run();

		cl.wait();
		sl.wait();
		std::cout << "server app leaving\n";
	}
	catch (...)
	{
		common::Logger::getInstance().error("exception in server test");
	}
}
