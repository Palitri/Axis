//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxTransform.h"

#include "..\..\Primitive\AxVector3.h"
#include "..\..\Primitive\AxVector4.h"

#include "..\..\Utilities\AxMaths.h"

#include "..\..\Utilities\Serialization\AxSerializationUtils.h"

AxTransform::AxTransform(void)
	: AxResource()
{
	this->typeId = AxTransform::classId;
    this->resourceType = AxResourceType_Transform;

    this->properties.Add(new AxProperty(AxString("Bone"), false));
    this->properties.Add(new AxProperty(AxString("Bone Index"), 0));

	AxMatrix::CreateIdentity(this->pivot);
	AxMatrix::CreateIdentity(this->transform);

	this->parent = 0;
}


AxTransform::~AxTransform(void)
{
}


void AxTransform::InsertTransformLayer(AxTransformOperation operation, int index)
{
    int numProperties = this->properties.count;

    int propertyIndex;
	if (index < this->transformLayers.count)
        propertyIndex = transformLayers[index].propertyFirst;
    else
        propertyIndex = numProperties;

    int propertyFirst = propertyIndex;

    switch (operation)
    {
        case AxTransformOperation_Translate:
            this->properties.Insert(propertyIndex++, new AxProperty("Translate X", 0.0f));
            this->properties.Insert(propertyIndex++, new AxProperty("Translate Y", 0.0f));
            this->properties.Insert(propertyIndex++, new AxProperty("Translate Z", 0.0f));
            break;

        case AxTransformOperation_RotateX:
            this->properties.Insert(propertyIndex++, new AxProperty("Rotate X", 0.0f));
            break;

        case AxTransformOperation_RotateY:
            this->properties.Insert(propertyIndex++, new AxProperty("Rotate Y", 0.0f));
            break;

        case AxTransformOperation_RotateZ:
            this->properties.Insert(propertyIndex++, new AxProperty("Rotate Z", 0.0f));
            break;

        case AxTransformOperation_Scale:
            this->properties.Insert(propertyIndex++, new AxProperty("Scale X", 1.0f));
            this->properties.Insert(propertyIndex++, new AxProperty("Scale Y", 1.0f));
            this->properties.Insert(propertyIndex++, new AxProperty("Scale Z", 1.0f));
            break;

        case AxTransformOperation_TranslateAbsolute:
            this->properties.Insert(propertyIndex++, new AxProperty("Translate AbsoluteX", 0.0f));
            this->properties.Insert(propertyIndex++, new AxProperty("Translate AbsoluteY", 0.0f));
            this->properties.Insert(propertyIndex++, new AxProperty("Translate AbsoluteZ", 0.0f));
            break;

        case AxTransformOperation_RotateAbsoluteX:
            this->properties.Insert(propertyIndex++, new AxProperty("Rotate AbsoluteX", 0.0f));
            break;

        case AxTransformOperation_RotateAbsoluteY:
            this->properties.Insert(propertyIndex++, new AxProperty("Rotate AbsoluteY", 0.0f));
            break;

        case AxTransformOperation_RotateAbsoluteZ:
            this->properties.Insert(propertyIndex++, new AxProperty("Rotate AbsoluteZ", 0.0f));
            break;

        case AxTransformOperation_ScaleAbsolute:
            this->properties.Insert(propertyIndex++, new AxProperty("Scale AbsoluteX", 1.0f));
            this->properties.Insert(propertyIndex++, new AxProperty("Scale AbsoluteY", 1.0f));
            this->properties.Insert(propertyIndex++, new AxProperty("Scale AbsoluteZ", 1.0f));
            break;

		case AxTransformOperation_Reflect:
			this->properties.Insert(propertyIndex++, new AxProperty("Reflect", 0, AxParameterType_ReferenceTransform));
            break;

		case AxTransformOperation_Skybox:
			this->properties.Insert(propertyIndex++, new AxProperty("Camera", 0, AxParameterType_ReferenceTransform));
            break;
    }

    int numNewProperties = this->properties.count - numProperties;
    this->transformLayers.Insert(index, AxTransformLayer(operation, propertyFirst, numNewProperties));

    int count = this->transformLayers.count;
    for (int i = index + 1; i < count; i++)
        this->transformLayers[i].propertyFirst += numNewProperties;
}

