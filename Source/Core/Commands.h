#pragma once

#include <CoreMinimal.h>

enum class EMethodRequest : uint8_t
{
	GET			= 0x00,		// позволяет запросить конкретный ресурс
	POST		= 0x01,		// отправка данных на сервер (например, заполненной формы логина)
	HEAD		= 0x02,		// запрос размер запрашиваемого ресурса перед его GET-запросом
	PUT			= 0x03,		// используется для создания/размещения новых ресурсов на сервере
	DEL			= 0x04,		// позволяет удалить существующие ресурсы на сервере

	UNKNOW		= 0xFF
};

enum class ECommandRequest : uint16_t
{
	SERVER		= 0x0000,
	ROOM		= 0x0001,

	UNKNOW		= 0xFFFF
};

enum class EResponseType : uint8_t
{
	JSON		= 0x00,		// текстовый вид ответа
	BIN			= 0x01,		// бинарный вид ответа

	UNKNOW		= 0xFF
};

struct FServerRequest
{
	EMethodRequest MT = EMethodRequest::UNKNOW;
	ECommandRequest CT = ECommandRequest::UNKNOW;
	EResponseType RT = EResponseType::UNKNOW;
};

struct FClient;
enum class EServerResponse;

namespace ServerCommands
{
	bool Validation(const std::vector<uint8_t>& ReceivedData, FServerRequest& OutputServerRequest);
	bool Execution(FClient* Client, const FServerRequest& ServerRequest, std::vector<uint8_t>& OutputResponseData);
	bool Server(FClient* Client, const FServerRequest& ServerRequest, std::vector<uint8_t>& OutputResponseData);
	void ServerVersion(std::vector<uint8_t>& OutputResponseData, EResponseType RT, bool bHand);
	void CreateRoom();

	const std::string& ResponseErrorString(EServerResponse ServerResponse);
	std::vector<uint8_t> ResponseErrorData(EServerResponse ServerResponse);
}
