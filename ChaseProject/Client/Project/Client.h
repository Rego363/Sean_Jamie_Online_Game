#pragma once

#include <iostream>
#include <string>
#include <WS2tcpip.h>

class Client
{
public:
	Client();
	~Client();
	void init();
	bool run();
	std::string receive();
	void sendMsg(std::string msg);
	void close();
	int getSock();

private:
	std::string ipAddress;
	int port;
	WSAData data;
	WORD ver;
	int wsResult;
	SOCKET sock; 
	sockaddr_in hint;
	int connResult;
	char buf[4096];
	std::string userInput;
};