int AxTransform::AddTransformLayer(AxTransformOperation operation)
{
    this->InsertTransformLayer(operation, this->transformLayers.count);
    return this->transformLayers.count - 1;
}

void AxTransform::RemoveTransformLayer(int index)
{
    int count = this->transformLayers.count;
    for (int i = index + 1; i < count; i++)
        this->transformLayers[i].propertyFirst -= this->transformLayers[i].propertyCount;

	int propertyFirst = this->transformLayers[index].propertyFirst;
	int propertyCount = this->transformLayers[index].propertyCount;
	for (int i = 0; i < propertyCount; i++)
		delete this->properties[propertyFirst + i];

	this->properties.RemoveRange(propertyFirst, propertyCount);
	this->transformLayers.RemoveAt(index);
}

void AxTransform::ClearTransformLayers()
{
    this->transformLayers.Clear();
    this->properties.Clear();
}

void AxTransform::Process(AxTransform *parent)
{
	this->parent = parent;

	if (!this->oldPivot.Equals(this->pivot))
	{
		AxMatrix::Invert(this->pivotInverse, this->pivot);
		AxMatrix::Copy(this->oldPivot, this->pivot);
	}

	AxMatrix::Copy(this->localMatrix, this->transform);

	
	AxTransform *cameraTransform = 0;
	AxTransform *reflectionTransform = 0;
	int transformsCount = this->transformLayers.count;
    if (transformsCount != 0)
    {
		int propertyIndex = this->transformLayers[0].propertyFirst;
        for (int i = 0; i < transformsCount; i++)
		{
            switch (transformLayers[i].operation)
            {
                case AxTransformOperation_Translate:
				{
                    AxMatrix::CreateTranslation(this->m,
						*(float*)(this->properties[propertyIndex]->GetEffectiveParameter()->value),
						*(float*)(this->properties[propertyIndex + 1]->GetEffectiveParameter()->value),
						*(float*)(this->properties[propertyIndex + 2]->GetEffectiveParameter()->value));
					AxMatrix::Multiply(this->localMatrix, this->m, this->localMatrix);

                    propertyIndex += 3;
                    break;
				}

                case AxTransformOperation_RotateX:
				{
                    AxMatrix::CreateRotationX(this->m, *(float*)(this->properties[propertyIndex]->GetEffectiveParameter()->value));
					AxMatrix::Multiply(this->localMatrix, this->m, this->localMatrix);

                    propertyIndex++;
                    break;
				}

                case AxTransformOperation_RotateY:
				{
                    AxMatrix::CreateRotationY(this->m, *(float*)(this->properties[propertyIndex]->GetEffectiveParameter()->value));
                    AxMatrix::Multiply(this->localMatrix, this->m, this->localMatrix);

                    propertyIndex++;
                    break;
				}

                case AxTransformOperation_RotateZ:
				{
                    AxMatrix::CreateRotationZ(this->m, *(float*)(this->properties[propertyIndex]->GetEffectiveParameter()->value));
                    AxMatrix::Multiply(this->localMatrix, this->m, this->localMatrix);

                    propertyIndex++;
                    break;
				}

                // TODO: If the transform layers model is kept, make this ScaleUniform and add a Scale with separate X, Y and Z
				case AxTransformOperation_Scale:
				{
                    AxMatrix::CreateScaling(this->m,
						*(float*)(this->properties[propertyIndex]->GetEffectiveParameter()->value),
                        *(float*)(this->properties[propertyIndex + 1]->GetEffectiveParameter()->value),
                        *(float*)(this->properties[propertyIndex + 2]->GetEffectiveParameter()->value));
					AxMatrix::Multiply(this->localMatrix, this->m, this->localMatrix);

                    propertyIndex += 3;
                    break;
				}

                case AxTransformOperation_TranslateAbsolute:
				{
                    AxMatrix::CreateTranslation(this->m,
						*(float*)(this->properties[propertyIndex]->GetEffectiveParameter()->value),
                        *(float*)(this->properties[propertyIndex + 1]->GetEffectiveParameter()->value),
                        *(float*)(this->properties[propertyIndex + 2]->GetEffectiveParameter()->value));
					AxMatrix::Multiply(this->localMatrix, this->localMatrix, this->m);

                    propertyIndex += 3;
                    break;
				}

                case AxTransformOperation_RotateAbsoluteX:
				{
                    AxMatrix::CreateRotationX(this->m, *(float*)(this->properties[propertyIndex]->GetEffectiveParameter()->value));
                    AxMatrix::Multiply(this->localMatrix, this->localMatrix, this->m);

                    propertyIndex++;
                    break;
				}

                case AxTransformOperation_RotateAbsoluteY:
				{
                    AxMatrix::CreateRotationY(this->m, *(float*)(this->properties[propertyIndex]->GetEffectiveParameter()->value));
                    AxMatrix::Multiply(this->localMatrix, this->localMatrix, this->m);

                    propertyIndex++;
                    break;
				}

                case AxTransformOperation_RotateAbsoluteZ:
				{
                    AxMatrix::CreateRotationZ(this->m, *(float*)(this->properties[propertyIndex]->GetEffectiveParameter()->value));
                    AxMatrix::Multiply(this->localMatrix, this->localMatrix, this->m);

                    propertyIndex++;
                    break;
				}

                case AxTransformOperation_ScaleAbsolute:
				{
                    AxMatrix::CreateScaling(this->m,
						*(float*)(this->properties[propertyIndex]->GetEffectiveParameter()->value),
						*(float*)(this->properties[propertyIndex + 1]->GetEffectiveParameter()->value),
						*(float*)(this->properties[propertyIndex + 2]->GetEffectiveParameter()->value));
                    AxMatrix::Multiply(this->localMatrix, this->localMatrix, this->m);

                    propertyIndex += 3;
                    break;
				}

				case AxTransformOperation_Reflect:
				{
					reflectionTransform = (AxTransform*)(this->properties[propertyIndex]->GetEffectiveParameter()->value);
                    propertyIndex += 1;
                    break;
				}

				case AxTransformOperation_Skybox:
				{
					cameraTransform = (AxTransform*)(this->properties[propertyIndex]->GetEffectiveParameter()->value);
                    propertyIndex += 1;
					break;
				}
            }
		}
    }

	if (this->parent == 0)
		AxMatrix::Copy(this->worldMatrix, this->localMatrix);
	else 
		AxMatrix::Multiply(this->worldMatrix, this->localMatrix, this->parent->worldMatrix);

	if (reflectionTransform != 0)
	{
		AxVector3 planePoint, planeNormal;
		AxMatrix::GetTranslation(planePoint, reflectionTransform->worldMatrix);
		AxMatrix::GetAxisY(planeNormal, reflectionTransform->worldMatrix);
		AxVector4 plane;
		AxMaths::PlaneGeneralFromPointNormal(plane, planePoint, planeNormal);
		AxMatrix::CreateReflectionPlane(this->worldMatrix, plane);
	}

	AxMatrix::Multiply(this->pivotedWorldMatrix, this->pivotInverse, this->worldMatrix);

	if (cameraTransform != 0)
	{
		AxVector3 location;
		AxMatrix::GetTranslation(location, cameraTransform->worldMatrix);
		AxMatrix::SetTranslation(this->pivotedWorldMatrix, location);
		AxMatrix::SetScaling(this->pivotedWorldMatrix, AxVector3(100.0f));
	}

}

