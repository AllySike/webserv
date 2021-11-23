#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <string>
#include <sstream>
#include <winsock2.h>
#include <ws2tcpip.h>
class TCPListener
{
	public:
		TCPListener(const char* ipAddress, int port);

		int init_listener();
		int run_listener();

	protected:
		virtual void onClientConnected(SOCKET clientSocket);
		virtual void onCLiendDisconnected(SOCKET clientSocket);
		virtual void onMessageReceived(SOCKET clientSocket, const char* msg, int len);
		void sendToClient(SOCKET ClientSocket, const char* msg, int len);
		void broadcastToClients(SOCKET sendingCLient, const char* msg, int len);

	private:
		const char*	_ipAddress; //IP Address the server will run on
		SOCKET			_socket; //internal fd for the listening socket
		int			_port; //port for the service
		fd_set		_master; //master file descriptor set
};

