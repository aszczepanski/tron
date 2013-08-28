#include <application/application.h>
#include <common/logger.h>
#include <client/server_listener.h>
#include <server/server_tcp_listener.h>
#include <client/server_sender.h>
#include <iostream>
#include <string>
#include <server/server_udp_listener.h>

using namespace application;

Application::Application()
{
}

void Application::run()
{
	common::Logger& logger = common::Logger::getInstance();
	logger.log("starting application");

	int n;
	std::cout << "1 - client test\n"
		<< "2 - server test\n";
	std::cin >> n;

	if (n == 1)
	{
		// client test
		clientApplication();
	}
	else
	{
		// server test
		serverApplication();
	}

}

void Application::clientApplication()
{
	try
	{
		std::string token;

		// TCP test
		client::ServerSender ss("127.0.0.1", "6060");
		ss.registerClient(token);
		ss.sendTurn();

		// UDP test
		client::ServerListener sl("127.0.0.1", "6060");
		sl.run();
		sl.wait();

		ss.closeSocket();
	}
	catch (...)
	{
		common::Logger::getInstance().error("exception in client test");
	}
}

void Application::serverApplication()
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

		std::cout << "1 - client test\n"
			<< "2 - quit\n";
		std::cin >> n;
		if (n == 1)
		{
			// client inside server test
			clientApplication();
		}
//		else
//		{
//			cl.closeSocket();
//		}

		cl.wait();
		sl.wait();
	}
	catch (...)
	{
		common::Logger::getInstance().error("exception in server test");
	}
}