AxVector3 AxTransform::GetRightVectorExtrinsic()
{
	AxVector3 result;
	AxMatrix::GetAxisX(result, this->worldMatrix);
	return result;
}

AxVector3 AxTransform::GetRightVectorIntrinsic()
{
	AxVector3 result;
	AxMatrix::GetAxisX(result, this->transform);
	return result;
}

AxVector3 AxTransform::GetUpVectorExtrinsic()
{
	AxVector3 result;
	AxMatrix::GetAxisY(result, this->worldMatrix);
	return result;
}

AxVector3 AxTransform::GetUpVectorIntrinsic()
{
	AxVector3 result;
	AxMatrix::GetAxisY(result, this->transform);
	return result;
}

AxVector3 AxTransform::GetForwardVectorExtrinsic()
{
	AxVector3 result;
	AxMatrix::GetAxisZ(result, this->worldMatrix);
	return result;
}

AxVector3 AxTransform::GetForwardVectorIntrinsic()
{
	AxVector3 result;
	AxMatrix::GetAxisZ(result, this->transform);
	return result;
}

AxVector3 AxTransform::GetPositionExtrinsic()
{
	AxVector3 result;
	AxMatrix::GetTranslation(result, this->worldMatrix);
	return result;
}

AxVector3 AxTransform::GetPositionIntrinsic()
{
	AxVector3 result;
	AxMatrix::GetTranslation(result, this->localMatrix);
	return result;
}

