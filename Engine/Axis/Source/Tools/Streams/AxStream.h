#pragma once

#include "..\..\AxGlobals.h"

#include <exception>

#include "..\AxString.h"

class AXDLLCLASS StreamException
	: public std::exception
{
public:
	char *message;

	StreamException(const char *message);
	~StreamException(void);

	virtual const char *what() const throw();
};

enum StreamSeekMode
{
	StreamSeekMode_FromBeginning,
	StreamSeekMode_Relative,
	StreamSeekMode_FromEnd
};


class AXDLLCLASS AxStream
{
public:
	AxStream(void);
	virtual ~AxStream(void);

	long long position, length;

	virtual long long ReadData(void *data, long long size) = 0;
	virtual long long WriteData(const void *data, long long size) = 0;

	virtual bool Seek(long long offset, StreamSeekMode seekType = StreamSeekMode_FromBeginning);
	
	virtual long long Peek(void *data, long long size);

	//virtual long long ReadStreamData(AxStream &stream, long long size = -1, int maxBufferSize = 65536) { stream.WriteStreamData(this, size, maxBufferSize); };
	virtual long long WriteStreamData(AxStream &source, long long size = -1, int maxBufferSize = 65536);

	virtual bool ReadBool();
	virtual void WriteBool(bool value);

	virtual char ReadInt8();
	virtual void WriteInt8(char value);
	virtual short ReadInt16();
	virtual void WriteInt16(short value);
	virtual int ReadInt32();
	virtual void WriteInt32(int value);
	virtual long long ReadInt64();
	virtual void WriteInt64(long long value);
	virtual int Read7BitEncodedInt32();
	virtual void Write7BitEncodedInt32(int value);
	
	virtual unsigned char ReadUInt8();
	virtual void WriteUInt8(unsigned char value);
	virtual unsigned short ReadUInt16();
	virtual void WriteUInt16(unsigned short value);
	virtual unsigned int ReadUInt32();
	virtual void WriteUInt32(unsigned int value);
	virtual unsigned long long ReadUInt64();
	virtual void WriteUInt64(unsigned long long value);

	virtual float ReadFloat32();
	virtual void WriteFloat32(float value);
	virtual double ReadFloat64();
	virtual void WriteFloat64(double value);

	virtual AxString ReadString();
	virtual void WriteString(AxString &value);

	virtual AxString ReadText();
	virtual void WriteText(AxString text);
	virtual AxString ReadUText();
	virtual void WriteUText(AxString text);
	virtual AxString ReadTextLine();
	virtual void WriteTextLine(AxString text);
	virtual AxString ReadUTextLine();
	virtual void WriteUTextLine(AxString text);
	virtual AxString ReadTextFull();
	virtual AxString ReadUTextFull();
};

