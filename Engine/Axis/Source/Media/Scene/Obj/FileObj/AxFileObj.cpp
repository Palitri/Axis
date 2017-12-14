//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxFileObj.h"

#include "..\..\..\..\Tools\AxStrings.h"

AxFileObj::AxFileObj(void)
{
	this->data = 0;
	this->dataSize = 0;
}


AxFileObj::~AxFileObj(void)
{
	for (int i = 0; i < this->meshes.count; i++)
		delete this->meshes[i];

	for (int i = 0; i < this->materials.count; i++)
		delete this->materials[i];

	if (this->data != 0)
		delete[] this->data;
}

bool AxFileObj::ParseLine()
{
	if (this->dataPos >= this->dataSize)
		return false;

	int start = dataPos;
	this->argumentsCount = 0;
	bool hasEnded = false;
	bool isDataEnd;
	do
	{
		char c = this->data[this->dataPos];
		bool isEmpty = (c == ' ') || (c == '\t');
		bool isLineEnd = (c == '\r') || (c == '\n');

		isDataEnd = (this->dataPos >= this->dataSize);
		hasEnded |= isLineEnd;

		if (isEmpty || isLineEnd || isDataEnd)
		{
			int length = this->dataPos - start;
			if (length > 0)
			{
				AxFileObjArgument *argument;

				if (this->arguments.count > this->argumentsCount)
				{
					argument = &this->arguments[this->argumentsCount];
				}
				else
				{
					argument = &this->arguments.Add(AxFileObjArgument());
				}

				argument->start = start;
				argument->end = dataPos - 1;

				this->argumentsCount++;
			}
			
			start = dataPos + 1;
		}
		else
		{
			// Break on the first non-empty, non-line terminating character on the next line
			if (hasEnded)
				break;
		}

		this->dataPos++;
	}
	while (!isDataEnd);

	return true;
}

bool AxFileObj::ArgEquals(int argIndex, const char *str)
{
	if (argIndex == -1)
		return false;
	
	return this->DataEquals(this->arguments[argIndex].start, this->arguments[argIndex].end, str);
}

bool AxFileObj::ArgEquals(int arg1Index, AxFileObjArgument *arg2)
{
	if ((arg1Index == -1) || (arg2 == 0))
		return false;
	
	AxFileObjArgument *arg1 = &this->arguments[arg1Index];
	int length1 = arg1->end - arg1->start + 1;
	int length2 = arg2->end - arg2->start + 1;
	
	if (length1 != length2)
		return false;

	for (int i = 0; i < length1; i++)
		if (this->data[arg1->start + i] != this->data[arg2->start + i])
			return false;

	return true;
}

bool AxFileObj::DataEquals(int start, int end, const char *str)
{
	int strIndex = 0;

	for (int i = start; i <= end; i++)
		if (this->data[i] != str[strIndex++])
			return false;

	return str[strIndex] == 0;
}

bool AxFileObj::DataToInt(int pos, int &value)
{
	char *start = &this->data[pos];
	char *end;
	value = AxString::ToInt(start, &end);
	return end != start;
}

bool AxFileObj::DataToFloat(int pos, float &value)
{
	char *start = &this->data[pos];
	char *end;
	value = (float)AxString::ToDouble(start, &end);
	return end != start;
}

int AxFileObj::DataCharIndex(int start, int end, const char character)
{
	while (start <= end)
		if (this->data[start] == character)
			return start;
		else
			start++;

	return -1;
}

AxString AxFileObj::DataCreateString(int startArgIndex, int endArgIndex)
{
	return AxString(&this->data[arguments[startArgIndex].start], this->arguments[endArgIndex].end - this->arguments[startArgIndex].start + 1);
}

