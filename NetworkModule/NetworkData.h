#ifndef NETWORKMODULE_NETWORK_NETWORKDATA_H
#define NETWORKMODULE_NETWORK_NETWORKDATA_H

#define MAX_PACKET_SIZE 1000000

#include <DirectXMath.h>

enum PacketTypes {

	CONNECTION_REQUEST = 0,
	CONNECTION_ACCEPTED = 1,
	ACTION_EVENT = 2,
	DISCONNECT_REQUEST = 3,
	DISCONNECT_ACCEPTED = 4,
	ENTITY_UPDATE = 5,

};

struct Packet 
{
	unsigned int packet_type;	//PacketType enums

	void serialize(char * data) 
	{			//Turn the PacketType into bytes
		memcpy(data, this, sizeof(Packet));
	}

	void deserialize(char * data)
	{			//Turn bytes into PacketType
		memcpy(this, data, sizeof(Packet));
	}
};

struct EntityPacket
{
	int EntityID;
	DirectX::XMFLOAT3 newPos;
	DirectX::XMFLOAT3 newVelocity;
	DirectX::XMFLOAT3 newRotation;
	DirectX::XMFLOAT3 newRotationVelocity;

	void serialize(char * data)
	{			//Turn the PacketType into bytes
		memcpy(data, this, sizeof(EntityPacket));
	}

	void deserialize(char * data)
	{			//Turn bytes into PacketType
		memcpy(this, data, sizeof(EntityPacket));
	}

};

#endif