void AxTransform::SetPositionExtrinsic(AxVector3 &position)
{
	if (this->parent == 0)
	{
		AxMatrix::SetTranslation(this->transform, position);
	}
	else
	{
		AxMatrix parentInvert;
		AxMatrix::Invert(parentInvert, this->parent->worldMatrix);
		AxMatrix::Multiply(this->transform, this->transform, this->parent->worldMatrix);

		AxMatrix::SetTranslation(this->transform, position);
		
		AxMatrix::Multiply(this->transform, this->transform, parentInvert);
	}

	this->Process(this->parent);
}

void AxTransform::SetPositionIntrinsic(AxVector3 &position)
{
	AxMatrix::SetTranslation(this->transform, position);
	
	this->Process(this->parent);
}

AxVector3 AxTransform::GetRotationExtrinsic()
{
	AxVector3 result;
	AxMatrix::ExtractEulerRotationXYZ(result, this->worldMatrix);
	return result;
}

AxVector3 AxTransform::GetRotationIntrinsic()
{
	AxVector3 result;
	AxMatrix::ExtractEulerRotationXYZ(result, this->transform);
	return result;
}

void AxTransform::SetRotationExtrinsic(AxVector3 &rotation)
{
	AxVector3 scaling, translation;
	AxMatrix::GetScaling(scaling, this->transform);
	AxMatrix::GetTranslation(translation, this->transform);

	if (this->parent == 0)
	{
		AxMatrix::CreateRotationX(this->transform, rotation.x);
		AxMatrix::CreateRotationY(this->m, rotation.y);
		AxMatrix::Multiply3x3(this->transform, this->transform, this->m);
		AxMatrix::CreateRotationZ(this->m, rotation.z);
		AxMatrix::Multiply3x3(this->transform, this->transform, this->m);
	}
	else
	{
		AxMatrix parentInvert;
		AxMatrix::Invert(parentInvert, this->parent->worldMatrix);

		AxMatrix::CreateRotationX(this->transform, rotation.x);
		AxMatrix::CreateRotationY(this->m, rotation.y);
		AxMatrix::Multiply3x3(this->transform, this->transform, this->m);
		AxMatrix::CreateRotationZ(this->m, rotation.z);
		AxMatrix::Multiply3x3(this->transform, this->transform, this->m);

		AxMatrix::Multiply(this->transform, this->transform, parentInvert);
	}

	AxMatrix::SetTranslation(this->transform, translation);
	AxMatrix::SetScaling(this->transform, scaling);

	this->Process(this->parent);
}

void AxTransform::SetRotationIntrinsic(AxVector3 &rotation)
{
	AxVector3 scaling, translation;
	AxMatrix::GetScaling(scaling, this->transform);
	AxMatrix::GetTranslation(translation, this->transform);

	AxMatrix::CreateRotationX(this->transform, rotation.x);
	AxMatrix::CreateRotationY(this->m, rotation.y);
	AxMatrix::Multiply3x3(this->transform, this->transform, this->m);
	AxMatrix::CreateRotationZ(this->m, rotation.z);
	AxMatrix::Multiply3x3(this->transform, this->transform, this->m);

	AxMatrix::SetTranslation(this->transform, translation);
	AxMatrix::SetScaling(this->transform, scaling);

	this->Process(this->parent);
}


AxVector3 AxTransform::GetScalingExtrinsic()
{
	AxVector3 result;
	AxMatrix::GetScaling(result, this->worldMatrix);
	return result;
}

AxVector3 AxTransform::GetScalingIntrinsic()
{
	AxVector3 result;
	AxMatrix::GetScaling(result, this->transform);
	return result;
}