bool  AxFileObj::ReadTextureMap(AxFileObjTextureMap * texture)
{
	int argumentIndex = 1;
	while (argumentIndex < this->argumentsCount)
	{
		if (this->ArgEquals(argumentIndex, "-o"))
		{
			if (this->argumentsCount < argumentIndex + 4)
				return false;

			if (!this->DataToFloat(this->arguments[argumentIndex + 1].start, texture->offset.x))
				return false;
			if (!this->DataToFloat(this->arguments[argumentIndex + 2].start, texture->offset.y))
				return false;
			if (!this->DataToFloat(this->arguments[argumentIndex + 3].start, texture->offset.z))
				return false;

			argumentIndex += 4;
		}
		
		else if (this->ArgEquals(argumentIndex, "-s"))
		{
			if (this->argumentsCount < argumentIndex + 4)
				return false;

			if (!this->DataToFloat(this->arguments[argumentIndex + 1].start, texture->scale.x))
				return false;
			if (!this->DataToFloat(this->arguments[argumentIndex + 2].start, texture->scale.y))
				return false;
			if (!this->DataToFloat(this->arguments[argumentIndex + 3].start, texture->scale.z))
				return false;

			argumentIndex += 4;
		}
		
		else if (this->ArgEquals(argumentIndex, "-t"))
		{
			if (this->argumentsCount < argumentIndex + 4)
				return false;

			if (!this->DataToFloat(this->arguments[argumentIndex + 1].start, texture->turbulence.x))
				return false;
			if (!this->DataToFloat(this->arguments[argumentIndex + 2].start, texture->turbulence.y))
				return false;
			if (!this->DataToFloat(this->arguments[argumentIndex + 3].start, texture->turbulence.z))
				return false;

			argumentIndex += 4;
		}
		
		else if (this->ArgEquals(argumentIndex, "-mm"))
		{
			if (this->argumentsCount < argumentIndex + 3)
				return false;

			if (!this->DataToFloat(this->arguments[argumentIndex + 1].start, texture->brightness))
				return false;
			if (!this->DataToFloat(this->arguments[argumentIndex + 2].start, texture->contrastFactor))
				return false;

			argumentIndex += 3;
		}
		
		else if (this->ArgEquals(argumentIndex, "-bm"))
		{
			if (this->argumentsCount < argumentIndex + 2)
				return false;

			if (!this->DataToFloat(this->arguments[argumentIndex + 1].start, texture->bumpFactor))
				return false;

			argumentIndex += 2;
		}
		
		else if (this->ArgEquals(argumentIndex, "-boost"))
		{
			if (this->argumentsCount < argumentIndex + 2)
				return false;

			if (!this->DataToFloat(this->arguments[argumentIndex + 1].start, texture->mipMapSharpness))
				return false;

			argumentIndex += 2;
		}

		else if (this->ArgEquals(argumentIndex, "-type"))
		{
			if (this->argumentsCount < argumentIndex + 2)
				return false;

			if (this->ArgEquals(argumentIndex + 1, "sphere"))
				texture->mapType = AxFileObjTextureMapType_Sphere;
			else if (this->ArgEquals(argumentIndex + 1, "cube_top"))
				texture->mapType = AxFileObjTextureMapType_CubeTop;
			else if (this->ArgEquals(argumentIndex + 1, "cube_bottom"))
				texture->mapType = AxFileObjTextureMapType_CubeBottom;
			else if (this->ArgEquals(argumentIndex + 1, "cube_front"))
				texture->mapType = AxFileObjTextureMapType_CubeFront;
			else if (this->ArgEquals(argumentIndex + 1, "cube_back"))
				texture->mapType = AxFileObjTextureMapType_CubeBack;
			else if (this->ArgEquals(argumentIndex + 1, "cube_left"))
				texture->mapType = AxFileObjTextureMapType_CubeLeft;
			else if (this->ArgEquals(argumentIndex + 1, "cube_right"))
				texture->mapType = AxFileObjTextureMapType_CubeRight;
			else return false;

			argumentIndex += 2;
		}
		
		else if (this->ArgEquals(argumentIndex, "-imfchan"))
		{
			if (this->argumentsCount < argumentIndex + 2)
				return false;

			if (this->ArgEquals(argumentIndex + 1, "r"))
				texture->mapChannel = AxFileObjTextureMapChannel_R;
			else if (this->ArgEquals(argumentIndex + 1, "g"))
				texture->mapChannel = AxFileObjTextureMapChannel_G;
			else if (this->ArgEquals(argumentIndex + 1, "b"))
				texture->mapChannel = AxFileObjTextureMapChannel_B;
			else if (this->ArgEquals(argumentIndex + 1, "m"))
				texture->mapChannel = AxFileObjTextureMapChannel_Matte;
			else if (this->ArgEquals(argumentIndex + 1, "l"))
				texture->mapChannel = AxFileObjTextureMapChannel_Luminance;
			else if (this->ArgEquals(argumentIndex + 1, "z"))
				texture->mapChannel = AxFileObjTextureMapChannel_Depth;
			else return false;

			argumentIndex += 2;
		}
		
		else if (this->ArgEquals(argumentIndex, "-texres"))
		{
			if (this->argumentsCount < argumentIndex + 2)
				return false;

			if (!this->DataToInt(argumentIndex + 1, texture->resolution))
				return false;

			argumentIndex += 2;
		}

		else if (this->ArgEquals(argumentIndex, "-blendu"))
		{
			if (this->argumentsCount < argumentIndex + 2)
				return false;

			if (this->ArgEquals(argumentIndex + 1, "on"))
				texture->blendU = true;
			else if (this->ArgEquals(argumentIndex + 1, "off"))
				texture->blendU = false;
			else
				return false;

			argumentIndex += 2;
		}

		else if (this->ArgEquals(argumentIndex, "-blendv"))
		{
			if (this->argumentsCount < argumentIndex + 2)
				return false;

			if (this->ArgEquals(argumentIndex + 1, "on"))
				texture->blendV = true;
			else if (this->ArgEquals(argumentIndex + 1, "off"))
				texture->blendV = false;
			else
				return false;

			argumentIndex += 2;
		}

		else if (this->ArgEquals(argumentIndex, "-clamp"))
		{
			if (this->argumentsCount < argumentIndex + 2)
				return false;

			if (this->ArgEquals(argumentIndex + 1, "on"))
				texture->clamp = true;
			else if (this->ArgEquals(argumentIndex + 1, "off"))
				texture->clamp = false;
			else
				return false;

			argumentIndex += 2;
		}

		else
		{
			texture->fileName = this->DataCreateString(argumentIndex, this->argumentsCount - 1);

			argumentIndex = this->argumentsCount;
		}
	}

	return true;
}

