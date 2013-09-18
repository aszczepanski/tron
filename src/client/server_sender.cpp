#include <client/server_sender.h>
#include <iostream>
#include <string>
#include <cstring>
#include <common/protocol.h>
#include <client/shared_memory.h>
#include <client/client_tcp.h>

using namespace client;

common::Mutex ServerSender::mutex;

ServerSender::ServerSender(SharedMemory& sharedMemory, ClientTCP& client)
	: client(client), sharedMemory(sharedMemory)
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
	if (!sharedMemory.getEnd())
	{
		client.send(&request, sizeof(REQUEST));
	}
	mutex.unlock();
}

void ServerSender::endGame()
{
	REQUEST request;
	bzero(&request, sizeof(REQUEST));
	request.request_type = REQUEST::END_GAME;
	request.length = 0;

	mutex.lock();
	if (!sharedMemory.getEnd())
	{
		client.send(&request, sizeof(REQUEST));
	}
	mutex.unlock();
}

void ServerSender::leaveGame()
{
	std::cout << "aaaa\n";
	REQUEST request;
	bzero(&request, sizeof(REQUEST));
	request.request_type = REQUEST::LEAVE_GAME;
	request.length = TOKEN_SIZE;

	std::string token = sharedMemory.getToken();

	mutex.lock();
	if (!sharedMemory.getEnd())
	{
		client.send(&request, sizeof(REQUEST));
		for (int i=0; i<TOKEN_SIZE; i++)
		{
			client.send(&token[i], 1);
		}
	}
	mutex.unlock();
	std::cout << "bbbb\n";
}

void ServerSender::registerClient()
{
	std::string token;
	REQUEST request;
	bzero(&request, sizeof(REQUEST));
	request.request_type = REQUEST::REGISTER_TOKEN;

	mutex.lock();
	if (!sharedMemory.getEnd())
	{
		client.send(&request, sizeof(REQUEST));
	}
	mutex.unlock();

}

void ServerSender::sendTurn(common::Direction direction)
{
	REQUEST request;
	request.request_type = REQUEST::NEW_TURN;

	mutex.lock();
	if (!sharedMemory.getEnd())
	{
		client.send(&request, sizeof(REQUEST));
		client.send(&direction, sizeof(common::Direction));
	}
	mutex.unlock();
}

void ServerSender::getStageInfo()
{
	REQUEST request;
	memset(&request, 0, sizeof(REQUEST));
	request.request_type = REQUEST::STAGE_INFO;
	request.length = 0;

	mutex.lock();
	if (!sharedMemory.getEnd())
	{
		client.send(&request, sizeof(REQUEST));
	}
	mutex.unlock();
}
