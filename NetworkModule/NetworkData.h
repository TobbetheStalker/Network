#ifndef NETWORKMODULE_NETWORK_NETWORKDATA_H
#define NETWORKMODULE_NETWORK_NETWORKDATA_H

#define MAX_PACKET_SIZE 1000000

#include <DirectXMath.h>

enum PacketTypes {

	CONNECTION_REQUEST = 0,
	CONNECTION_ACCEPTED = 1,
	DISCONNECT_REQUEST = 2,
	DISCONNECT_ACCEPTED = 3,
	UPDATE_ENTITY = 4,
	UPDATE_ANIMATION = 5,
	UPDATE_STATE = 6,

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