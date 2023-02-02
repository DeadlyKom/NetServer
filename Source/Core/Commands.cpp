#include "Commands.h"
#include <nlohmann/json.hpp>

#include "Client.h"
#include "Server.h"

namespace ResponseError
{
	static const std::string OK = "OK\n";
	static const std::string FAIL = "FAIL\n";
	static const std::string CREATED = "CREATED\n";
	static const std::string BAD_REQUEST = "BAD_REQUEST\n";
	static const std::string UNKNOW = "UNKNOW\n";
}

namespace Server
{
	struct FVersion
	{
		uint8_t Major = 0;
		uint8_t Minor = 0;
		uint8_t Maintenance = 1;
	};
	static const FVersion Version;
}

bool ServerCommands::Validation(const std::vector<uint8_t>& ReceivedData, FServerRequest& OutputServerRequest)
{
	nlohmann::json Json = nlohmann::json::parse(ReceivedData, nullptr, false);
	if (Json.is_discarded())
	{
		return true;
	}

	const bool bMethodType = Json["MT"].is_null() || !Json["MT"].is_number_integer();
	if (!bMethodType)
	{
		OutputServerRequest.MT = Json["MT"].get<EMethodRequest>();
	}
	const bool bCommandType = Json["CT"].is_null() || !Json["CT"].is_number_integer();
	if (!bCommandType)
	{
		OutputServerRequest.CT = Json["CT"].get<ECommandRequest>();
	}
	const bool bResponseType = Json["RT"].is_null() || !Json["RT"].is_number_integer();
	if (!bResponseType)
	{
		OutputServerRequest.RT = Json["RT"].get<EResponseType>();
	}

	return bMethodType || bCommandType || bResponseType;
}

bool ServerCommands::Execution(FClient* Client, const FServerRequest& ServerRequest, std::vector<uint8_t>& OutputResponseData)
{
	switch (ServerRequest.CT)
	{
	case ECommandRequest::SERVER: return Server(Client, ServerRequest, OutputResponseData);
	case ECommandRequest::ROOM:
		break;
	default:
		break;
	}
	return false;
}

bool ServerCommands::Server(FClient* Client, const FServerRequest& ServerRequest, std::vector<uint8_t>& OutputResponseData)
{
	switch (ServerRequest.MT)
	{
	case EMethodRequest::GET:  ServerVersion(OutputResponseData, ServerRequest.RT, false); break;
	case EMethodRequest::HEAD: ServerVersion(OutputResponseData, ServerRequest.RT, true);  break;
	case EMethodRequest::PUT:
	case EMethodRequest::POST:
	case EMethodRequest::DEL:
	default:
		return true; // bad request
	}
	return false;
}

void ServerCommands::ServerVersion(std::vector<uint8_t>& OutputResponseData, EResponseType RT, bool bHand)
{
	nlohmann::json JsonVersion;
	JsonVersion["Major"] = Server::Version.Major;
	JsonVersion["Minor"] = Server::Version.Minor;
	JsonVersion["Maintenance"] = Server::Version.Maintenance;

	switch (RT)
	{
	case EResponseType::JSON:
		if (bHand)
		{
			OutputResponseData = Utils::ToData(Utils::Format("%i\n", to_string(JsonVersion).size()));
		}
		else
		{
			OutputResponseData = Utils::ToData(to_string(JsonVersion));
		}
		break;
	case EResponseType::BIN:
		if (bHand)
		{
			OutputResponseData.push_back(sizeof(Server::FVersion));
		}
		else
		{
			OutputResponseData.push_back(Server::Version.Major);
			OutputResponseData.push_back(Server::Version.Minor);
			OutputResponseData.push_back(Server::Version.Maintenance);
		}
		break;
	default:
		break;
	}
}

void ServerCommands::CreateRoom()
{}

const std::string& ServerCommands::ResponseErrorString(EServerResponse ServerResponse)
{
	switch (ServerResponse)
	{
	case EServerResponse::OK:			return ResponseError::OK;
	case EServerResponse::FAIL:			return ResponseError::FAIL;
	case EServerResponse::CREATED:		return ResponseError::CREATED;
	case EServerResponse::BAD_REQUEST:	return ResponseError::BAD_REQUEST;
	default:							return ResponseError::UNKNOW;
	}
}

std::vector<uint8_t> ServerCommands::ResponseErrorData(EServerResponse ServerResponse)
{
	return Utils::ToData(ServerCommands::ResponseErrorString(ServerResponse));
}

