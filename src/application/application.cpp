#include <application/application.h>
#include <common/logger.h>
#include <client/server_listener.h>
#include <server/server_tcp_listener.h>
#include <client/server_sender.h>
#include <iostream>
#include <string>
#include <server/server_udp_listener.h>
#include <server/application.h>
#include <client/application.h>
#include <unistd.h>

using namespace application;

Application::Application()
{
}

void Application::run()
{
	common::Logger& logger = common::Logger::getInstance();
	logger.log("starting application");

	//while (true)
	{
		int n;
		std::cout << "1 - client test" << std::endl
			<< "2 - server test" << std::endl
			<< "3 - quit" << std::endl;
		std::cin >> n;

		if (1 == n)
		{
			client::Application clientApp(false);
			clientApp.run();
			clientApp.wait();
		}
		else if (2 == n)
		{
			server::Application serverApp;
			serverApp.run();

			sleep(1);

			client::Application clientApp(true);

			clientApp.run();
		

			clientApp.wait();

			// TODO closing connection on server side
			//serverApp.wait();
		}
		else
		{
			//break;
		}
	}

}

