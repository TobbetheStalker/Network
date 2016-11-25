#include "ClientNetwork.h"

ClientNetwork::ClientNetwork()
{
}

ClientNetwork::~ClientNetwork()
{
}

bool ClientNetwork::Initialize()
{
	// create WSADATA object
	WSADATA wsaData;
	this->client_id = 0;
	// our sockets for the server
	this->listenSocket = INVALID_SOCKET;

	int iResult;	// for error checking return values

	struct addrinfo *result = NULL; // address info for the server to listen to
	struct addrinfo hints;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return false;
	}

	// set address information
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;    // TCP
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);	//NULL = Dont need addres since it will be on local machine

	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return false;
	}

	// Create a SOCKET for connecting to server
	this->listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (this->listenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	// Set the mode of the socket to be nonblocking
	u_long iMode = 1;
	iResult = ioctlsocket(this->listenSocket, FIONBIO, &iMode);

	if (iResult == SOCKET_ERROR) {
		printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
		closesocket(this->listenSocket);
		WSACleanup();
		return false;
	}

	// Setup the TCP listening socket
	iResult = bind(this->listenSocket, result->ai_addr, (int)result->ai_addrlen);

	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(this->listenSocket);
		WSACleanup();
		return false;
	}

	// no longer need address information
	freeaddrinfo(result);

	// start listening for new clients attempting to connect
	iResult = listen(this->listenSocket, SOMAXCONN);

	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(this->listenSocket);
		WSACleanup();
		return false;
	}
	printf("Initlized\n");
	return true;
}

bool ClientNetwork::Shutdown()
{
	int i = 0;
	i = this->connectedClients.size();
	this->connectedClients.clear();
	printf("%d Clients has been removed on server shutdown\n", i);

	return true;
}

void ClientNetwork::Update()
{
	// Get any new clients
	this->AcceptNewClient(this->client_id);

	//Read messages
	ReadMessagesFromClients();

}

void ClientNetwork::Join(char* ip)
{
	addrinfo *result = NULL;
	addrinfo *ptr = NULL;
	addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;  //TCP

	int iResult = getaddrinfo(ip, DEFAULT_PORT, &hints, &result);

	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		exit(1);
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		this->connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (this->connectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			exit(1);
		}

		// Connect to server.
		iResult = connect(this->connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

		if (iResult == SOCKET_ERROR)
		{
			closesocket(this->connectSocket);
			this->connectSocket = INVALID_SOCKET;
			printf("The server is down... did not connect");
		}
	}

	// no longer need address info for server
	freeaddrinfo(result);

	// if connection failed
	if (this->connectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		WSACleanup();
		exit(1);
	}

	// Set the mode of the socket to be nonblocking
	u_long iMode = 1;

	iResult = ioctlsocket(this->connectSocket, FIONBIO, &iMode);
	if (iResult == SOCKET_ERROR)
	{
		printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
		closesocket(this->connectSocket);
		WSACleanup();
		exit(1);
	}

	//Send CONNECTION_REQUEST package
	const unsigned int packet_size = sizeof(FlagPacket);
	char packet_data[packet_size];

	FlagPacket packet;
	packet.packet_type = CONNECTION_REQUEST;

	packet.serialize(packet_data);
	NetworkService::sendMessage(this->connectSocket, packet_data, packet_size);
	printf("Sent CONNECTION_REQUEST to host\n");

	this->connectedClients.insert(pair<unsigned int, SOCKET>(this->client_id, this->connectSocket));
	printf("client %d has been connected to the this client\n", this->client_id);
	this->client_id++;

}

void ClientNetwork::SendFlagPackage(PacketTypes type)
{
	const unsigned int packet_size = sizeof(FlagPacket);
	char packet_data[packet_size];

	//Fill the packet
	FlagPacket packet;
	packet.packet_type = type;

	packet.serialize(packet_data);
	this->SendToAll(packet_data, packet_size);
}

void ClientNetwork::SendEntityUpdatePackage(unsigned int entityID, DirectX::XMFLOAT3 newPos, DirectX::XMFLOAT3 newVelocity, DirectX::XMFLOAT3 newRotation, DirectX::XMFLOAT3 newRotationVelocity)
{
	const unsigned int packet_size = sizeof(EntityPacket);
	char packet_data[packet_size];

	EntityPacket packet;
	packet.packet_type = UPDATE_ENTITY;
	packet.entityID = entityID;
	packet.newPos = newPos;
	packet.newRotation = newRotation;
	packet.newRotationVelocity = newRotationVelocity;
	packet.newVelocity = newVelocity;

	packet.serialize(packet_data);
	this->SendToAll(packet_data, packet_size);
}

void ClientNetwork::SendAnimationPacket(unsigned int entityID)
{
	const unsigned int packet_size = sizeof(AnimationPacket);
	char packet_data[packet_size];

	AnimationPacket packet;
	packet.packet_type = UPDATE_ANIMATION;
	packet.entityID = entityID;

	packet.serialize(packet_data);
	this->SendToAll(packet_data, packet_size);
}

