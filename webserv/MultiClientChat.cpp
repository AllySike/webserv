#include "MultiClientChat.h"
#include <string>

MultiClientChat::MultiClientChat(const char* ipAddress, int port):
	TCPListener(ipAddress, port){}

void MultiClientChat::onClientConnected(SOCKET clientSocket)
{
	std::string welcomeMsg = "Welcome to Our Chat Server!\r\n";
	sendToClient(clientSocket, welcomeMsg.c_str(), (int)welcomeMsg.size() + 1);
}

void MultiClientChat::onCLiendDisconnected(SOCKET clientSocket)
{
	(void)clientSocket;
}

void MultiClientChat::onMessageReceived(SOCKET clientSocket, const char* msg, int len)
{
	broadcastToClients(clientSocket, msg, len);
}
