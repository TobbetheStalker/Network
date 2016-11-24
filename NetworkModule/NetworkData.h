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

struct FlagPacket 
{
	unsigned int packet_type;	//PacketType enums

	void serialize(char * data) 
	{			//Turn the PacketType into bytes
		memcpy(data, this, sizeof(FlagPacket));
	}

	void deserialize(char * data)
	{			//Turn bytes into PacketType
		memcpy(this, data, sizeof(FlagPacket));
	}
};

struct EntityPacket
{											// nummber of bytes:
	unsigned int EntityID;					// 4
	DirectX::XMFLOAT3 newPos;				// 12
	DirectX::XMFLOAT3 newVelocity;			// 12
	DirectX::XMFLOAT3 newRotation;			// 12
	DirectX::XMFLOAT3 newRotationVelocity;	// 12

	void serialize(char * data)
	{
		memcpy(data, this, sizeof(EntityPacket));
	}

	void deserialize(char * data)
	{
		memcpy(this, data, sizeof(EntityPacket));
	}

};

struct AnimationPacket
{

	void serialize(char * data)
	{
		memcpy(data, this, sizeof(AnimationPacket));
	}

	void deserialize(char * data)
	{
		memcpy(this, data, sizeof(AnimationPacket));
	}
};

struct StatePacket
{
	unsigned int ID;
	bool newState;

	void serialize(char * data)
	{
		memcpy(data, this, sizeof(StatePacket));
	}

	void deserialize(char * data)
	{
		memcpy(this, data, sizeof(StatePacket));
	}
};
#endif