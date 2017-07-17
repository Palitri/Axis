#pragma once

#include "..\..\AxGlobals.h"

#include "AxParameterType.h"

#include "..\Primitive\AxVector4.h"

class AXDLLCLASS AxParameter
{
private:
	static AxParameter *GetEffectiveParameter(AxParameter *parameter);

	void DisposeValue();

public:
	static bool IsReference(AxParameterType type);

	void *value;
	AxParameterType type;

	AxParameter(void);
	AxParameter(int value);
	AxParameter(float value);
	AxParameter(bool value);
	AxParameter(const AxVector4 &value);
	AxParameter(void *value, AxParameterType type);
	virtual ~AxParameter(void);

	bool Equals(AxParameter &parameter);
	bool IsReference();

	// Gets the effective value of the parameter.
	AxParameter *GetEffectiveParameter();
	// Recalls the effective value of the parameter and sets it. If the parameter was a reference before calling this method, after that it will hold the referenced value
	void RecallEffectiveParameter();

	// Sets the type of the parameter and reallocates memory if required
	void SetType(AxParameterType type);

	// Sets a reference value of the given type. If necessary, previous memory is deallocated and no new memory is allocated. Only a reference is saved.
	//void SetReference(AxParameterType type, void *value);
	// Sets a value of the given type. If necessary, memory is reallocated. Depending on the type, a copy of the referenced value or a reference is saved.
	void SetValue(void *value, AxParameterType type);
	// Returns the effective value of the parameter. Traces to a non-mechanism value
	void *GetEffectiveValue();

	// Sets an integer value
	void SetInt(int value);
	// Gets the effective value as an integer.
	int GetInt();

	// Sets a float value
	void SetFloat(float value);
	// Gets the effective value as a float.
	float GetFloat();

	// Sets an bool value
	void SetBool(bool value);
	// Gets the effective value as a bool.
	bool GetBool();	

	// Sets a color value
	void SetColor(const AxVector4 &value);
	// Gets the effective value as a color.
	AxVector4 GetColor();
};

