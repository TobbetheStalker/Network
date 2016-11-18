#ifndef NETWORKMODULE_NETWORK_CLIENTNETWORK_H
#define NETWORKMODULE_NETWORK_CLIENTNETWORK_H

#include "NetworkService.h"
#include <ws2tcpip.h>
#include <map>
#include "NetworkData.h"

using namespace std;
#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "6881"

class ClientNetwork
{

private:
	SOCKET listenSocket;	
	SOCKET connectSocket; // Socket to listen for new connections	
	std::map<unsigned int, SOCKET> connectedClients;	// table to keep track of each client's socket
	unsigned int client_id;

	int ReceiveData(unsigned int client_id, char * recvbuf);
	bool AcceptNewClient(unsigned int & id);	//accept new connections
	void SendToAll(char * packets, int totalSize);
	bool RemoveClient(unsigned int clientID);
	void ReadMessagesFromClients();

	//Create packages helper functions
	void Packet_Flag(PacketTypes type, Packet& packet);	//Send a packet with only a header and and empty data
	//EntityPacket Packet_EntityUpdate(int entityID, DirectX::XMFLOAT3 newPos, DirectX::XMFLOAT3 newVelocity, DirectX::XMFLOAT3 newRotation, DirectX::XMFLOAT3 newRotationVelocity);

public:
	ClientNetwork();
	~ClientNetwork();

	bool Initialize();
	bool Shutdown();

	void Update();
	void Join(char* ip);

	//Public package functions
	void SendFlagPackage(PacketTypes type);
	//void SendEntityUpdatePackage(int entityID, DirectX::XMFLOAT3 newPos, DirectX::XMFLOAT3 newVelocity, DirectX::XMFLOAT3 newRotation, DirectX::XMFLOAT3 newRotationVelocity);
	



};


#endif
