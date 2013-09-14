#include <server/application.h>
#include <server/shared_memory.h>
#include <common/logger.h>
#include <server/server_tcp_listener.h>
#include <server/server_udp_listener.h>
#include <iostream>
#include <server/player.h>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void* server::Application::start_routine()
{
	int n;
	try
	{
		SharedMemory sharedMemory;

		// TCP test
		server::ServerTCPListener cl("6060", sharedMemory);
		cl.run();

		// UDP test
//		server::ServerUDPListener sl("6060", sharedMemory);
//		sl.run();

		while (!sharedMemory.getStart())
		{
			std::cout << "\t\t\t" << sharedMemory.getStart() << std::endl;
			sleep(1);
		}

		std::cout << "\t\tstarting\n";

		//for (int i=0; i<15; i++)
		while(!sharedMemory.getEnd())
		{
			sharedMemory.updatePositions();

			usleep(80000u);
		}

		cl.wait();
//		sl.wait();

		std::cout << "server app leaving\n";
	}
	catch (...)
	{
		common::Logger::getInstance().error("exception in server test");
	}
}
