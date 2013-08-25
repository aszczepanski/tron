#include "application/application.h"
#include "common/logger.h"

using namespace application;

Application::Application()
{
}

void Application::run()
{
	int x = 3;
	common::Logger::getInstance().log("application");
	common::Logger::getInstance().debug(x);
}
