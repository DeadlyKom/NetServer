#include "Server.h"

namespace
{
	static std::string IPv4 = "192.168.0.101";
}

FServer::FServer()
	: ServerPort(80)
{}

FServer::~FServer()
{}

FServer& FServer::Get()
{
	static std::shared_ptr<FServer> InstanceAppFramework(new FServer());
	return *InstanceAppFramework.get();
}

int32_t FServer::Launch(const std::vector<std::string>& Args)
{
	Startup(Args);

	bListen = !Initialize();
	if (!bListen)
	{
		Release();
		return 1;
	}
	Listen();
	Release();
	return 0;
}

void FServer::Release()
{
	Shutdown();
}

void FServer::Startup(const std::vector<std::string>& Args)
{}

bool FServer::Initialize()
{
	ServerSocket.Startup();
	if (ServerSocket.InitializeTCP(IPv4, ServerPort))
	{
		return true;
	}

	ServerSocket.EnableBlockingMode();
	return false;
}

void FServer::Shutdown()
{
	bListen = false;
	ServerSocket.Shutdown();
}

void FServer::Listen()
{
	fd_set Active, Read;
	ServerSocket.Listen(MAX_CONNECTIONS);

	// initialize the set of active sockets
	Clients.clear();
	FD_ZERO(&Active);
	for (int32_t i = 0; i < FD_SETSIZE; ++i)
	{
		Active.fd_array[i] = SOCKET_ERROR;
	}
	FD_SET(ServerSocket.GetSocket(), &Active);

	while (bListen)
	{
		//timeval Timeout;
		//Timeout.tv_sec = 15;
		//Timeout.tv_usec = 0;

		// block until input arrives on one or more active sockets
		Read = Active;
		int32_t SocketNum = select(FD_SETSIZE, &Read, NULL, NULL, NULL/*&Timeout*/);
		if (SocketNum WIN(== SOCKET_ERROR)NIX(< 0))
		{
			bListen = false;
			continue;
		}

		// service all the sockets with input pending
		for (int32_t i = 0; i < SocketNum; ++i)
		{
			// check if event is a server
			if (FD_ISSET(ServerSocket.GetSocket(), &Read))
			{
				// connection request on server socket
				FSocket NewSocket;
				if (ServerSocket.Accept(NewSocket) == INDEX_NONE)
				{
					continue;
				}

				FD_SET(NewSocket.GetSocket(), &Active);
				FClient NewClient(NewSocket);
				Clients.try_emplace(NewSocket.GetSocket(), NewClient);
			}
			else
			{
				// data arriving on an already-connected socket
				std::map<SocketID, FClient>::iterator ItClient = Clients.find(Read.fd_array[i]);
				if (ItClient == Clients.end())
				{
					continue;
				}

				if (ItClient->second.ResiveData() == EClientResponse::SHUTDOWN)
				{
					FD_CLR(Read.fd_array[i], &Active);
				}
			}
		}
	}
}

//void FServer::Listen()
//{
//	while (bListen)
//	{
//		SocketAdrIn SocketClient;
//		if (ServerSocket.Receive(SocketClient) > 0)
//		{
//			// adding a client to the general list
//			const ULONG& HostAddress = SocketClient.sin_addr.s_addr;
//			if (Clients.find(HostAddress) != Clients.end())
//			{
//				FClient NewClient(SocketClient, inet_ntoa(SocketClient.sin_addr));
//				Clients.emplace(HostAddress, NewClient);
//			}
//		}
//		//SocketAdrIn ClientAdr;
//		//int32_t ClientAdrSize = sizeof(ClientAdr);
//		//int32_t BytesReceived = recvfrom(ServerSocket, &Buffer[0], BUFFER_SIZE, 0, (sockaddr*)&ClientAdr, &ClientAdrSize);
//		//if (BytesReceived WIN(!= SOCKET_ERROR)NIX(> 0))
//		//{
//		//	// adding a client to the general list
//		//	const ULONG& HostAddress = ClientAdr.sin_addr.s_addr;
//		//	if (Clients.find(HostAddress) != Clients.end())
//		//	{
//		//		FClient NewClient(ClientAdr, inet_ntoa(ClientAdr.sin_addr));
//		//		Clients.emplace(HostAddress, NewClient);
//		//	}
//		//	
//		//	std::lock_guard<std::mutex> Guard(M);
//		//	for (int i = 0; i < (int)BytesReceived; ++i)
//		//	{
//		//		ReceivedData.push(Buffer[i]);
//		//	}
//		//	Buffer[BytesReceived] = 0;
//		//	std::cout << NameIP << " " << Buffer << " ," << BytesReceived << std::endl;
//		//	std::string Response = "=OK=";
//		//	int32_t BytesSent = sendto(ServerSocket, Response.c_str(), Response.size(), 0, (sockaddr*)&ClientAdr, ClientAdrSize);
//		//	if (BytesSent WIN(== SOCKET_ERROR)NIX(< 0))
//		//	{
//		//		int a = 10;
//		//	}
//		//}
//	}
//}
