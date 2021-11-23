#pragma once
#include "TCPListener.h"
class MultiClientChat : public TCPListener
{
	public:
		MultiClientChat(const char* ipAddress, int port);
		
	protected:
		virtual void onClientConnected(SOCKET clientSocket);
		virtual void onCLiendDisconnected(SOCKET clientSocket);
		virtual void onMessageReceived(SOCKET clientSocket, const char* msg, int len);
};

