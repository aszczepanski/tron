#ifndef CLIENT_SHARED_MEMORY_H
#define CLIENT_SHARED_MEMORY_H

#include <string>

namespace client
{

class ServerSender;

class SharedMemory
{
public:
	static SharedMemory& getInstance();
	void setServerSender(ServerSender*);
	void setToken(const std::string& token);
	std::string getToken() const;
	void keyLeft();
	void keyRight();
	void keyEsc();
	bool isHost() const;
	void setHost(bool host);
private:
	SharedMemory();
	SharedMemory(const SharedMemory&);
	SharedMemory& operator= (const SharedMemory&);
	std::string token;
	ServerSender* serverSender;
	unsigned short menuPos;
	bool host;
};

}

#endif // CLIENT_SHARED_MEMORY_H
