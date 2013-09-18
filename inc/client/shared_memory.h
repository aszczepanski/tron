#ifndef CLIENT_SHARED_MEMORY_H
#define CLIENT_SHARED_MEMORY_H

#include <string>
#include <utility>
#include <vector>
#include <common/player_data.h>
#include <common/protocol.h>
#include <common/mutex.h>
#include <map>

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
	void keyUp();
	void keyDown();
	void keyEsc();
	bool isHost() const;
	void setHost(bool host);

	void keyStart();

	void leaveGame();

	bool setEnd();
	bool getEnd() const;

	void getPositions(std::vector<PLAYER_INFO>&) const;
	void addPosition(PLAYER_INFO);

	void getTurns(std::vector<TURN_INFO>&) const;
	void addTurn(TURN_INFO);

	void getCrashes(std::vector<CRASH_INFO>&) const;
	void addCrash(CRASH_INFO);

	void setPlayerNr(const int nr);
	void getPlayerNr(int& nr);
private:
	SharedMemory();
	SharedMemory(const SharedMemory&);
	SharedMemory& operator= (const SharedMemory&);
	std::string token;
	ServerSender* serverSender;
	unsigned short menuPos;
	bool host;

	bool end;

	mutable common::Mutex mutex;

	std::map< int,PLAYER_INFO > positions;
	std::vector< TURN_INFO > turns;
	std::vector< CRASH_INFO > crashes;

	int nr;
};

}

#endif // CLIENT_SHARED_MEMORY_H
