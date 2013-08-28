#include <client/client_tcp.h>
#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <cstdio>
#include <cassert>
#include <string>
using namespace std;
using namespace client;

ClientTCP::ClientTCP(const string& hostname, const string& port)
	: ISocketHandler(), hostname(hostname), port(port), connectionOpened(false)
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
	
	if (inet_aton(hostname.c_str(), (in_addr*)&addr.sin_addr.s_addr) == 0)
	{
		perror("aton error");
		throw AtonError();
	}

	cout << "TCP connecting" << endl;

	if (connect(sockfd, (sockaddr*)&addr, sizeof(addr)) == -1)
	{
		perror("connect error");
		throw ConnectError();
	}

	connectionOpened = true;
}

ClientTCP::~ClientTCP()
{
	if (connectionOpened)
	{
		std::cout << "sockfd = " << sockfd << endl;
		if (close(sockfd) < 0)
		{
			perror("close sockfd");
		}
	}
}

void ClientTCP::closeSocket()
{
	assert(connectionOpened);
	std::cout << "sockfd = " << sockfd << endl;
	if (close(sockfd) < 0)
	{
		perror("close sockfd");
	}
	connectionOpened = false;
}

void ClientTCP::send(void* msg, size_t size) const
{
	assert(connectionOpened);
	mutex.lock();
	int st = write(sockfd, msg, size);
	mutex.unlock();

	if (-1 == st)
	{
		perror("write error");
		throw WriteError();
	}
}

void ClientTCP::receive(void* buf, size_t size) const
{
	assert(connectionOpened);
	mutex.lock();
	int st = read(sockfd, buf, size);
	mutex.unlock();

	if (-1 == st)
	{
		perror("read error");
		throw ReadError();
	}
}
