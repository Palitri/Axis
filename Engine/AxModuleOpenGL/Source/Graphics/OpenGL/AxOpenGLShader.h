#pragma once

#include "..\..\AxisEngine.h"

#include "AxOpenGLGraphicsDevice.h"

#include "DynamicShading\AxDynamicShaderWriter.h"

class AxOpenGLGraphicsDevice;

struct AxOpenGLMaterialShadingOperation
{
    AxShadingOperation shadingOp;
    AxColorOperation colorOp;
};

class AxOpenGLShader :
	public AxDeviceShader
{
private:
	static bool GetShaderCompileStatus(GLuint shader, AxString &statusMessage);
	static bool GetShaderProgramCompileStatus(GLuint shaderProgram, AxString &statusMessage);


	AxDynamicShaderWriter *shaderWriter;

	bool SetFakeParameter(int address, int index, void *value);
	bool CompileShader(AxString &vertexShaderCode, AxString &pixelShaderCode);
	void Release();
	int IndexOf(AxShadingOperation shadingOp);

	void AxMatrixToGLUniform(AxMatrix &axMatrix, float *glUniform, int offset);
	void AxVector3ToGLUniform(AxVector3 &axVector3, float *glUniform, int offset);
	void AxVector4ToGLUniform(AxVector4 &axVector4, float *glUniform, int offset);

public:
	static const int maxBones = 48;
	static const int maxLights = 4;

	// There are no actual view and projection matrices in the shader, but a viewProjectionMatrix, for optimization
	static const int viewMatrixAddress = -2;
	static const int projectionMatrixAddress = -3;
	
	// There actually is a world matrix, but WebGL uniform location objects can't be compared correctly, so in case there is skeletal animation, this fake value is used in supplemental to the real one, for comparison purposes
	static const int worldMatrixFakeAddress = -4;
	
	// Because these are array uniforms, they need special management. See AxWebGLShader.worldMatrixFakeAddress for clarification
	// Actually, there is a lightsPosIntensity array vector in the shader, each element a 4-vector with xyz as position and w as intensity
	static const int lightsPosFakeAddress = -5;
	static const int lightsIntensityFakeAddress = -6;
	static const int lightsColorFakeAddress = -7;

	AxOpenGLGraphicsDevice *context;

    AxString compilationMessage;
    
    
    GLuint vertexPositionAttribute, vertexTexCoordsAttribute, vertexNormalAttribute, vertexBoneIndicesAttribute, vertexBoneWeightsAttribute, vertexTangentsAttribute, vertexBitangentsAttribute;

	GLuint glVertexShader, glPixelShader;
	GLuint glShaderProgram;
    
    AxMatrix viewMatrix, projectionMatrix;
    
    AxList<AxOpenGLMaterialShadingOperation> shadingOps;
    
    int worldMatrixAddress, viewProjectionMatrixAddress, lightsPosIntensityAddress, lightsColorAddress;
    
    int setTextureIndex;
    
    GLfloat *bones;
    bool isSkeletalAnimated;
    
    GLfloat *lightsPosIntensity;
    GLfloat *lightsColor;
    
    // Used for general purposes, for conversion between AxMatrix and GL's matrix format
    GLfloat *glMatrix;



	AxMatrix modelTransform, viewTransform, projectionTransform,
    		 modelViewTransform;

	bool isAnimated;

	AxOpenGLShader(void);
	~AxOpenGLShader(void);
	
	virtual int GetOperationCount();
	virtual void AddOperation(AxShadingOperation shadingOp, AxColorOperation colorOp);
	virtual void GetOperation(int index, AxShadingOperation &shadingOp, AxColorOperation &colorOp);
	virtual void SetOperation(int index, AxShadingOperation shadingOp, AxColorOperation colorOp);
	virtual void ClearOperations();

	virtual int GetParameterAddress(AxString parameterName);
	virtual void SetParameter(int address, int index, void *value);

	virtual bool Update(AxString &compilationMessage);
};

