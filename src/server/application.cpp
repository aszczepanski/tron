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
		// TCP test
		server::ServerTCPListener cl("6060");
		cl.run();

		// UDP test
		server::ServerUDPListener sl("6060");
		sl.run();

//		std::cout << "1 - client test\n"
//			<< "2 - quit\n";
//		std::cin >> n;

		cl.wait();
		sl.wait();
	}
	catch (...)
	{
		common::Logger::getInstance().error("exception in server test");
	}
}
