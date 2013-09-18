#include <server/application.h>
#include <server/shared_memory.h>
#include <common/logger.h>
#include <server/server_tcp_listener.h>
#include <iostream>
#include <server/player.h>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void sigHandler(int signum)
{
	std::cout << "signal: " << signum << std::endl;
}

void* server::Application::start_routine()
{
	signal(SIGPIPE, sigHandler);

	int n;
	try
	{
		SharedMemory sharedMemory;

		// TCP test
		server::ServerTCPListener cl("6060", sharedMemory);
		cl.run();

		while (!sharedMemory.getStart())
		{
			std::cout << "\t\t\t" << sharedMemory.getStart() << std::endl;
			sleep(1);
		}

		std::cout << "\t\tstarting\n";

		while(!sharedMemory.getEnd())
		{
			sharedMemory.updatePositions();

			usleep(20000u);
		}

		cl.wait();

		std::cout << "server app leaving\n";
	}
	catch (...)
	{
		common::Logger::getInstance().error("exception in server test");
	}
}