void AxTransform::SetScalingExtrinsic(AxVector3 &scaling)
{
	if (this->parent == 0)
	{
		AxMatrix::SetScaling(this->transform, scaling);
	}
	else
	{
		AxMatrix parentInvert;
		AxMatrix::Invert(parentInvert, this->parent->worldMatrix);
		AxMatrix::Multiply(this->transform, this->transform, this->parent->worldMatrix);

		AxMatrix::SetScaling(this->transform, scaling);
		
		AxMatrix::Multiply(this->transform, this->transform, parentInvert);
	}

	this->Process(this->parent);
}

void AxTransform::SetScalingIntrinsic(AxVector3 &scaling)
{
	AxMatrix::SetScaling(this->transform, scaling);

	this->Process(this->parent);
}


void AxTransform::TranslateExtrinsic(AxVector3 &translation)
{
	AxMatrix::CreateTranslation(this->m, translation);
	AxMatrix::Multiply(this->transform, this->transform, this->m);

	this->Process(this->parent);
}

void AxTransform::TranslateIntrinsic(AxVector3 &translation)
{
	AxMatrix::CreateTranslation(this->m, translation);
	AxMatrix::Multiply(this->transform, this->m, this->transform);

	this->Process(this->parent);
}

void AxTransform::RotateExtrinsic(AxVector3 &angles)
{
	if (this->parent == 0)
	{
		AxMatrix::CreateRotationX(this->m, angles.x);
		AxMatrix::Multiply3x3(this->transform, this->transform, this->m);
		AxMatrix::CreateRotationY(this->m, angles.y);
		AxMatrix::Multiply3x3(this->transform, this->transform, this->m);
		AxMatrix::CreateRotationZ(this->m, angles.z);
		AxMatrix::Multiply3x3(this->transform, this->transform, this->m);
	}
	else
	{
		AxMatrix parentInvert;
		AxMatrix::Invert(parentInvert, this->parent->worldMatrix);
		AxMatrix::Multiply(this->transform, this->transform, this->parent->worldMatrix);

		AxMatrix::CreateRotationX(this->m, angles.x);
		AxMatrix::Multiply3x3(this->transform, this->transform, this->m);
		AxMatrix::CreateRotationY(this->m, angles.y);
		AxMatrix::Multiply3x3(this->transform, this->transform, this->m);
		AxMatrix::CreateRotationZ(this->m, angles.z);
		AxMatrix::Multiply3x3(this->transform, this->transform, this->m);

		AxMatrix::Multiply(this->transform, this->transform, parentInvert);
	}

	this->Process(this->parent);
}

void AxTransform::RotateIntrinsic(AxVector3 &angles)
{
	if (this->parent == 0)
	{
		AxMatrix::CreateRotationAxis(this->m, this->GetRightVectorExtrinsic().Normalize(), angles.x);
		AxMatrix::Multiply3x3(this->transform, this->transform, this->m);
		AxMatrix::CreateRotationAxis(this->m, this->GetUpVectorExtrinsic().Normalize(), angles.y);
		AxMatrix::Multiply3x3(this->transform, this->transform, this->m);
		AxMatrix::CreateRotationAxis(this->m, this->GetForwardVectorExtrinsic().Normalize(), angles.z);
		AxMatrix::Multiply3x3(this->transform, this->transform, this->m);
	}
	else
	{
		AxMatrix parentInvert;
		AxMatrix::Invert(parentInvert, this->parent->worldMatrix);
		AxMatrix::Multiply(this->transform, this->transform, this->parent->worldMatrix);

		AxMatrix::CreateRotationAxis(this->m, this->GetRightVectorExtrinsic().Normalize(), angles.x);
		AxMatrix::Multiply3x3(this->transform, this->transform, this->m);
		AxMatrix::CreateRotationAxis(this->m, this->GetUpVectorExtrinsic().Normalize(), angles.y);
		AxMatrix::Multiply3x3(this->transform, this->transform, this->m);
		AxMatrix::CreateRotationAxis(this->m, this->GetForwardVectorExtrinsic().Normalize(), angles.z);
		AxMatrix::Multiply3x3(this->transform, this->transform, this->m);

		AxMatrix::Multiply(this->transform, this->transform, parentInvert);
	}

	this->Process(this->parent);
}

