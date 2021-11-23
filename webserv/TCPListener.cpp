#include "TCPListener.h"

TCPListener::TCPListener(const char* ipAddress, int port):
	_ipAddress(ipAddress), _port(port)
{
	// Create the master file descriptor set and zero it
	FD_ZERO(&this->_master);
	this->_socket = 0;
}

int TCPListener::init_listener()
{
	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
		return (wsOk);

	// Create a socket
	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket == INVALID_SOCKET)
		return (WSAGetLastError());

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(this->_port);
	inet_pton(AF_INET, this->_ipAddress, &hint.sin_addr);

	if (bind(this->_socket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR)
		return (WSAGetLastError());

	// Tell Winsock the socket is for listening 
	if (listen(this->_socket, SOMAXCONN) == SOCKET_ERROR)
		return (WSAGetLastError());

	// Add our first socket that we're interested in interacting with; the listening socket!
	// It's important that this socket is added for our server or else we won't 'hear' incoming
	// connections 
	FD_SET(this->_socket, &this->_master);

	return (0);
}

int TCPListener::run_listener()
{
	// this will be changed by the \quit command (see below, bonus not in video!)
	bool running = true;

	while (running)
	{
		fd_set copy = this->_master;

		// See who's talking to us
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		// Loop through all the current connections / potential connect
		for (int i = 0; i < socketCount; i++)
		{
			// Makes things easy for us doing this assignment
			SOCKET sock = copy.fd_array[i];

			// Is it an inbound communication?
			if (sock == this->_socket)
			{
				// Accept a new connection
				SOCKET client = accept(this->_socket, nullptr, nullptr);

				// Add the new connection to the list of connected clients
				FD_SET(client, &this->_master);

				onClientConnected(client);
			}
			else // It's an inbound message
			{
				char buf[4096];
				ZeroMemory(buf, 4096);

				// Receive message
				int bytesIn = recv(sock, buf, 4096, 0);
				if (bytesIn <= 0)
				{
					// Drop the client
					onCLiendDisconnected(sock);
					closesocket(sock);
					FD_CLR(sock, &this->_master);
				}
				else
				{
					onMessageReceived(sock, buf, bytesIn);
					// Check to see if it's a command. \quit kills the server
					//if (buf[0] == '\\')
					//{
					//	// Is the command quit? 
					//	std::string cmd = std::string(buf, bytesIn);
					//	if (cmd == "\\quit")
					//	{
					//		running = false;
					//		break;
					//	}

					//	// Unknown command
					//	continue;
					//}

					// Send message to other clients, and definiately NOT the listening socket
				}
			}
		}
	}

	// Remove the listening socket from the master file descriptor set and close it
	// to prevent anyone else trying to connect.
	FD_CLR(this->_socket, &this->_master);
	closesocket(this->_socket);

	//// Message to let users know what's happening.
	//std::string msg = "Server is shutting down. Goodbye\r\n";

	while (this->_master.fd_count > 0)
	{
		// Get the socket number
		SOCKET sock = this->_master.fd_array[0];

		//// Send the goodbye message
		//send(sock, msg.c_str(), msg.size() + 1, 0);

		// Remove it from the master file list and close the socket
		FD_CLR(sock, &this->_master);
		closesocket(sock);
	}

	// Cleanup winsock
	WSACleanup();
	return (0);
}

void TCPListener::sendToClient(SOCKET clientSocket, const char* msg, int len)
{
	send(clientSocket, msg, len, 0);
}

void TCPListener::broadcastToClients(SOCKET sendingCLient, const char* msg, int len)
{
	for (unsigned int i = 0; i < this->_master.fd_count; i++)
	{
		SOCKET outSock = this->_master.fd_array[i];
		if (outSock != this->_socket && outSock != sendingCLient)
			sendToClient((int)outSock, msg, len);
	}
}
void TCPListener::onClientConnected(SOCKET clientSocket)
{
	(void)clientSocket;
}

void TCPListener::onCLiendDisconnected(SOCKET clientSocket)
{
	(void)clientSocket;
}

void TCPListener::onMessageReceived(SOCKET clientSocket, const char* msg, int len)
{
	(void)clientSocket;
	(void)msg;
	(void)len;
}