#include "Client.h"
#include <Commands.h>
#include "Server.h"

FClient::FClient(FSocket _SocketTCP)
	: SocketTCP(_SocketTCP)
{}

void FClient::Startup()
{

}

EClientResponse FClient::ResiveData()
{
	std::vector<uint8_t> ReceivedData;
	if (SocketTCP.ReceiveTCP(ReceivedData) <= 0)
	{
		Shutdown();
		return EClientResponse::SHUTDOWN;
	}

	FServerRequest RT;
	std::vector<uint8_t> ResponseData;
	EClientResponse Result = EClientResponse::OK;

	if (ServerCommands::Validation(ReceivedData, RT))
	{
		Result = EClientResponse::FAIL;
	}

	if (Result == EClientResponse::OK)
	{
		Result = ServerCommands::Execution(this, RT, ResponseData) ? EClientResponse::FAIL : EClientResponse::OK;
	}

	if (Result != EClientResponse::OK)
	{
		ResponseData = ServerCommands::ResponseErrorData(EServerResponse::BAD_REQUEST);
	}

	SocketTCP.SendTCP(ResponseData);
	return Result;
}

void FClient::Shutdown()
{
	SocketTCP.Close();
}