void AxTransform::RotateAroundExtrinsic(AxVector3 &axis, float angle)
{
	if (this->parent == 0)
	{
		AxMatrix::CreateRotationAxis(this->m, axis, angle);
		AxMatrix::Multiply3x3(this->transform, this->transform, this->m);
	}
	else
	{
		AxMatrix parentInvert;
		AxMatrix::Invert(parentInvert, this->parent->worldMatrix);
		AxMatrix::Multiply(this->transform, this->transform, this->parent->worldMatrix);

		AxMatrix::CreateRotationAxis(this->m, axis, angle);
		AxMatrix::Multiply3x3(this->transform, this->transform, this->m);

		AxMatrix::Multiply(this->transform, this->transform, parentInvert);
	}

	this->Process(this->parent);
}

void AxTransform::RotateAroundIntrinsic(AxVector3 &axis, float angle)
{
	AxVector3 worldAxis;
	AxVector3::TransformNormal(worldAxis, axis, this->worldMatrix);

	if (this->parent == 0)
	{
		AxMatrix::CreateRotationAxis(this->m, worldAxis, angle);
		AxMatrix::Multiply3x3(this->transform, this->transform, this->m);
	}
	else
	{
		AxMatrix parentInvert;
		AxMatrix::Invert(parentInvert, this->parent->worldMatrix);
		AxMatrix::Multiply(this->transform, this->transform, this->parent->worldMatrix);

		AxMatrix::CreateRotationAxis(this->m, worldAxis, angle);
		AxMatrix::Multiply3x3(this->transform, this->transform, this->m);

		AxMatrix::Multiply(this->transform, this->transform, parentInvert);
	}

	this->Process(this->parent);
}

void AxTransform::RotateAroundRayExtrinsic(AxVector3 &rayPoint, AxVector3 &rayVector, float angle)
{
	AxVector3 rotationPivotPoint;
	AxMaths::PointToRayProjection(rotationPivotPoint, this->GetPositionExtrinsic(), rayPoint, rayVector);

	if (this->parent == 0)
	{
		AxMatrix::CreateTranslation(this->m, rotationPivotPoint.Invert());
		AxMatrix::Multiply(this->transform, this->transform, this->m);
		AxMatrix::CreateRotationAxis(this->m, rayVector, angle);
		AxMatrix::Multiply(this->transform, this->transform, this->m);
		AxMatrix::CreateTranslation(this->m, rotationPivotPoint);
		AxMatrix::Multiply(this->transform, this->transform, this->m);
	}
	else
	{
		AxMatrix parentInvert;
		AxMatrix::Invert(parentInvert, this->parent->worldMatrix);
		AxMatrix::Multiply(this->transform, this->transform, this->parent->worldMatrix);

		AxMatrix::CreateTranslation(this->m, rotationPivotPoint.Invert());
		AxMatrix::Multiply(this->transform, this->transform, this->m);
		AxMatrix::CreateRotationAxis(this->m, rayVector, angle);
		AxMatrix::Multiply(this->transform, this->transform, this->m);
		AxMatrix::CreateTranslation(this->m, rotationPivotPoint);
		AxMatrix::Multiply(this->transform, this->transform, this->m);

		AxMatrix::Multiply(this->transform, this->transform, parentInvert);
	}

	this->Process(this->parent);
}

void AxTransform::RotateAroundRayIntrinsic(AxVector3 &rayPoint, AxVector3 &rayVector, float angle)
{
	AxVector3 rotationPivotPoint;
	AxMaths::PointToRayProjection(rotationPivotPoint, this->GetPositionIntrinsic(), rayPoint, rayVector);

	if (this->parent == 0)
	{
		AxMatrix::CreateTranslation(this->m, rotationPivotPoint.Invert());
		AxMatrix::Multiply(this->transform, this->transform, this->m);
		AxMatrix::CreateRotationAxis(this->m, rayVector, angle);
		AxMatrix::Multiply(this->transform, this->transform, this->m);
		AxMatrix::CreateTranslation(this->m, rotationPivotPoint);
		AxMatrix::Multiply(this->transform, this->transform, this->m);
	}
	else
	{
		AxMatrix parentInvert;
		AxMatrix::Invert(parentInvert, this->parent->worldMatrix);
		AxMatrix::Multiply(this->transform, this->transform, this->parent->worldMatrix);

		AxMatrix::CreateTranslation(this->m, rotationPivotPoint.Invert());
		AxMatrix::Multiply(this->transform, this->transform, this->m);
		AxMatrix::CreateRotationAxis(this->m, rayVector, angle);
		AxMatrix::Multiply(this->transform, this->transform, this->m);
		AxMatrix::CreateTranslation(this->m, rotationPivotPoint);
		AxMatrix::Multiply(this->transform, this->transform, this->m);

		AxMatrix::Multiply(this->transform, this->transform, parentInvert);
	}

	this->Process(this->parent);
}

