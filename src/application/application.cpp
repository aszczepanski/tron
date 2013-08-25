#include <application/application.h>
#include <common/logger.h>
#include <client/server_listener.h>
#include <server/server_tcp_listener.h>
#include <iostream>

using namespace application;

Application::Application()
{
}

void Application::run()
{
	common::Logger::getInstance().log("application");

	int n;
	std::cin >> n;

	if (n == 1)
	{
		client::ServerListener sl("127.0.0.1", "6060");
		sl.run();
		sl.wait();
	}
	else
	{
		server::ServerTCPListener cl("6060");
		cl.run();
		cl.wait();
	}

}
