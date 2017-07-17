//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxOpenALSoundEmitter.h"

#include "AxOpenALAudioDevice.h"

AxOpenALSoundEmitter::AxOpenALSoundEmitter(void)
{
	alGenSources(1, &this->handle);

	this->soundBuffer = 0;

	this->isLooping = false;
	this->isAmbient = false;
	this->isPlaying = false;

	//alDopplerFactor();
	//alSpeedOfSound(80.0f);
	//alSourcef(this->handle, AL_ROLLOFF_FACTOR, 100.0f);
	//alSourcei(this->handle, AL_REFERENCE_DISTANCE, 1.0f);
	//alSourcei(this->handle, AL_MAX_DISTANCE, 1000.0f);

	//AxVector3 p(0), v(0), o[2] = { AxVector3(0.0f, 0.0f, 1.0f), AxVector3(0.0f, 1.0f, 0.0f) } ;
	//alListenerfv(AL_POSITION, (float*)&v);
	//alListenerfv(AL_VELOCITY, (float*)&v);
	//alListenerfv(AL_ORIENTATION, (float*)&o); //d holds "at" vector3f and "up" vector3f
}


AxOpenALSoundEmitter::~AxOpenALSoundEmitter(void)
{
	this->Stop();
	
	this->context->RemoveStreamingSoundEmitter(this);

	alDeleteSources(1, &this->handle);
}

void AxOpenALSoundEmitter::Play()
{
	alSourcePlay(this->handle); 
}

void AxOpenALSoundEmitter::Stop()
{
	alSourceStop(this->handle); 
}

bool AxOpenALSoundEmitter::RefillStreamingBuffers()
{
	if (!this->soundBuffer->isStreaming)
		return false;

    ALint processedBuffersCount;
	alGetSourcei(this->handle, AL_BUFFERS_PROCESSED, &processedBuffersCount);

	if (processedBuffersCount == 0)
		return false;

    while (processedBuffersCount > 0)
    {
        ALuint alBufferID;

		alSourceUnqueueBuffers(this->handle, 1, &alBufferID);

		unsigned int readBytes;
		if (this->isLooping)
			readBytes = this->soundBuffer->mediaSource->ReadDataLooping(this->soundBuffer->mediaSource->dataSize);
		else
			readBytes = this->soundBuffer->mediaSource->ReadData(this->soundBuffer->mediaSource->dataSize);
		
		alBufferData(alBufferID, this->soundBuffer->alFormat, this->soundBuffer->mediaSource->data, readBytes, this->soundBuffer->mediaSource->pcmFormat.frequency);

        alSourceQueueBuffers(this->handle, 1, &alBufferID);

		processedBuffersCount--;
    }

	return true;
}

int AxOpenALSoundEmitter::GetParameterAddress(AxString parameterName)
{
	if (parameterName.Equals("Sound"))
		return AxOpenALSoundEmitter::parameterAddress_Sound;
	else if (parameterName.Equals("WorldMatrix"))
		return AxOpenALSoundEmitter::parameterAddress_WorldMatrix;
	else if (parameterName.Equals("Velocity"))
		return AxOpenALSoundEmitter::parameterAddress_Velocity;
	else if (parameterName.Equals("Loop"))
		return AxOpenALSoundEmitter::parameterAddress_Loop;
	else if (parameterName.Equals("Rate"))
		return AxOpenALSoundEmitter::parameterAddress_Rate;
	else if (parameterName.Equals("Volume"))
		return AxOpenALSoundEmitter::parameterAddress_Volume;
	else if (parameterName.Equals("Ambient"))
		return AxOpenALSoundEmitter::parameterAddress_Ambient;
	else if (parameterName.Equals("Play"))
		return AxOpenALSoundEmitter::parameterAddress_Play;

	return AxOpenALSoundEmitter::parameterAddress_Invalid;
}

void AxOpenALSoundEmitter::SetParameter(int address, int index, void *value)
{
	if (address == AxOpenALSoundEmitter::parameterAddress_Invalid)
		return;

	switch (address)
	{
		case AxOpenALSoundEmitter::parameterAddress_Sound:
		{
			AxOpenALSound *buffer = value != 0 ? (AxOpenALSound*)value : 0;

			if (buffer == this->soundBuffer)
				break;

			if (this->soundBuffer != 0)
			{
				if (this->soundBuffer->isStreaming)
					this->context->RemoveStreamingSoundEmitter(this);

				alSourceStop(this->handle); 

				ALuint unqueuedBuffer;
				for (int i = 0; i < this->soundBuffer->buffersCount; i++)
					alSourceUnqueueBuffers(this->handle, 1, &unqueuedBuffer);
			}

			this->soundBuffer = buffer;

			alSourceQueueBuffers(this->handle, this->soundBuffer->buffersCount, this->soundBuffer->bufferIds);

			if (this->soundBuffer->isStreaming)
			{
				alSourcei(this->handle, AL_LOOPING, false);

				this->context->AddStreamingSoundEmitter(this);
			}
			else
			{
				alSourcei(this->handle, AL_LOOPING, this->isLooping);
			}

			if (this->isPlaying)
				alSourcePlay(this->handle); 

			break;
		}

		case AxOpenALSoundEmitter::parameterAddress_WorldMatrix:
		{
			if (!this->isAmbient)
				if (value != 0)
				{
					alSourcefv(this->handle, AL_POSITION, (ALfloat*)&((AxMatrix*)value)->_41);
					alSourcefv(this->handle, AL_DIRECTION, (ALfloat*)&((AxMatrix*)value)->_31);
				}
			break;
		}

		case AxOpenALSoundEmitter::parameterAddress_Velocity:
		{
			alSourcefv(this->handle, AL_VELOCITY, (ALfloat*)(AxVector3*)value);
			break;
		}

		case AxOpenALSoundEmitter::parameterAddress_Loop:
		{
			bool looping = *(bool*)value;
			if (looping == this->isLooping)
				break;

			this->isLooping = looping;

			if (this->soundBuffer == 0)
				break;

			if (!this->soundBuffer->isStreaming)
				alSourcei(this->handle, AL_LOOPING, this->isLooping);

			break;
		}

		case AxOpenALSoundEmitter::parameterAddress_Rate:
		{
			alSourcef(this->handle, AL_PITCH, *(float*)value);
			break;
		}

		case AxOpenALSoundEmitter::parameterAddress_Volume:
		{
			alSourcef(this->handle, AL_GAIN, *(float*)value);
			break;
		}

		case AxOpenALSoundEmitter::parameterAddress_Ambient:
		{
			bool ambient = *(bool*)value;
			if (ambient == this->isAmbient)
				break;

			this->isAmbient = ambient;

			if (this->isAmbient)
			{
				alSourcei(this->handle, AL_SOURCE_RELATIVE, AL_TRUE);
				alSource3f(this->handle, AL_POSITION, 0.0f, 0.0f, 0.0f);
			}
			else
			{
				alSourcei(this->handle, AL_SOURCE_RELATIVE, AL_FALSE);
			}

			break;
		}
		
		case AxOpenALSoundEmitter::parameterAddress_Play:
		{
			bool play = *(bool*)value;
			if (play == this->isPlaying)
				break;

			this->isPlaying = play;

			if (this->isPlaying)
				alSourcePlay(this->handle); 
			else
				alSourceStop(this->handle); 

			break;
		}
	}
}
