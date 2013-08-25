#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <common/mutex.h>

namespace common
{

class Logger
{
public:
	static Logger& getInstance();
	void log(const std::string&) const;
	void debug(const std::string&) const;
	void debug(const int x) const;
private:
	Logger();
	Logger(const Logger&);
	Logger& operator=(const Logger&);

	mutable Mutex mutex;
};

}

#endif // LOGGER_H
