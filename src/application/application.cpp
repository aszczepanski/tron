#include <application/application.h>
#include <common/logger.h>
#include <client/server_listener.h>
#include <server/server_tcp_listener.h>
#include <client/server_sender.h>
#include <iostream>
#include <server/server_udp_listener.h>

using namespace application;

Application::Application()
{
}

void Application::run()
{
	common::Logger& logger = common::Logger::getInstance();
	logger.log("application");

	int n;
	std::cin >> n;

	if (n == 1)
	{
		try
		{
			// TCP test
			client::ServerSender ss("127.0.0.1", "6060");
			ss.sendTurn();

			// UDP test
			client::ServerListener sl("127.0.0.1", "6060");
			sl.run();
			sl.wait();
		}
		catch (...)
		{
			logger.error("exception in client test");
		}
	}
	else
	{
		try
		{
			// TCP test
			server::ServerTCPListener cl("6060");
			cl.run();

			// UDP test
			server::ServerUDPListener sl("6060");
			sl.run();

			cl.wait();
			sl.wait();
		}
		catch (...)
		{
			logger.error("exception in server test");
		}
	}

}
