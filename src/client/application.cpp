#include <client/application.h>
#include <client/shared_memory.h>
#include <string>
#include <client/server_sender.h>
#include <client/server_listener.h>
#include <common/logger.h>
#include <iostream>
#include <cstdio>

using namespace client;

Application::Application(bool host, const std::string& hostName, const std::string& portName)
	: host(host), serverSender(hostName, portName), serverListener(hostName, portName)
{
}

void* Application::start_routine()
{
	try
	{
		std::string token;

		// TCP test
		serverSender.registerClient(token);

		// UDP test
		serverListener.run();

		// do something now...
		getchar();

		if (host)
		{
			//serverSender.endGame();
		}
		else
		{
			serverSender.endGame(); // TODO temporary
		}

		serverListener.wait();

		serverSender.leaveGame();

		common::Logger::getInstance().log("client application closing...");

	}
	catch (...)
	{
		common::Logger::getInstance().error("exception in client test");
	}
}