void AxTransform::ScaleExtrinsic(AxVector3 &scaling)
{
	if (this->parent == 0)
	{
		AxMatrix::CreateScaling(this->m, scaling);
		AxMatrix::Multiply3x3(this->transform, this->transform, this->m);
	}
	else
	{
		AxMatrix parentInvert;
		AxMatrix::Invert(parentInvert, this->parent->worldMatrix);
		AxMatrix::Multiply(this->transform, this->transform, this->parent->worldMatrix);

		AxMatrix::CreateScaling(this->m, scaling);
		AxMatrix::Multiply3x3(this->transform, this->transform, this->m);

		AxMatrix::Multiply(this->transform, this->transform, parentInvert);
	}

	this->Process(this->parent);
}

void AxTransform::ScaleIntrinsic(AxVector3 &scaling)
{
	AxMatrix::CreateScaling(this->m, scaling);
	AxMatrix::Multiply3x3(this->transform, this->m, this->transform);

	this->Process(this->parent);
}

void AxTransform::RelateToIdentity()
{
	AxMatrix::Copy(this->transform, this->worldMatrix);
}

void AxTransform::RelateTo(AxMatrix &parent)
{
	AxMatrix newParentInvert;
	AxMatrix::Invert(newParentInvert, parent);
	AxMatrix::Multiply(this->transform, this->worldMatrix, newParentInvert);
}

void AxTransform::RelateTo(AxTransform *parent)
{
	if (parent == 0)
		this->RelateToIdentity();
	else
		this->RelateTo(parent->worldMatrix);
}

void AxTransform::SerializeChunks(AxHierarchyStreamWriter &writer)
{
	writer.BeginChunk(AxTransform::SerializationId_Pivot);
	AxSerializationUtils::SerializeMatrix(*writer.stream, this->pivot);
	writer.EndChunk();

	writer.BeginChunk(AxTransform::SerializationId_Transform);
	AxSerializationUtils::SerializeMatrix(*writer.stream, this->transform);
	writer.EndChunk();

	writer.BeginChunk(AxTransform::SerializationId_BindInverse);
	AxSerializationUtils::SerializeMatrix(*writer.stream, this->boneBindPoseInverse);
	writer.EndChunk();

	writer.BeginChunk(AxTransform::SerializationId_TransformLayers);
	for (int i = 0; i < this->transformLayers.count; i++)
	{
		writer.BeginChunk(AxTransform::SerializationId_TransformLayer);
		writer.stream->WriteInt16(this->transformLayers[i].operation);
		writer.EndChunk();
	}
	writer.EndChunk();

    // Base serialization. Call it at the end, because transformation layers create properties, which need to be filled afterwards
	AxResource::SerializeChunks(writer);
}

bool AxTransform::DeserializeChunk(AxHierarchyStreamReader &reader)
{
	if (AxResource::DeserializeChunk(reader))
		return true;

	switch (reader.chunkId)
	{
		case AxTransform::SerializationId_Pivot:
		{
			AxSerializationUtils::DeserializeMatrix(this->pivot, *reader.stream);
			break;
		}

		case AxTransform::SerializationId_Transform:
		{
			AxSerializationUtils::DeserializeMatrix(this->transform, *reader.stream);
			break;
		}

		case AxTransform::SerializationId_BindInverse:
		{
			AxSerializationUtils::DeserializeMatrix(this->boneBindPoseInverse, *reader.stream);
			break;
		}

		case AxTransform::SerializationId_TransformLayers:
		{
			break;
		}

		case AxTransform::SerializationId_TransformLayer:
		{
			this->AddTransformLayer((AxTransformOperation)reader.stream->ReadInt16());
			break;
		}

		default:
		{
			return false;
		}
	}

	return true;
}