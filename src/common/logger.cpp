#include <common/logger.h>
#include <string>
#include <common/mutex.h>
#include <iostream>
#include <ctime>

using namespace std;
using namespace common;

Logger::Logger()
{
}

Logger& Logger::getInstance()
{
	static Logger logger;
	return logger;
}

void Logger::log(const string& s) const
{
	time_t curTime;
	time(&curTime);
	mutex.lock();
	cout << "LOG" << endl;
	cout << "\t" << ctime(&curTime);
	cout << "\t" << s << endl;
	mutex.unlock();
}

void Logger::debug(const string& s) const
{
	mutex.lock();
	cout << "DEBUG" << endl;
	cout << "\t" << s << endl;
	mutex.unlock();
}

void Logger::debug(const int x) const
{
	mutex.lock();
	cout << "DEBUG" << endl;
	cout << "\t" << x << endl;
	mutex.unlock();
}

void Logger::error(const std::string& s) const
{
	mutex.lock();
	cout << "ERROR" << endl;
	cout << "\t" << s << endl;
	mutex.unlock();
}
