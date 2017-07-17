#pragma once

class AxChunkId
{
public:
	static const unsigned int SerializationId_Main			= 0x53495841; // AXIS
    static const unsigned int SerializationId_Thumbnail		= 0x60000000;


	static const unsigned int ImageSerializationId_Main		= 0x4d495841; // AXIM
	static const unsigned int ImageSerializationId_Data		= 0x00004d49;
	static const unsigned int ImageSerializationId_Size		= 0x00014d49;
	static const unsigned int ImageSerializationId_Format	= 0x00024d49;
};