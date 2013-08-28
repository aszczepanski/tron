#include <server/server_tcp.h>
#include <common/isocket_handler.h>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <string>

using namespace server;

ServerTCP::ServerTCP(const std::string& port)
	: common::ISocketHandler(), in_sockfd(-1), port(port)
{
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("socket error");
		throw SocketError();
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port.c_str()));
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (sockaddr*)&addr, sizeof(addr)) == -1)
	{
		perror("bind error");
		throw BindError();
	}

	if (listen(sockfd, MAX_WAITING_SERVERS) == -1)
	{
		perror("listen error");
		throw ListenError();
	}

}

ServerTCP::ServerTCP(const ServerTCP& server)
	: common::ISocketHandler(), sockfd(server.sockfd),
	in_sockfd(server.in_sockfd), port(server.port)
{
}

namespace server
{
void swap(ServerTCP& a, ServerTCP& b)
{
	std::swap(a.sockfd, b.sockfd);
	std::swap(a.in_sockfd, b.in_sockfd);
	std::swap(a.port, b.port);
}
}

ServerTCP& ServerTCP::operator=(const ServerTCP& server)
{
	ServerTCP temp = ServerTCP(server);
	swap(*this, temp);
	return *this;
}

ServerTCP::~ServerTCP()
{
}

void ServerTCP::send(void* msg, size_t size) const
{
	mutex.lock();
	int st = write(in_sockfd, msg, size);
	mutex.unlock();
	if (-1 == st)
	{
		perror("write error");
		throw WriteError();
	}
}

void ServerTCP::receive(void* buf, size_t size) const
{
	mutex.lock();
	int st = read(in_sockfd, buf, size);
	mutex.unlock();
	if (-1 == st)
	{
		perror("read error");
		throw ReadError();
	}
}

void ServerTCP::closeSocket()
{
	close(in_sockfd);
}

void ServerTCP::closeConnection()
{
	close(sockfd);
}

ServerTCP ServerTCP::waitForSocket()
{
	socklen_t socklen;
	sockaddr_in in_addr;
	socklen = sizeof(in_addr);
	
	mutex.lock();
	int new_sockfd = accept(sockfd, (sockaddr*)&in_addr, &socklen);
	mutex.unlock();
	if (new_sockfd == -1)
	{
		perror("accept");
		throw AcceptError();
	}
	ServerTCP result = ServerTCP(*this);
	result.in_sockfd = new_sockfd;
	return result;
}
