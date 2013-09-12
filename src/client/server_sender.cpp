#include <client/server_sender.h>
#include <iostream>
#include <string>
#include <cstring>
#include <common/protocol.h>
#include <client/shared_memory.h>

using namespace client;

common::Mutex ServerSender::mutex;

ServerSender::ServerSender(const std::string& hostname, const std::string& port, SharedMemory& sharedMemory)
	: client(hostname, port), sharedMemory(sharedMemory)
{
}

void ServerSender::closeSocket()
{
	client.closeSocket();
}

void ServerSender::startGame()
{
	REQUEST request;
	memset(&request, 0, sizeof(REQUEST));
	request.request_type = REQUEST::START_GAME;
	request.length = 0; // TODO here token

	mutex.lock();
	client.send(&request, sizeof(REQUEST));
	mutex.unlock();
}

void ServerSender::endGame()
{
	REQUEST request;
	bzero(&request, sizeof(REQUEST));
	request.request_type = REQUEST::END_GAME;
	request.length = 0;

	mutex.lock();
	client.send(&request, sizeof(REQUEST));
	mutex.unlock();
}

void ServerSender::leaveGame()
{
	REQUEST request;
	bzero(&request, sizeof(REQUEST));
	request.request_type = REQUEST::LEAVE_GAME;
	request.length = TOKEN_SIZE;

	std::string token = sharedMemory.getToken();

	mutex.lock();
	client.send(&request, sizeof(REQUEST));
	for (int i=0; i<TOKEN_SIZE; i++)
	{
		client.send(&token[i], 1);
	}
	mutex.unlock();
}

void ServerSender::registerClient()
{
	std::string token;
	REQUEST request;
	bzero(&request, sizeof(REQUEST));
	request.request_type = REQUEST::REGISTER_TOKEN;

	mutex.lock();
	client.send(&request, sizeof(REQUEST));
	mutex.unlock();

	bzero(&request, sizeof(REQUEST));
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

	sharedMemory.setToken(token);
}

void ServerSender::sendTurn(common::Direction direction)
{
	REQUEST request;
	request.request_type = REQUEST::NEW_TURN;

	mutex.lock();
	client.send(&request, sizeof(REQUEST));
	client.send(&direction, sizeof(common::Direction));
	mutex.unlock();
}

void ServerSender::sendByte(unsigned char c)
{
	REQUEST request;
	memset(&request, 0, sizeof(REQUEST));
	request.request_type = REQUEST::NEW_TURN;
	request.length = 1;

	mutex.lock();
	client.send(&request, sizeof(REQUEST));
	client.send(&c, 1);
	mutex.unlock();
}

void ServerSender::getStageInfo()
{
	REQUEST request;
	memset(&request, 0, sizeof(REQUEST));
	request.request_type = REQUEST::STAGE_INFO;
	request.length = 0;

	mutex.lock();
	client.send(&request, sizeof(REQUEST));
	mutex.unlock();
}
