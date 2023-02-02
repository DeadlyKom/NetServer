#pragma once

#ifdef _WIN32
#define WIN(exp) exp
#define NIX(exp)
#else
#define WIN(exp)
#define NIX(exp) exp
#endif

#include <fcntl.h>
#include <cstdint>
#include <string>
#include <vector>

#include <queue>	//?

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <mstcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#endif

#ifdef _WIN32
typedef int32_t SockLen_t;
typedef SOCKADDR_IN SocketAdrIn;
typedef SOCKET SocketID;
typedef u_long ka_prop_t;
#else
typedef socklen_t SockLen_t;
typedef struct sockaddr_in SocketAdrIn;
typedef int32_t SocketID;
typedef int32_t ka_prop_t;
#endif

#define BUFFER_SIZE 1024

class FSocket
{
public:
	FSocket();
	void Startup();
	bool InitializeTCP(std::string IPv4, uint16_t Port);
	bool InitializeUDP(std::string IPv4, uint16_t Port);
	void EnableBlockingMode(bool bNewValue = true);
	bool Listen(uint32_t Backlog = 1);
	SocketID Accept(FSocket& OutputSocket);
	SocketID GetSocket() const { return Socket; }
	int32_t ReceiveTCP(std::vector<uint8_t>& OutputReceivedData);
	int32_t ReceiveUDP(SocketAdrIn& SocketClient, std::queue<uint8_t> OutputReceivedData);
	bool SendTCP(const std::vector<uint8_t>& ResponseData);
	void Close();
	void Shutdown();

private:
	int32_t GetError();

	char Buffer[BUFFER_SIZE];
	int32_t AddressLength;
	SocketAdrIn Address;
	SocketID Socket;
};
