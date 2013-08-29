#include <client/server_sender.h>
#include <iostream>
#include <string>
#include <cstring>
#include <common/protocol.h>

using namespace client;

ServerSender::ServerSender(const std::string& hostname, const std::string& port)
	: client(hostname, port)
{
}

void ServerSender::closeSocket()
{
	client.closeSocket();
}

void ServerSender::endGame()
{
	REQUEST request;
	bzero(&request, sizeof(REQUEST));
	request.request_type = REQUEST::END_GAME;
	request.length = 0;

	client.send(&request, sizeof(REQUEST));
}

void ServerSender::leaveGame()
{
	REQUEST request;
	bzero(&request, sizeof(REQUEST));
	request.request_type = REQUEST::LEAVE_GAME;
	request.length = 0;

	client.send(&request, sizeof(REQUEST));
}

void ServerSender::registerClient(std::string& token)
{
	REQUEST request;
	bzero(&request, sizeof(REQUEST));
	request.request_type = REQUEST::REGISTER_TOKEN;

	client.receive(&request, sizeof(REQUEST));

	std::cout << "\t\t" << request.length << std::endl;

	token.clear();

	unsigned char byte;
	for (int i=0; i<request.length; i++)
	{
		client.receive(&byte, 1);
		token += byte;
	}

	std::cout << "\t\t" << token << std::endl;
}

void ServerSender::sendTurn()
{
	char msg[] = "a";
	client.send(msg, strlen(msg));
}
