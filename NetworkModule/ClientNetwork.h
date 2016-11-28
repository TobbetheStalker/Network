#ifndef NETWORKMODULE_NETWORK_CLIENTNETWORK_H
#define NETWORKMODULE_NETWORK_CLIENTNETWORK_H

#include "NetworkService.h"
#include <ws2tcpip.h>
#include <map>
#include "NetworkData.h"
#include <vector>

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
	std::vector<Packet> packet_Buffer;
	bool isLocked;
	unsigned int client_id;

	int ReceiveData(unsigned int client_id, char * recvbuf);
	bool AcceptNewClient(unsigned int & id);	//accept new connections
	void SendToAll(char * packets, int totalSize);
	bool RemoveClient(unsigned int clientID);
	void ReadMessagesFromClients();

	//Test
	int testID = 0;
	DirectX::XMFLOAT3 testFloat3 = DirectX::XMFLOAT3(1.f,2.f,3.f);

public:
	ClientNetwork();
	~ClientNetwork();

	bool Initialize();
	bool Shutdown();    

	void Update();
	void Join(char* ip);

	//Public package functions (send to all other clients e.g the other player)
	void SendFlagPacket(PacketTypes type);
	void SendEntityUpdatePacket(unsigned int entityID, DirectX::XMFLOAT3 newPos, DirectX::XMFLOAT3 newVelocity, DirectX::XMFLOAT3 newRotation, DirectX::XMFLOAT3 newRotationVelocity);
	void SendAnimationPacket(unsigned int entityID);
	void SendStatePacket(unsigned int entityID, bool newState);

	bool isPacketBufferLocked();

};


#endif
