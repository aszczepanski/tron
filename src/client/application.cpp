#include <client/application.h>
#include <client/shared_memory.h>
#include <string>
#include <client/server_sender.h>
#include <client/server_listener.h>
#include <common/logger.h>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <client/open_gl_main.h>

using namespace client;

Application::Application(bool host, const std::string& hostName, const std::string& portName)
	: host(host), sharedMemory(SharedMemory::getInstance()), serverSender(hostName, portName, sharedMemory), serverListener(hostName, portName, sharedMemory)
{
}

void* Application::start_routine()
{
	try
	{
		// TCP test
		serverSender.registerClient();

		// UDP test
		serverListener.run();

		sharedMemory.setServerSender(&serverSender);
		sharedMemory.setHost(host);

		OpenGLMain ogl(sharedMemory);
		ogl.run();

		while (!sharedMemory.getEnd())
		//for (int i=0; i<200; i++)
		{
			usleep(40000u);
			serverSender.getStageInfo();
			std::cout << "CLIENT stage info request sent\n";
		}

		serverListener.wait();

		common::Logger::getInstance().log("client application closing...");

	}
	catch (...)
	{
		common::Logger::getInstance().error("exception in client test");
	}
}

