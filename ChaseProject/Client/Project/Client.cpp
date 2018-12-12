#include "Client.h"
#pragma comment(lib, "ws2_32.lib")

Client::Client()
{
	ipAddress = "149.153.106.163";			// IP Address of the server
	port = 54000;						// Listening port # on the server

										// Initialize WinSock
	ver = MAKEWORD(2, 2);
	wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		std::cerr << "Can't start Winsock, Err #" << wsResult << std::endl;
		return;
	}

	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
		return;
	}

	// Fill in a hint structure
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

}

Client::~Client()
{

}

void Client::init()
{
	ipAddress = "149.153.106.163";			// IP Address of the server
	port = 54000;						// Listening port # on the server

											// Initialize WinSock
	ver = MAKEWORD(2, 2);
	wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		std::cerr << "Can't start Winsock, Err #" << wsResult << std::endl;
		return;
	}

	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
		return;
	}

	// Fill in a hint structure
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	
}

bool Client::run()
{
	// Connect to server
	connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return false;
	}
	u_long iMode = 1;
	ioctlsocket(sock, FIONBIO, &iMode);
	return true;
}

std::string Client::receive()
{
	ZeroMemory(buf, 4096);
	int bytesReceived = recv(sock, buf, 4096, 0);
	if (bytesReceived > 0)
	{
		// Echo response to console
		if (buf == "Welcome to the Awesome Chat Server Player 1!")
		{
			std::cout << "SERVER> " << std::string(buf, 0, bytesReceived) << std::endl;
		}
		return buf;
	}
	return "";
}

void Client::sendMsg(std::string msg)
{
	int sendResult = send(sock, msg.c_str(), msg.size() + 1, 0);
}

void Client::close()
{
	closesocket(sock);
	WSACleanup();
}

int Client::getSock()
{
	return sock;
}