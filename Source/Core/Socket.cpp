#include "Socket.h"
#include <CoreMinimal.h>

FSocket::FSocket()
	: Socket(SOCKET_ERROR)
{}

void FSocket::Startup()
{
	WIN(
	WSAData wsData;
	if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0)
	{
		std::cout << "Error WinSock version initializaion. Error #" << GetError() << std::endl;
		return;
	}
	else
	{
		std::cout << "WinSock initialization is OK" << std::endl;
	});
}

bool FSocket::InitializeTCP(std::string IPv4, uint16_t Port)
{
	SocketAdrIn Address;
	Address.sin_addr.WIN(S_un.S_addr)NIX(s_addr) = inet_addr(IPv4.c_str());
	Address.sin_port = htons(Port);
	Address.sin_family = AF_INET;

	if ((Socket = socket(AF_INET, SOCK_STREAM, 0)) WIN(== INVALID_SOCKET)NIX(== -1))
	{
		std::cout << "Error initialization socket. Error #" << GetError() << std::endl;
		return true;
	}

	if (bind(Socket, (sockaddr*)&Address, sizeof(Address)) WIN(== SOCKET_ERROR)NIX(< 0))
	{
		std::cout << "Error Socket binding info. Error #" << GetError() << std::endl;
		return true;
	}

	return false;
}

bool FSocket::InitializeUDP(std::string IPv4, uint16_t Port)
{
	SocketAdrIn Address;
	Address.sin_addr.WIN(S_un.S_addr)NIX(s_addr) = inet_addr(IPv4.c_str());
	Address.sin_port = htons(Port);
	Address.sin_family = AF_INET;

	if ((Socket = socket(AF_INET, SOCK_DGRAM, 0)) WIN(== INVALID_SOCKET)NIX(== -1))
	{
		std::cout << "Error initialization socket. Error #" << GetError() << std::endl;
		return true;
	}

	if (bind(Socket, (sockaddr*)&Address, sizeof(Address)) WIN(== SOCKET_ERROR)NIX(< 0))
	{
		std::cout << "Error Socket binding to info. Error #" << GetError() << std::endl;
		return true;
	}

	return false;
}

void FSocket::EnableBlockingMode(bool bNewValue /*= true*/)
{
	WIN(unsigned long Mode = !!bNewValue; ioctlsocket(Socket, FIONBIO, &Mode);)
	NIX(const int Flags = fcntl(Socket, F_GETFL, 0);  fcntl(Socket, F_SETFL, bNewValue ? Flags ^ O_NONBLOCK : Flags | O_NONBLOCK);)
}

bool FSocket::Listen(uint32_t Backlog /*= 1*/)
{
	if (listen(Socket, Backlog) WIN(== SOCKET_ERROR) NIX(< 0))
	{
		std::cout << "Error Socket listen. Error #" << GetError() << std::endl;
		return true;
	}
	return false;
}

SocketID FSocket::Accept(FSocket& OutputSocket)
{
	OutputSocket.AddressLength = sizeof(OutputSocket.Address);
	OutputSocket.Socket = accept(Socket, (sockaddr*)&OutputSocket.Address, &OutputSocket.AddressLength);
	if (Socket WIN(== INVALID_SOCKET) NIX(< 0))
	{
		std::cout << "Error Socket accept. Error #" << GetError() << std::endl;
		return INDEX_NONE;
	}
	return OutputSocket.Socket;
}

int32_t FSocket::ReceiveTCP(std::vector<uint8_t>& OutputReceivedData)
{
	int32_t BytesReceived = recv(Socket, &Buffer[0], BUFFER_SIZE, 0);
	for (int i = 0; i < BytesReceived; ++i)
	{
		OutputReceivedData.push_back(Buffer[i]);
	}
	return BytesReceived;
}

int32_t FSocket::ReceiveUDP(SocketAdrIn& SocketClient, std::queue<uint8_t> OutputReceivedData)
{
	int32_t SocketClientSize = sizeof(SocketClient);
	int32_t BytesReceived = recvfrom(Socket, &Buffer[0], BUFFER_SIZE, 0, (sockaddr*)&SocketClient, &SocketClientSize);
	if (BytesReceived WIN(== SOCKET_ERROR)NIX(< 0))
	{
		std::cout << "Error read UDP. Error #" << GetError() << std::endl;
		return 0;
	}
	for (int i = 0; i < BytesReceived; ++i)
	{
		OutputReceivedData.push(Buffer[i]);
	}
	return BytesReceived;
}

bool FSocket::SendTCP(const std::vector<uint8_t>& ResponseData)
{
	return send(Socket, reinterpret_cast<const char*>(ResponseData.data()), ResponseData.size(), 0) < 0;
}

void FSocket::Close()
{
	WIN(closesocket)NIX(close)(Socket);
	Socket = SOCKET_ERROR;
}

void FSocket::Shutdown()
{
	Close();
	WIN(WSACleanup());
}

int32_t FSocket::GetError()
{
	return WIN(WSAGetLastError())NIX(errno);
}
