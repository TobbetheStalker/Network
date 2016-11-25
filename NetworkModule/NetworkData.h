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
	ANIMATION_UPDATE = 6,
	STATE_UPDATE = 7,

};

struct FlagPacket				// 4
{
	PacketTypes packet_type;	// 4

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
	PacketTypes packet_type;
	unsigned int entityID;					// 4
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

struct AnimationPacket	//4
{
	PacketTypes packet_type;
	unsigned int entityID;	//4

	void serialize(char * data)
	{
		memcpy(data, this, sizeof(AnimationPacket));
	}

	void deserialize(char * data)
	{
		memcpy(this, data, sizeof(AnimationPacket));
	}
};

struct StatePacket			//8
{
	PacketTypes packet_type;
	unsigned int entityID;	//4
	bool newState;			//4

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