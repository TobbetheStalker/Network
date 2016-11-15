#ifndef NETWORKMODULE_NETWORK_NETWORKDATA_H
#define NETWORKMODULE_NETWORK_NETWORKDATA_H

#define MAX_PACKET_SIZE 1000000

enum PacketTypes {

	INIT_CONNECTION = 0,
	ACTION_EVENT = 1,
	DISCONNECT_REQUEST = 2,
	DISCONNECT_ACCEPTED = 3,

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

#endif