bool AxFileObj::Read(AxStream *dataStream)
{
	AxFileObjMesh *currentMesh = new AxFileObjMesh();
	AxFileObjMaterial *currentMaterial;
	int currentSmoothingGroups = 0;

	this->dataPos = 0;
	this->dataSize = (int)(dataStream->length - dataStream->position);
	if (this->data != 0)
		delete[] this->data;
	this->data = new char[this->dataSize];
	dataStream->ReadData(this->data, this->dataSize);

	AxFileObjArgument lastHeadArg;
	bool hasLastHeadArg = false;
	bool success = false;

	int vertexIndexOffset = 1;
	int texCoordIndexOffset = 1;
	int normalIndexOffset = 1;

	while (this->ParseLine())
	{
		if (hasLastHeadArg && (!this->ArgEquals(0, &lastHeadArg)))
		{
			if (this->DataEquals(lastHeadArg.start, lastHeadArg.end, "f") && (!this->ArgEquals(0, "s")))
			{
				vertexIndexOffset += currentMesh->vertices.count;
				texCoordIndexOffset += currentMesh->texCoords.count;
				normalIndexOffset += currentMesh->normals.count;

				this->meshes.Add(currentMesh);
				currentMesh = new AxFileObjMesh();

				success = true;
			}
		}

		if (this->ArgEquals(0, "#"))
		{
			// Ignore comments
		}

		else if (this->ArgEquals(0, "mtllib"))
		{
			if (this->argumentsCount < 2)
				return false;
			
			this->materialFiles.Add(this->DataCreateString(1, this->argumentsCount - 1));

			success = true;
		}

		else if (this->ArgEquals(0, "v"))
		{
			if (this->argumentsCount < 4)
				return false;

			AxFileObjVector4 vertex;

			if (!this->DataToFloat(this->arguments[1].start, vertex.x))
				return false;
			if (!this->DataToFloat(this->arguments[2].start, vertex.y))
				return false;
			if (!this->DataToFloat(this->arguments[3].start, vertex.z))
				return false;
			if (this->argumentsCount > 4)
			{
				if (!this->DataToFloat(this->arguments[4].start, vertex.w))
					return false;
			}
			else
				vertex.w = 1.0f;

			currentMesh->vertices.Add(vertex);
		}

		else if (this->ArgEquals(0, "vt"))
		{
			if (this->argumentsCount < 3)
				return false;

			AxFileObjVector3 texCoords;

			if (!this->DataToFloat(this->arguments[1].start, texCoords.x))
				return false;
			if (!this->DataToFloat(this->arguments[2].start, texCoords.y))
				return false;
			if (this->argumentsCount > 3)
			{
				if (!this->DataToFloat(this->arguments[3].start, texCoords.z))
					return false;
			}
			else
				texCoords.z = 0.0f;

			currentMesh->texCoords.Add(texCoords);
		}

		else if (this->ArgEquals(0, "vn"))
		{
			if (this->argumentsCount < 4)
				return false;

			AxFileObjVector3 normal;

			if (!this->DataToFloat(this->arguments[1].start, normal.x))
				return false;
			if (!this->DataToFloat(this->arguments[2].start, normal.y))
				return false;
			if (!this->DataToFloat(this->arguments[3].start, normal.z))
				return false;

			currentMesh->normals.Add(normal);
		}
		
		else if (this->ArgEquals(0, "g") || this->ArgEquals(0, "o"))
		{
			if (this->argumentsCount < 2)
				return false;
			
			currentMesh->name = this->DataCreateString(1, this->argumentsCount - 1);
		}

		else if (this->ArgEquals(0, "usemtl"))
		{
			if (this->argumentsCount < 2)
				return false;
			
			currentMesh->materialName = this->DataCreateString(1, this->argumentsCount - 1);
		}

		else if (this->ArgEquals(0, "s"))
		{
			if (this->ArgEquals(1, "off"))
				currentSmoothingGroups = 0;
			else if (!this->DataToInt(this->arguments[1].start, currentSmoothingGroups))
				return false;
		}

		else if (this->ArgEquals(0, "f"))
		{
			AxFileObjFace *face = new AxFileObjFace();
			for (int argumentIndex = 1; argumentIndex < this->argumentsCount; argumentIndex++)
			{
				AxFileObjFaceVertex faceVertex;

				int start = this->arguments[argumentIndex].start;

				if (!this->DataToInt(start, faceVertex.vertexIndex))
					return false;
				
				start = this->DataCharIndex(start + 1, this->arguments[argumentIndex].end, '/');
				if (!this->DataToInt(start, faceVertex.texCoordIndex))
					faceVertex.texCoordIndex = 0;

				start = this->DataCharIndex(start + 1, this->arguments[argumentIndex].end, '/');
				if (!this->DataToInt(start, faceVertex.normalIndex))
					faceVertex.normalIndex = 0;

				faceVertex.vertexIndex -= vertexIndexOffset;
				faceVertex.texCoordIndex -= texCoordIndexOffset;
				faceVertex.normalIndex -= normalIndexOffset;

				face->vertices.Add(faceVertex);
			}

			face->smoothingGroups = currentSmoothingGroups;
			currentMesh->faces.Add(face);
		}
		

		// Material data (from .mtl files)

		else if (this->ArgEquals(0, "newmtl"))
		{
			if (this->argumentsCount < 2)
				return false;

			currentMaterial = new AxFileObjMaterial();
			currentMaterial->name = this->DataCreateString(1, this->argumentsCount - 1);

			this->materials.Add(currentMaterial);

			success = true;
		}

		else if (this->ArgEquals(0, "Tr"))
		{
			if (this->argumentsCount < 2)
				return false;

			if (!this->DataToFloat(this->arguments[1].start, currentMaterial->transparency))
				return false;
		}

		else if (this->ArgEquals(0, "Ka"))
		{
			if (this->argumentsCount < 4)
				return false;

			if (!this->DataToFloat(this->arguments[1].start, currentMaterial->ambientColor.x))
				return false;
			if (!this->DataToFloat(this->arguments[2].start, currentMaterial->ambientColor.y))
				return false;
			if (!this->DataToFloat(this->arguments[3].start, currentMaterial->ambientColor.z))
				return false;
		}

		else if (this->ArgEquals(0, "Kd"))
		{
			if (this->argumentsCount < 4)
				return false;

			if (!this->DataToFloat(this->arguments[1].start, currentMaterial->diffuseColor.x))
				return false;
			if (!this->DataToFloat(this->arguments[2].start, currentMaterial->diffuseColor.y))
				return false;
			if (!this->DataToFloat(this->arguments[3].start, currentMaterial->diffuseColor.z))
				return false;
		}

		else if (this->ArgEquals(0, "Ks"))
		{
			if (this->argumentsCount < 4)
				return false;

			if (!this->DataToFloat(this->arguments[1].start, currentMaterial->specularColor.x))
				return false;
			if (!this->DataToFloat(this->arguments[2].start, currentMaterial->specularColor.y))
				return false;
			if (!this->DataToFloat(this->arguments[3].start, currentMaterial->specularColor.z))
				return false;
		}

		else if (this->ArgEquals(0, "Ns"))
		{
			if (this->argumentsCount < 2)
				return false;

			if (!this->DataToFloat(this->arguments[1].start, currentMaterial->specularFactor))
				return false;
		}

		else if (this->ArgEquals(0, "map_Ka"))
		{
			if (currentMaterial->ambientMap == 0)
				currentMaterial->ambientMap = new AxFileObjTextureMap();

			if (!this->ReadTextureMap(currentMaterial->ambientMap))
				return false;
		}

		else if (this->ArgEquals(0, "map_Kd"))
		{
			if (currentMaterial->diffuseMap == 0)
				currentMaterial->diffuseMap = new AxFileObjTextureMap();

			if (!this->ReadTextureMap(currentMaterial->diffuseMap))
				return false;
		}

		else if (this->ArgEquals(0, "map_Ks"))
		{
			if (currentMaterial->specularColorMap == 0)
				currentMaterial->specularColorMap = new AxFileObjTextureMap();

			if (!this->ReadTextureMap(currentMaterial->specularColorMap))
				return false;
		}

		else if (this->ArgEquals(0, "map_Ns"))
		{
			if (currentMaterial->specularHighlightMap == 0)
				currentMaterial->specularHighlightMap = new AxFileObjTextureMap();

			if (!this->ReadTextureMap(currentMaterial->specularHighlightMap))
				return false;
		}

		else if (this->ArgEquals(0, "map_d"))
		{
			if (currentMaterial->alphaMap == 0)
				currentMaterial->alphaMap = new AxFileObjTextureMap();

			if (!this->ReadTextureMap(currentMaterial->alphaMap))
				return false;
		}

		else if (this->ArgEquals(0, "map_bump") || this->ArgEquals(0, "bump"))
		{
			if (currentMaterial->bumpMap == 0)
				currentMaterial->bumpMap = new AxFileObjTextureMap();

			if (!this->ReadTextureMap(currentMaterial->bumpMap))
				return false;
		}

		else if (this->ArgEquals(0, "map_disp") || this->ArgEquals(0, "disp"))
		{
			if (currentMaterial->displacementMap == 0)
				currentMaterial->displacementMap = new AxFileObjTextureMap();

			if (!this->ReadTextureMap(currentMaterial->displacementMap))
				return false;
		}

		else if (this->ArgEquals(0, "map_decal") || this->ArgEquals(0, "decal"))
		{
			if (currentMaterial->stencilMap == 0)
				currentMaterial->stencilMap = new AxFileObjTextureMap();

			if (!this->ReadTextureMap(currentMaterial->stencilMap))
				return false;
		}

		else if (this->ArgEquals(0, "map_refl") || this->ArgEquals(0, "refl"))
		{
			if (currentMaterial->reflectionMap == 0)
				currentMaterial->reflectionMap = new AxFileObjTextureMap();

			if (!this->ReadTextureMap(currentMaterial->reflectionMap))
				return false;
		}


		lastHeadArg = this->arguments[0];
		hasLastHeadArg = true;
	}

	delete[] this->data;
	this->data = 0;

	delete currentMesh;

	return success;
}