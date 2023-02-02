#pragma once

#include <mutex>
#include <queue>
#include <thread>

#include <Room.h>
#include <Client.h>
#include <Commands.h>
#include <CoreMinimal.h>

#define MAX_CONNECTIONS 10

enum class EServerResponse
{
	OK,
	FAIL,
	CREATED,
	BAD_REQUEST,
};

class FServer : public std::enable_shared_from_this<FServer>
{
public:
	FServer();
	virtual ~FServer();
	static FServer& Get();

	virtual int32_t Launch(const std::vector<std::string>& Args);
	virtual void Release();

	virtual void Startup(const  std::vector<std::string>& Args);
	virtual bool Initialize();
	virtual void Shutdown();

protected:
	void Listen();

	uint16_t ServerPort;
	FSocket ServerSocket;

	bool bListen;
	std::map<SocketID, FClient> Clients;
};
