#include "server/server_udp.h"
#include "common/isocket_handler.h"
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <cstdlib>
#include <cstring>
#include <string>

using namespace server;

ServerUDP::ServerUDP(const std::string& port)
	: common::ISocketHandler(), port(port)
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
	{
		throw SocketError();
		perror("socket error");
	}

	length = sizeof(server);

	bzero(&server,length);

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(atoi(port.c_str()));

	if (bind(sock, (struct sockaddr*)&server, length) == -1)
	{
		throw BindError();
		perror("bind error");
	}

	fromlen = sizeof(struct sockaddr_in);
}

ServerUDP::ServerUDP(const ServerUDP& server)
	: sock(server.sock), length(server.length), fromlen(server.fromlen), from(server.from), port(server.port)
{
}

ServerUDP& ServerUDP::operator=(const ServerUDP& server)
{
	ServerUDP tmp(server);
	swap(*this, tmp);
	return *this;
}

namespace server
{
void swap(ServerUDP& a, ServerUDP& b)
{
	std::swap(a.sock, b.sock);
	std::swap(a.length, b.length);
	std::swap(a.fromlen, b.fromlen);
	std::swap(a.server, b.server);
	std::swap(a.from, b.from);
	std::swap(a.port, b.port);
}
}

ServerUDP::~ServerUDP()
{
}

void ServerUDP::send(void* msg, size_t size) const
{
	if (sendto(sock, msg, size, 0, (struct sockaddr*)&from, fromlen) == -1)
	{
		perror("sendto error");
		throw SendError();
	}
}

void ServerUDP::receive(void* buf, size_t size) const
{
	if (recvfrom(sock, buf, size, 0, (struct sockaddr*)&from, &fromlen) == -1)
	{
		perror("recvfrom error");
		throw ReceiveError();
	}
}

