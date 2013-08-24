#include "client/client_tcp.h"
#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <cstdio>
#include <cassert>
using namespace std;
using namespace client;

ClientTCP::ClientTCP(const char* hostname, const char* port)
	: ISocketHandler(), hostname(hostname), port(port), connectionOpened(false)
{
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("socket error");
		exit(0);
	}
	
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	
	if (inet_aton(hostname, (in_addr*)&addr.sin_addr.s_addr) == 0)
	{
		perror("aton error");
		exit(0);
	}

	cout << "TCP connecting" << endl;

	if (connect(sockfd, (sockaddr*)&addr, sizeof(addr)) == -1)
	{
		perror("connect error");
		exit(0);
	}

	connectionOpened = true;
}

ClientTCP::~ClientTCP()
{
	mutex.lock();
	if (connectionOpened)
	{
		close(sockfd);
	}
	mutex.unlock();
}

void ClientTCP::closeConnection()
{
	assert(connectionOpened);
	close(sockfd);
}

void ClientTCP::send(void* msg, size_t size)
{
	assert(connectionOpened);
	mutex.lock();
	int st = write(sockfd, msg, size);
	mutex.unlock();

	if (-1 == st)
	{
		perror("write error");
		exit(0);
	}
}

void ClientTCP::receive(void* buf, size_t size)
{
	assert(connectionOpened);
	mutex.lock();
	int st = read(sockfd, buf, size);
	mutex.unlock();

	if (-1 == st)
	{
		perror("read error");
		exit(0);
	}
}
