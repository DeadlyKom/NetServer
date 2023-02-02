#pragma once

#include <CoreMinimal.h>

enum class EClientResponse
{
	UNKNOW,
	OK,
	FAIL,
	SHUTDOWN,
};

struct FClient
{
	FClient(FSocket _SocketTCP);

	void Startup();
	EClientResponse ResiveData();
	void Shutdown();

private:
	FSocket SocketTCP;
};
