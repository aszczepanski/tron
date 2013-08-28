#include <client/server_sender.h>
#include <iostream>
#include <string>
#include <cstring>

using namespace client;

ServerSender::ServerSender(const std::string& hostname, const std::string& port)
	: client(hostname, port)
{
}

void ServerSender::sendTurn()
{
	char msg[] = "a";
	client.send(msg, strlen(msg));

	client.closeSocket();
}
