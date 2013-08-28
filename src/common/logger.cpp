#include <common/logger.h>
#include <string>
#include <common/mutex.h>
#include <iostream>

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
	mutex.lock();
	cout << "LOG" << endl;
	cout << "\t" << __TIME__ << endl;
	cout << "\t" << s << endl;
	mutex.unlock();
}

void Logger::debug(const string& s) const
{
	mutex.lock();
	cout << "DEBUG" << endl;
	cout << "\t" << __FILE__ << endl;
	cout << "\t" "line: " << __LINE__ << endl;
	cout << "\t" << s << endl;
	mutex.unlock();
}

void Logger::debug(const int x) const
{
	mutex.lock();
	cout << "DEBUG" << endl;
	cout << "\t" << __FILE__ << endl;
	cout << "\t" "line: " << __LINE__ << endl;
	cout << "\t" << x << endl;
	mutex.unlock();
}

void Logger::error(const std::string&) const
{
	mutex.lock();
	cout << "ERROR" << endl;
	cout << "\t" << __FILE__ << endl;
	cout << "\t" "line: " << __LINE__ << endl;
	mutex.unlock();
}
