#include <application/application.h>
#include <common/logger.h>
#include <client/server_listener.h>
#include <server/server_tcp_listener.h>
#include <client/server_sender.h>
#include <iostream>
#include <string>
#include <server/application.h>
#include <client/application.h>
#include <client/open_gl_main.h>
#include <client/shared_memory.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
			client::Application clientApp(false, "192.168.0.108");
			clientApp.run();

			client::OpenGLMain ogl(client::SharedMemory::getInstance());
			ogl.start();

			clientApp.wait();
		}
		else if (2 == n)
		{
			if (fork() == 0) {
				// TODO no need to create seperate thread
				server::Application serverApp;
				serverApp.run();
				serverApp.wait();
			}
			else
			{
				// required to give time for server to prepare
				// rly bad method ;p
				sleep(1);

				client::Application clientApp(true);

				clientApp.run();

				// not sure if this is the best idea but works
				// (should be in seperate process using unix fork,
				// shared memory and semaphores mechanism)
				client::OpenGLMain ogl(client::SharedMemory::getInstance());
				ogl.start();

				clientApp.wait();

				// TODO closing connection on server side
				std::cout << "here1\n";

				std::cout << "waiting for server...\n";

				wait(NULL);
				std::cout << "here2\n";
			}
		}
		else
		{
			//break;
		}
	}

}

