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
#include <GL/freeglut.h>

using namespace client;

Application::Application(bool host, const std::string& hostName, const std::string& portName)
	: host(host), sharedMemory(SharedMemory::getInstance()), client(hostName,portName),
		serverSender(sharedMemory,client), serverListener(sharedMemory,client)
{
}

void* Application::start_routine()
{
	try
	{
		serverSender.registerClient();

		serverListener.run();

		sharedMemory.setServerSender(&serverSender);
		sharedMemory.setHost(host);

		while (!sharedMemory.getEnd())
		{
			usleep(40000u);
			serverSender.getStageInfo();
		}

		serverListener.wait();

		common::Logger::getInstance().log("client application closing...");

	}
	catch (...)
	{
		common::Logger::getInstance().error("exception in client test");
	}

	std::cout << "r\n";
	glutLeaveMainLoop();
	std::cout << "s\n";
}