void ClientNetwork::SendStatePacket(unsigned int entityID, bool newState)
{
	const unsigned int packet_size = sizeof(StatePacket);
	char packet_data[packet_size];

	StatePacket packet;
	packet.packet_type = UPDATE_STATE;
	packet.entityID = entityID;
	packet.newState = newState;

	packet.serialize(packet_data);
	this->SendToAll(packet_data, packet_size);
}

bool ClientNetwork::AcceptNewClient(unsigned int & id)
{
	SOCKET otherClientSocket;
	// if client waiting, accept the connection and save the socket
	otherClientSocket = accept(this->listenSocket, NULL, NULL);

	if (otherClientSocket != INVALID_SOCKET)
	{
		//disable nagle on the client's socket
		char value = 1;
		setsockopt(otherClientSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));	//TCP Options
																						
		// insert new client into session id table 
		this->connectedClients.insert(pair<unsigned int, SOCKET>(id, otherClientSocket));
		printf("client %d has been connected to the server\n", this->client_id);
		this->client_id++;

		return true;
	}

	return false;
}

int ClientNetwork::ReceiveData(unsigned int client_id, char * recvbuf)
{
	if (this->connectedClients.find(client_id) != this->connectedClients.end())
	{
		SOCKET currentSocket = this->connectedClients[client_id];
		int iResult = NetworkService::receiveMessage(currentSocket, recvbuf, MAX_PACKET_SIZE);
		if (iResult == 0)
		{
			printf("Connection closed\n");
			closesocket(currentSocket);
		}
		return iResult;
	}
	return 0;
}

void ClientNetwork::ReadMessagesFromClients()
{
	char network_data[MAX_PACKET_SIZE];
	bool t = true;
	unsigned int header = -1;
	EntityPacket eP;
	AnimationPacket aP;
	StatePacket sP;

	//printf("PT: %d, fP: %d, eP, %d aP: %d, sP: %d\n",sizeof(PacketTypes), sizeof(FlagPacket), sizeof(EntityPacket), sizeof(AnimationPacket), sizeof(StatePacket));
	// go through all clients
	std::map<unsigned int, SOCKET>::iterator iter;

	for (iter = this->connectedClients.begin(); iter != this->connectedClients.end();)
	{
		// get data for that client
		int data_length = this->ReceiveData(iter->first, network_data);

		if (data_length <= 0)
		{
			//no data recieved exit function
			continue;
		}

		//Read the header (the first 4 bytes)
		memcpy(&header, &(network_data[4]), sizeof(PacketTypes));

		switch (header)
		{

		case CONNECTION_REQUEST:

			printf("Host received connection packet from client\n");

			this->SendFlagPackage(CONNECTION_ACCEPTED);

			iter++;
			break;

		case CONNECTION_ACCEPTED:

			printf("Client received CONNECTION_ACCEPTED packet from Host\n");

			//this->SendFlagPackage(ACTION_EVENT);	//To spam the other client
			//this->SendEntityUpdatePackage(this->testID, this->testFloat3, this->testFloat3, this->testFloat3, this->testFloat3);
			this->SendAnimationPacket(this->testID);
			this->testID++;
			this->SendStatePacket(this->testID, true);
			this->testID++;

			iter++;
			break;

		case DISCONNECT_REQUEST:
			
			printf("Host recived: DISCONNECT_REQUEST from Client %d \n", iter->first);
			
			this->SendFlagPackage(DISCONNECT_ACCEPTED);
			this->RemoveClient(iter->first);	//Send the clientID
			
			iter = this->connectedClients.end();
			break;

		case DISCONNECT_ACCEPTED:
			
			printf("Client recived: DISCONNECT_ACCEPTED\n");
			this->RemoveClient(iter->first);	//Send the clientID
			iter = this->connectedClients.end();
			break;

		case UPDATE_ENTITY:

			printf("Recived ENTITY_UPDATE packet\n");

			eP.deserialize(network_data);
			
			printf("EntityID: %d\n", eP.entityID);

			iter++;
			break;

		case UPDATE_ANIMATION:

			printf("Recived ANIMATION_UPDATE packet\n");

			
			aP.deserialize(network_data);

			printf("EntityID: %d\n", aP.entityID);

			iter++;
			break;

		case UPDATE_STATE:

			printf("Recived STATE_UPDATE packet\n");
			
			sP.deserialize(network_data);
			printf("StateID: %d\n", sP.entityID);

			iter++;
			break;
		
		default:
			printf("Unkown packet type %d\n", header);
		}
	}

}

void ClientNetwork::SendToAll(char * packets, int totalSize)
{
	SOCKET currentSocket;
	std::map<unsigned int, SOCKET>::iterator iter;
	int iSendResult;

	for (iter = this->connectedClients.begin(); iter != this->connectedClients.end(); iter++)
	{
		currentSocket = iter->second;	//Secound is the mapped socket
		iSendResult = NetworkService::sendMessage(currentSocket, packets, totalSize);

		if (iSendResult == SOCKET_ERROR)
		{
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(currentSocket);
		}
	}
}

bool ClientNetwork::RemoveClient(unsigned int clientID)
{
			//Remove the Client from the list
	if (this->connectedClients.erase(clientID) != 0)	//Erase returns nummber of objects deleted
	{	
		printf("Client %d removed\n", clientID);
	}
	else
	{
		printf("No Client removed\n");
	}

	return true;
}
