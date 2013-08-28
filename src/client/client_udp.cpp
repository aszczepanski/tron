#include "client/client_udp.h"
#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <cstdio>
#include <string>
#include <cstring>
using namespace std;
using namespace client;

ClientUDP::ClientUDP(const string& hostname, const string& port)
	: ISocketHandler(), hostname(hostname), port(port)
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
	{
		perror("socket error");
		throw SocketError();
	}
	
	server.sin_family = AF_INET;
	hp = gethostbyname(hostname.c_str());
	if (hp == 0)
	{
		perror("Unknown host");
		throw UnknownHostError();
	}

	bcopy((char*)hp->h_addr, (char*)&server.sin_addr, hp->h_length);
	server.sin_port = htons(atoi(port.c_str()));
	length = sizeof(struct sockaddr_in);
}

ClientUDP::~ClientUDP()
{
}

void ClientUDP::closeSocket()
{
	close(sock);
}

void ClientUDP::send(void* msg, size_t size) const
{
	mutex.lock();
	int st = sendto(sock, msg, size, 0, (const struct sockaddr*)&server, length);
	mutex.unlock();
	if (-1 == st)
	{
		perror("sendto error");
		throw WriteError();
	}
}

void ClientUDP::receive(void* buf, size_t size) const
{
	mutex.lock();
	int st = recvfrom(sock, buf, size, 0, (struct sockaddr*)&from, &length);
	mutex.unlock();
	if (-1 == st)
	{
		buf = NULL;
		perror("recvfrom error");
		throw ReadError();
	}
}
