//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxFbxScene.h"

#include "AxFbxKeyframesTrack.h"

AxFbxScene::AxFbxScene(void)
{
}


AxFbxScene::~AxFbxScene(void)
{
}

AxVector2 AxFbxScene::FbxToAxVector2(FbxDouble2 fbxVector)
{
	return AxVector2((float)fbxVector[0], (float)fbxVector[1]);
}

AxVector3 AxFbxScene::FbxToAxVector3(FbxDouble3 fbxVector)
{
	return AxVector3((float)fbxVector[0], (float)fbxVector[1], (float)fbxVector[2]);
}

AxVector3 AxFbxScene::FbxToAxVector3(FbxDouble4 fbxVector)
{
	return AxVector3((float)fbxVector[0], (float)fbxVector[1], (float)fbxVector[2]);
}

void AxFbxScene::FbxToAxMatrix(AxMatrix &result, FbxAMatrix *fbxMatrix)
{
	result._11 = (float)fbxMatrix->Get(0, 0);
	result._12 = (float)fbxMatrix->Get(0, 1);
	result._13 = (float)fbxMatrix->Get(0, 2);
	result._14 = (float)fbxMatrix->Get(0, 3);
	result._21 = (float)fbxMatrix->Get(1, 0);
	result._22 = (float)fbxMatrix->Get(1, 1);
	result._23 = (float)fbxMatrix->Get(1, 2);
	result._24 = (float)fbxMatrix->Get(1, 3);
	result._31 = (float)fbxMatrix->Get(2, 0);
	result._32 = (float)fbxMatrix->Get(2, 1);
	result._33 = (float)fbxMatrix->Get(2, 2);
	result._34 = (float)fbxMatrix->Get(2, 3);
	result._41 = (float)fbxMatrix->Get(3, 0);
	result._42 = (float)fbxMatrix->Get(3, 1);
	result._43 = (float)fbxMatrix->Get(3, 2);
	result._44 = (float)fbxMatrix->Get(3, 3);
}

void AxFbxScene::FbxCreateRotationMatrix(AxMatrix &result, AxVector3 eulerAngles, EFbxRotationOrder rotationOrder)
{
	AxVector3 rotation;
	AxVector3::Scale(rotation, eulerAngles, AxMath::Pi / 180.0f);

	AxMatrix m;

	switch (rotationOrder)
	{
		case eEulerXYZ:
		{
			AxMatrix::CreateRotationX(result, rotation.x);
			AxMatrix::CreateRotationY(m, rotation.y);
			AxMatrix::Multiply(result, result, m);
			AxMatrix::CreateRotationZ(m, rotation.z);
			AxMatrix::Multiply(result, result, m);

			break;
		}
		
		case eEulerXZY:
		{
			AxMatrix::CreateRotationX(result, rotation.x);
			AxMatrix::CreateRotationZ(m, rotation.y);
			AxMatrix::Multiply(result, result, m);
			AxMatrix::CreateRotationY(m, rotation.z);
			AxMatrix::Multiply(result, result, m);

			break;
		}

		case eEulerYZX:
		{
			AxMatrix::CreateRotationY(result, rotation.x);
			AxMatrix::CreateRotationZ(m, rotation.y);
			AxMatrix::Multiply(result, result, m);
			AxMatrix::CreateRotationX(m, rotation.z);
			AxMatrix::Multiply(result, result, m);

			break;
		}

		case eEulerYXZ:
		{
			AxMatrix::CreateRotationY(result, rotation.x);
			AxMatrix::CreateRotationX(m, rotation.y);
			AxMatrix::Multiply(result, result, m);
			AxMatrix::CreateRotationZ(m, rotation.z);
			AxMatrix::Multiply(result, result, m);

			break;
		}

		case eEulerZXY:
		{
			AxMatrix::CreateRotationZ(result, rotation.x);
			AxMatrix::CreateRotationX(m, rotation.y);
			AxMatrix::Multiply(result, result, m);
			AxMatrix::CreateRotationY(m, rotation.z);
			AxMatrix::Multiply(result, result, m);

			break;
		}

		case eEulerZYX:
		{
			AxMatrix::CreateRotationZ(result, rotation.x);
			AxMatrix::CreateRotationY(m, rotation.y);
			AxMatrix::Multiply(result, result, m);
			AxMatrix::CreateRotationX(m, rotation.z);
			AxMatrix::Multiply(result, result, m);

			break;
		}

		case eSphericXYZ:
		{
			AxMatrix::CreateRotationX(result, rotation.x);
			AxMatrix::CreateRotationY(m, rotation.y);
			AxMatrix::Multiply(result, m, result);
			AxMatrix::CreateRotationZ(m, rotation.z);
			AxMatrix::Multiply(result, m, result);

			break;
		}

		default:
		{
			AxMatrix::CreateIdentity(result);
		}
	}
}

// Geometric transformation for the fbx node. This transformation should not be inherited to children. It is to be applied directly to the position of the node's attributes in object space
void AxFbxScene::GetNodeGeometryTransform(AxMatrix &transform, FbxNode *fbxNode)
{
	AxMatrix m;
	AxMatrix::CreateTranslation(transform, this->FbxToAxVector3(fbxNode->GetGeometricTranslation(FbxNode::eSourcePivot)));
	this->FbxCreateRotationMatrix(m, this->FbxToAxVector3(fbxNode->GetGeometricRotation(FbxNode::eSourcePivot)), fbxNode->RotationOrder);
	AxMatrix::Multiply(transform, m, transform);
	AxMatrix::CreateScaling(m, this->FbxToAxVector3(fbxNode->GetGeometricScaling(FbxNode::eSourcePivot)));
	AxMatrix::Multiply(transform, m, transform);
}

void AxFbxScene::LoadNodeDefaultTransform(AxTransform *transform, FbxNode *fbxNode)
{
	AxMatrix m;

	AxVector3 translation = this->FbxToAxVector3(fbxNode->LclTranslation.Get());
	AxMatrix::CreateTranslation(transform->transform, translation);


		AxVector3 total;
		AxVector3 rotationOffset = this->FbxToAxVector3(fbxNode->RotationOffset.Get());
		AxVector3 rotationPivot = this->FbxToAxVector3(fbxNode->RotationPivot.Get());
		AxVector3 preRotation = this->FbxToAxVector3(fbxNode->PreRotation.Get());
		//AxVector3::Add(total, rotationOffset, rotationPivot);
		//AxVector3::Add(total, total, preRotation);
		//AxMatrix::CreateTranslation(m, total);
		//AxMatrix::Multiply(transform->manual, m, transform->manual);


	AxVector3 rotation = this->FbxToAxVector3(fbxNode->LclRotation.Get());
	this->FbxCreateRotationMatrix(m, rotation, fbxNode->RotationOrder);
	AxMatrix::Multiply(transform->transform, m, transform->transform);


		AxVector3 postRotation = this->FbxToAxVector3(fbxNode->PostRotation.Get());
		AxVector3 rotationPivotInverse;
		AxVector3::Scale(rotationPivotInverse, rotationPivot, -1.0f);
		AxVector3 scalingOffset = this->FbxToAxVector3(fbxNode->ScalingOffset.Get());
		AxVector3 scalingPivot = this->FbxToAxVector3(fbxNode->ScalingPivot.Get());
		//AxVector3::Add(total, postRotation, rotationPivotInverse);
		//AxVector3::Add(total, total, scalingOffset);
		//AxVector3::Add(total, total, scalingPivot);
		//AxMatrix::CreateTranslation(m, total);
		//AxMatrix::Multiply(transform->manual, m, transform->manual);


	AxVector3 scaling = this->FbxToAxVector3(fbxNode->LclScaling.Get());
	AxMatrix::CreateScaling(m, scaling);
	AxMatrix::Multiply(transform->transform, m, transform->transform);


		AxVector3 scalingPivotInverse;
		AxVector3::Scale(scalingPivotInverse, scalingPivot, -1.0f);
		//AxMatrix::CreateTranslation(m, scalingPivotInverse);
		//AxMatrix::Multiply(transform->manual, m, transform->manual);
}

//bool AxFbxScene::LoadNodeKeyframeAnimation(AxEntitySet *entitySet, FbxNode* fbxNode, AxTransform *transformToAnimate)
//{
//	AxString name = fbxNode->GetName();
//	
//	float frameRate = FbxTime::GetFrameRate(this->fbxScene->GetGlobalSettings().GetTimeMode());
//	AxVector3 v1 = this->FbxToAxVector3(fbxNode->RotationOffset.Get());
//	//if ((v.x != 0.0f) || (v.y != 0.0f) || (v.z != 0.0f))
//	//	return false;
//	AxVector3 v2 = this->FbxToAxVector3(fbxNode->RotationPivot.Get());
//	//if ((v.x != 0.0f) || (v.y != 0.0f) || (v.z != 0.0f))
//	//	return false;
//	AxVector3 v3 = this->FbxToAxVector3(fbxNode->PreRotation.Get());
//	//if ((v.x != 0.0f) || (v.y != 0.0f) || (v.z != 0.0f))
//	//	return false;
//	AxVector3 v4 = this->FbxToAxVector3(fbxNode->PostRotation.Get());
//	//if ((v.x != 0.0f) || (v.y != 0.0f) || (v.z != 0.0f))
//	//	return false;
//	AxVector3 v5 = this->FbxToAxVector3(fbxNode->ScalingOffset.Get());
//	//if ((v.x != 0.0f) || (v.y != 0.0f) || (v.z != 0.0f))
//	//	return false;
//	AxVector3 v6 = this->FbxToAxVector3(fbxNode->ScalingPivot.Get());
//	//if ((v.x != 0.0f) || (v.y != 0.0f) || (v.z != 0.0f))
//	//	return false;
//
//	AxVector3 ro = this->FbxToAxVector3(fbxNode->RotationOffset.Get());
//	AxVector3 rp = this->FbxToAxVector3(fbxNode->RotationPivot.Get());
//	AxVector3 pr = this->FbxToAxVector3(fbxNode->PreRotation.Get());
//	AxVector3 sr = this->FbxToAxVector3(fbxNode->PostRotation.Get());
//	AxVector3 so = this->FbxToAxVector3(fbxNode->ScalingOffset.Get());
//	AxVector3 sp = this->FbxToAxVector3(fbxNode->ScalingPivot.Get());
//
//	AxMatrix geometryTransform;
//	this->GetNodeGeometryTransform(geometryTransform, fbxNode);
//	transformToAnimate->pivot = geometryTransform;
//
//	bool isAnimated = false;
//
//	int numAnimations = this->fbxScene->GetSrcObjectCount(FbxAnimStack::ClassId);
//	for (int animationIndex = 0; animationIndex < numAnimations; animationIndex++)
//	{
//		FbxAnimStack *animStack = (FbxAnimStack*)this->fbxScene->GetSrcObject(FbxAnimStack::ClassId, animationIndex);
//		AxString animationName = animStack->GetName();
//
//		//FbxAnimEvaluator *animEvaluator = this->fbxScene->GetEvaluator();
//		//animEvaluator->SetContext(animStack);
//
//
//		int numLayers = animStack->GetMemberCount();
//		for (int layerIndex = 0; layerIndex < numLayers; layerIndex++)
//		{
//			FbxAnimLayer *animLayer = (FbxAnimLayer*)animStack->GetMember(layerIndex);
//			AxString layerName = animLayer->GetName();
//
//			AxFbxKeyframesTrack translationTrack;
//			translationTrack.ProcessFbxCurve(fbxNode->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X), AxFbxKeyFrameValueComponentX);
//			translationTrack.ProcessFbxCurve(fbxNode->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y), AxFbxKeyFrameValueComponentY);
//			translationTrack.ProcessFbxCurve(fbxNode->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z), AxFbxKeyFrameValueComponentZ);
//
//			AxFbxKeyframesTrack rotationTrack;
//			rotationTrack.ProcessFbxCurve(fbxNode->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X), AxFbxKeyFrameValueComponentX);
//			rotationTrack.ProcessFbxCurve(fbxNode->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y), AxFbxKeyFrameValueComponentY);
//			rotationTrack.ProcessFbxCurve(fbxNode->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z), AxFbxKeyFrameValueComponentZ);
//
//			AxFbxKeyframesTrack scalingTrack;
//			scalingTrack.ProcessFbxCurve(fbxNode->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X), AxFbxKeyFrameValueComponentX, 1.0f);
//			scalingTrack.ProcessFbxCurve(fbxNode->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y), AxFbxKeyFrameValueComponentY, 1.0f);
//			scalingTrack.ProcessFbxCurve(fbxNode->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z), AxFbxKeyFrameValueComponentZ, 1.0f);
//
//			if ((translationTrack.keyframes.count != 0) || (rotationTrack.keyframes.count != 0) || (scalingTrack.keyframes.count != 0))
//			{
//				AxKeyframeAnimationMechanism *keyFrames = (AxKeyframeAnimationMechanism*)this->context->AddResource(*new AxKeyframeAnimationMechanism());
//				keyFrames->name = this->context->AcquireResourceName(animationName, AxResourceType_Mechanism);
//				entitySet->references.Add(keyFrames);
//				
//				keyFrames->properties[AxKeyframeAnimationMechanism::propertyIndex_Frame]->SetReference(AxParameterType_ReferenceMechanism, this->animationFrame);
//				keyFrames->properties[AxKeyframeAnimationMechanism::propertyIndex_Transform]->SetReference(AxParameterType_ReferenceTransform, transformToAnimate);
//
//
//				// Translation
//				if (translationTrack.keyframes.count > 0)
//				{
//					// TEST METHOD 1
//					//AxVector3 t1 = translationTrack.keyframes[0].value;
//					AxVector3 t1(0);
//					AxVector3 t1t;
//					FbxAnimCurve *c = fbxNode->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
//					if (c != 0)
//					{
//						if (c->KeyGetCount() > 0)
//						{
//							t1.x = c->KeyGetValue(0);
//							t1t.x = c->KeyGetTime(0).GetSecondDouble();
//						}
//					}
//					c = fbxNode->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
//					if (c != 0)
//					{
//						if (c->KeyGetCount() > 0)
//						{
//							t1.y = c->KeyGetValue(0);
//							t1t.y = c->KeyGetTime(0).GetSecondDouble();
//						}
//					}
//					c = fbxNode->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
//					if (c != 0)
//					{
//						if (c->KeyGetCount() > 0)
//						{
//							t1.z = c->KeyGetValue(0);
//							t1t.z = c->KeyGetTime(0).GetSecondDouble();
//						}
//					}
//
//					// TEST METHOD 2
//					AxVector3 t2(0);
//					FbxAnimCurve *translationCurve = fbxNode->LclTranslation.GetCurve(animLayer);
//					if (translationCurve != 0)
//					{
//						if (translationCurve->KeyGetCount() > 0)
//						{
//							FbxAnimEvaluator *animEvaluator = this->fbxScene->GetAnimationEvaluator();
//							FbxTime frameTime = translationCurve->KeyGetTime(0);
//							t2 = this->FbxToAxVector3(animEvaluator->GetNodeLocalTranslation(fbxNode, frameTime));
//						}
//					}
//
//					// TEST METHOD 3
//					AxVector3 t3 = this->FbxToAxVector3(fbxNode->LclTranslation.Get());
//
//					// TEST METHOD 4
//					AxVector3 t4(0);
//					if (translationCurve != 0)
//					{
//						if (translationCurve->KeyGetCount() > 0)
//						{
//							FbxTime frameTime = translationCurve->KeyGetTime(0);
//							t4 = this->FbxToAxVector3(fbxNode->EvaluateLocalTranslation(frameTime));
//						}
//					}
//
//
//
//					for (int keyIndex = 0; keyIndex < translationTrack.keyframes.count; keyIndex++)
//					{
//						keyFrames->translation.Add(new AxKeyframeAnimationMechanism::TranslationFrame(translationTrack.keyframes[keyIndex].time, translationTrack.keyframes[keyIndex].value));
//						//keyFrames->translation.Add(new AxKeyframeAnimationMechanism::TranslationFrame(translationTrack.keyframes[keyIndex].time, translationTrack.keyframes[0].value));
//					}
//				}
//				else
//				{
//					AxVector3 translation = this->FbxToAxVector3(fbxNode->LclTranslation.Get());
//
//					//AxVector3 pivot = this->FbxToAxVector3(fbxNode->RotationPivot);
//					//AxVector3::Add(translation, translation, pivot);
//					//AxMatrix::CreateTranslation(transformToAnimate->pivot, pivot);
//						
//					keyFrames->translation.Add(new AxKeyframeAnimationMechanism::TranslationFrame(0.0f, translation));
//				}
//				
//				
//				// Rotation
//				if (rotationTrack.keyframes.count > 0)
//				{
//					for (int keyIndex = 0; keyIndex < rotationTrack.keyframes.count; keyIndex++)
//					{
//						AxVector3::Scale(rotationTrack.keyframes[keyIndex].value, rotationTrack.keyframes[keyIndex].value, AxMath::Pi / 180.0f);
//						keyFrames->rotationEuler.Add(new AxKeyframeAnimationMechanism::RotationEulerFrame(rotationTrack.keyframes[keyIndex].time, rotationTrack.keyframes[keyIndex].value));
//					}
//				}
//				else
//				{
//					AxVector3 rotation = this->FbxToAxVector3(fbxNode->LclRotation.Get());
//					AxVector3::Scale(rotation, rotation, AxMath::Pi / 180.0f);
//
//					keyFrames->rotationEuler.Add(new AxKeyframeAnimationMechanism::RotationEulerFrame(0.0f, rotation));
//				}
//				
//
//				// Scaling
//				if (scalingTrack.keyframes.count > 0)
//				{
//					for (int keyIndex = 0; keyIndex < scalingTrack.keyframes.count; keyIndex++)
//					{
//						keyFrames->scaling.Add(new AxKeyframeAnimationMechanism::ScalingFrame(scalingTrack.keyframes[keyIndex].time, scalingTrack.keyframes[keyIndex].value));
//					}
//				}
//				else
//				{
//					AxVector3 scaling = this->FbxToAxVector3(fbxNode->LclScaling.Get());
//					keyFrames->scaling.Add(new AxKeyframeAnimationMechanism::ScalingFrame(0.0f, scaling));
//				}
//
//				// Set the frame mechanism to the biggest number of frames
//				float animationTime = AxMath::Max(AxMath::Max(keyFrames->translation[keyFrames->translation.count - 1]->frame, keyFrames->rotationEuler[keyFrames->rotationEuler.count - 1]->frame), keyFrames->scaling[keyFrames->scaling.count - 1]->frame);
//				this->animationFrame->properties[AxLinearMechanism::propertyIndex_Max]->SetFloat(AxMath::Max(this->animationFrame->properties[AxLinearMechanism::propertyIndex_Max]->GetFloat(), animationTime));
//			
//				keyFrames->BuildKeyframeTransformations();
//
//				return true;
//			}
//		}
//	}
//
//	return isAnimated;
//}

bool AxFbxScene::LoadNodeKeyframeAnimation(AxEntitySet *entitySet, FbxNode* fbxNode, AxTransform *transformToAnimate)
{
	AxString name = fbxNode->GetName();
	FbxEuler::EOrder rotationOrder = fbxNode->RotationOrder;

	AxMatrix geometryTransform;
	this->GetNodeGeometryTransform(geometryTransform, fbxNode);
	transformToAnimate->pivot = geometryTransform;

	AxVector3 ro = this->FbxToAxVector3(fbxNode->RotationOffset.Get());
	AxVector3 rp = this->FbxToAxVector3(fbxNode->RotationPivot.Get());
	AxVector3 pr = this->FbxToAxVector3(fbxNode->PreRotation.Get());
	AxVector3 sr = this->FbxToAxVector3(fbxNode->PostRotation.Get());
	AxVector3 so = this->FbxToAxVector3(fbxNode->ScalingOffset.Get());
	AxVector3 sp = this->FbxToAxVector3(fbxNode->ScalingPivot.Get());
	AxMatrix preRotation;
	this->FbxCreateRotationMatrix(preRotation, pr, rotationOrder);

	bool isAnimated = false;

	int numAnimations = this->fbxScene->GetSrcObjectCount(FbxAnimStack::ClassId);
	for (int animationIndex = 0; animationIndex < numAnimations; animationIndex++)
	{
		FbxAnimStack *animStack = (FbxAnimStack*)this->fbxScene->GetSrcObject(FbxAnimStack::ClassId, animationIndex);
		AxString animationName = animStack->GetName();

		FbxAnimEvaluator *animEvaluator = this->fbxScene->GetAnimationEvaluator();
		//animEvaluator->SetContext(animStack);

		int numLayers = animStack->GetMemberCount();
		for (int layerIndex = 0; layerIndex < numLayers; layerIndex++)
		{
			FbxAnimLayer *animLayer = (FbxAnimLayer*)animStack->GetMember(layerIndex);
			AxString layerName = animLayer->GetName();

			FbxAnimCurve *translationCurve = fbxNode->LclTranslation.GetCurve(animLayer);
			FbxAnimCurve *rotationCurve = fbxNode->LclRotation.GetCurve(animLayer);
			FbxAnimCurve *scalingCurve = fbxNode->LclScaling.GetCurve(animLayer);

			bool hasAnimation = (translationCurve != 0) || (rotationCurve != 0) || (scalingCurve != 0);

			if (hasAnimation)
			{
				AxKeyframeAnimationMechanism *keyFrames = (AxKeyframeAnimationMechanism*)this->context->AddResource(new AxKeyframeAnimationMechanism(this->context));
				keyFrames->name = this->context->AcquireResourceName(animationName, AxResourceType_Mechanism);
				entitySet->references.Add(keyFrames);
				
				keyFrames->properties[AxKeyframeAnimationMechanism::propertyIndex_Frame]->SetValue(this->animationFrame, AxParameterType_ReferenceMechanism);
				keyFrames->properties[AxKeyframeAnimationMechanism::propertyIndex_Transform]->SetValue(transformToAnimate, AxParameterType_ReferenceTransform);

				if (scalingCurve != 0)
				{
					AxKeyFramesTrack *scalingTrack = keyFrames->keyFramesTracks.Add(new AxKeyFramesTrack());

					int numKeys = scalingCurve->KeyGetCount();
					for (int keyIndex = 0; keyIndex < numKeys; keyIndex++)
					{
						FbxTime frameTime = scalingCurve->KeyGetTime(keyIndex);
						//AxVector3 scaling = this->FbxToAxVector3(animEvaluator->GetNodeLocalScaling(fbxNode, frameTime));
						AxVector3 scaling = this->FbxToAxVector3(fbxNode->EvaluateLocalScaling(frameTime));
						//AxVector3 scaling(1.0f);

						float frameSeconds = (float)frameTime.GetSecondDouble();
						scalingTrack->Add(new AxKeyFrameScaling(frameSeconds, scaling));

						this->animationFrame->properties[AxLinearMechanism::propertyIndex_Max]->SetFloat(AxMath::Max(this->animationFrame->properties[AxLinearMechanism::propertyIndex_Max]->GetFloat(), frameSeconds));
					}
				}
				else
				{
					AxKeyFramesTrack *scalingTrack = keyFrames->keyFramesTracks.Add(new AxKeyFramesTrack());
					AxVector3 scaling = this->FbxToAxVector3(fbxNode->LclScaling.Get());
					scalingTrack->Add(new AxKeyFrameScaling(0.0f, scaling));
				}

				if (rotationCurve != 0)
				{
					AxKeyFramesTrack *rotationTrack = keyFrames->keyFramesTracks.Add(new AxKeyFramesTrack());

					int numKeys = rotationCurve->KeyGetCount();
					for (int keyIndex = 0; keyIndex < numKeys; keyIndex++)
					{
						FbxTime frameTime = rotationCurve->KeyGetTime(keyIndex);
						//AxVector3 rotation = this->FbxToAxVector3(animEvaluator->GetNodeLocalRotation(fbxNode, frameTime));
						AxVector3 rotation = this->FbxToAxVector3(fbxNode->EvaluateLocalRotation(frameTime));
						//AxVector3 rotation(0.0f);
						AxVector3::Scale(rotation, rotation, AxMath::Pi / 180.0f);
						//AxMatrix m;
						//this->FbxCreateRotationMatrix(m, rotation, rotationOrder);
						//AxMatrix::Multiply(m, preRotation, m);
						//AxMatrix::ExtractEulerRotation(rotation, m);
						//if (rotation.x < 0)
						//	rotation.x += AxMath::Pi2;
						//if (rotation.y < 0)
						//	rotation.y += AxMath::Pi2;
						//if (rotation.z < 0)
						//	rotation.z += AxMath::Pi2;

						float frameSeconds = (float)frameTime.GetSecondDouble();
						rotationTrack->Add(new AxKeyFrameRotationEuler(frameSeconds, rotation));

						this->animationFrame->properties[AxLinearMechanism::propertyIndex_Max]->SetFloat(AxMath::Max(this->animationFrame->properties[AxLinearMechanism::propertyIndex_Max]->GetFloat(), frameSeconds));
					}
				}
				else
				{
					AxKeyFramesTrack *rotationTrack = keyFrames->keyFramesTracks.Add(new AxKeyFramesTrack());
					AxVector3 rotation = this->FbxToAxVector3(fbxNode->LclRotation.Get());
					AxVector3::Scale(rotation, rotation, AxMath::Pi / 180.0f);
					rotationTrack->Add(new AxKeyFrameRotationEuler(0.0f, rotation));
				}

				if (translationCurve != 0)
				{
					AxKeyFramesTrack *translationTrack = keyFrames->keyFramesTracks.Add(new AxKeyFramesTrack());

					int directIndex = -1;
					int numKeys = translationCurve->KeyGetCount();
					for (int keyIndex = 0; keyIndex < numKeys; keyIndex++)
					{
						FbxTime frameTime = translationCurve->KeyGetTime(keyIndex);
						//AxVector3 translation = this->FbxToAxVector3(animEvaluator->GetNodeLocalTranslation(fbxNode, frameTime));
						AxVector3 translation = this->FbxToAxVector3(fbxNode->EvaluateLocalTranslation(frameTime));
						//AxVector3 translation(0.0f);

						float frameSeconds = (float)frameTime.GetSecondDouble();
						translationTrack->Add(new AxKeyFrameTranslation(frameSeconds, translation));

						this->animationFrame->properties[AxLinearMechanism::propertyIndex_Max]->SetFloat(AxMath::Max(this->animationFrame->properties[AxLinearMechanism::propertyIndex_Max]->GetFloat(), frameSeconds));
					}
				}
				else
				{
					AxKeyFramesTrack *translationTrack = keyFrames->keyFramesTracks.Add(new AxKeyFramesTrack());
					AxVector3 translation = this->FbxToAxVector3(fbxNode->LclTranslation.Get());
					translationTrack->Add(new AxKeyFrameTranslation(0.0f, translation));
				}

				return true;
			}
		}
	}

	return false;
}

bool AxFbxScene::GetMaterialPropertyTextureName(const FbxProperty &fbxProperty, AxString &fileName)
{
	if (!fbxProperty.IsValid())
		return false;

	// Check for layered textues. A layered texture contains number of standard textures
	int numLayeredTextures = fbxProperty.GetSrcObjectCount<FbxLayeredTexture>();
	if (numLayeredTextures > 0)
	{
		// Get only the first layered texture with index 0
		FbxLayeredTexture *layeredTexture = fbxProperty.GetSrcObject<FbxLayeredTexture>(0);
		int numTextures = layeredTexture->GetSrcObjectCount<FbxTexture>();
		if (numTextures > 0)
		{
			// Get only the first texture with index 0
			const FbxFileTexture* texture = layeredTexture->GetSrcObject<FbxFileTexture>(0);
			if (texture != 0)
			{
				//char *textureName = (char*)texture->GetUserDataPtr();
				fileName = texture->GetFileName();
				return true;
			}
		}
	}

	// Check for standard textures
	else 
	{
		const int numTextures = fbxProperty.GetSrcObjectCount<FbxFileTexture>();
		if (numTextures > 0)
		{
			// Get only the first texture with index 0
			const FbxFileTexture* texture = fbxProperty.GetSrcObject<FbxFileTexture>(0);
			if (texture != 0)
			{
				//char *textureName = (char*)texture->GetUserDataPtr();
				fileName = texture->GetFileName();
				return true;
			}
		}
	}

	return false;
}

AxVector4 AxFbxScene::GetMaterialPropertyColor(const FbxProperty &fbxProperty)
{
	if (!fbxProperty.IsValid())
		return AxVector4(0.0f, 0.0f, 0.0f, 1.0f);

	FbxDouble3 colorValue = fbxProperty.Get<FbxDouble3>();
	return AxVector4((float)colorValue[0], (float)colorValue[1], (float)colorValue[2], 1.0f);
}

float AxFbxScene::GetMaterialPropertyColorFactor(const FbxProperty &fbxProperty)
{
	if (!fbxProperty.IsValid())
		return 1.0f;

	return (float)fbxProperty.Get<FbxDouble>();
}

AxMaterial *AxFbxScene::AcquireNodeMaterial(FbxSurfaceMaterial *fbxMaterial)
{
	AxResource *existingMaterial;
	if (this->resourceMap.GetValue(fbxMaterial, existingMaterial))
	{
		return (AxMaterial*)existingMaterial;
	}
					
	AxMaterial *material = (AxMaterial*)this->context->AddResource(new AxMaterial(this->context));
	material->name = this->context->AcquireResourceName(AxString(fbxMaterial->GetName()), material->resourceType);

	this->resourceMap.Add(fbxMaterial, material);

	material->AddShadingLayer(AxShadingOperation_Material, AxColorOperation_None);

	material->AddShadingLayer(AxShadingOperation_AmbientLighting, AxColorOperation_Set);

	AxString textureName;
	AxVector4 colorValue;
	FbxProperty color;
			
	material->AddShadingLayer(AxShadingOperation_LightSource, AxColorOperation_None);
	material->AddShadingLayer(AxShadingOperation_LightSource, AxColorOperation_None);

	bool usePixelLighting = false;
	color = fbxMaterial->FindProperty(FbxSurfaceMaterial::sNormalMap);
	if (this->GetMaterialPropertyTextureName(color, textureName))
	{
		AxTexture2D *texture = this->context->AcquireTexture(textureName);
		if (texture != 0)
		{
			material->AddShadingLayer(AxShadingOperation_NormalMap, AxColorOperation_Set);
			material->GetProperty("Normal map")->SetValue(texture, AxParameterType_ReferenceTexture);

			usePixelLighting = true;
		}
	}

	if (usePixelLighting)
		material->AddShadingLayer(AxShadingOperation_PixelLighting, AxColorOperation_Add);
	else
		material->AddShadingLayer(AxShadingOperation_VertexLighting, AxColorOperation_Add);


	// TODO: emissive can be achieved with the FlatColor shader op
	color = fbxMaterial->FindProperty(FbxSurfaceMaterial::sEmissive);
	if (this->GetMaterialPropertyTextureName(color, textureName))
	{
		FbxProperty colorFactor = fbxMaterial->FindProperty(FbxSurfaceMaterial::sEmissiveFactor);
		AxVector4::Scale(colorValue, this->GetMaterialPropertyColor(color), this->GetMaterialPropertyColorFactor(colorFactor));
		material->GetProperty("Ambient")->SetColor(colorValue);
	}
	else
	{
		FbxProperty colorFactor = fbxMaterial->FindProperty(FbxSurfaceMaterial::sAmbientFactor);
		AxVector4::Scale(colorValue, this->GetMaterialPropertyColor(color), this->GetMaterialPropertyColorFactor(colorFactor));
		material->GetProperty("Ambient")->SetColor(colorValue);
	}

	//color = fbxMaterial->FindProperty(FbxSurfaceMaterial::sAmbient);
	//if (this->GetMaterialPropertyTextureName(color, textureName))
	//{
	//}
	//else
	//{
	//	//FbxProperty colorFactor = fbxMaterial->FindProperty(FbxSurfaceMaterial::sAmbientFactor);
	//	//AxVector4::Scale(colorValue, this->GetMaterialPropertyColor(color), this->GetMaterialPropertyColorFactor(colorFactor));
	//	//material->GetProperty("Ambient")->SetColor(colorValue);
	//	material->AddShadingLayer(AxShadingOperation_AmbientLighting, AxColorOperation_Add);
	//	material->GetProperty("Color map")->SetValue(texture, AxParameterType_ReferenceTexture);
	//}
			
	color = fbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
	if (this->GetMaterialPropertyTextureName(color, textureName))
	{
		AxTexture2D *texture = this->context->AcquireTexture(textureName);
		if (texture != 0)
		{
			material->AddShadingLayer(AxShadingOperation_ColorMap, AxColorOperation_Modulate);
			material->GetProperty("Color map")->SetValue(texture, AxParameterType_ReferenceTexture);
		}
	}
	else
	{
		FbxProperty colorFactor = fbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
		AxVector4::Scale(colorValue, this->GetMaterialPropertyColor(color), this->GetMaterialPropertyColorFactor(colorFactor));
		material->GetProperty("Diffuse")->SetColor(colorValue);
	}
			
	color = fbxMaterial->FindProperty(FbxSurfaceMaterial::sSpecular);
	if (this->GetMaterialPropertyTextureName(color, textureName))
	{
	}
	else
	{
		FbxProperty colorFactor = fbxMaterial->FindProperty(FbxSurfaceMaterial::sSpecularFactor);
		AxVector4::Scale(colorValue, this->GetMaterialPropertyColor(color), this->GetMaterialPropertyColorFactor(colorFactor));
		material->GetProperty("Specular")->SetColor(colorValue);
	}
			
	color = fbxMaterial->FindProperty(FbxSurfaceMaterial::sReflection);
	if (this->GetMaterialPropertyTextureName(color, textureName))
	{
		AxTexture2D *texture = this->context->AcquireTexture(textureName);
		if (texture != 0)
		{
			material->AddShadingLayer(AxShadingOperation_ReflectionMap, AxColorOperation_Add);
			material->GetProperty("Reflection map")->SetValue(texture, AxParameterType_ReferenceTexture);
		}
	}

	FbxProperty shininess = fbxMaterial->FindProperty(FbxSurfaceMaterial::sSpecularFactor);
	material->GetProperty("Gloss")->SetFloat(this->GetMaterialPropertyColorFactor(shininess));
	
	material->UpdateShader();

	return material;
}

void AxFbxScene::LoadMesh_Vertices(AxDeviceMesh *deviceMesh, FbxMesh *fbxMesh, AxMatrix &geometryTransform)
{
	// Load Vertices
	int numVertices = fbxMesh->GetControlPointsCount();
	FbxVector4 *verticesFbx = fbxMesh->GetControlPoints(); 
	deviceMesh->CreateVertexBuffer(numVertices);
	AxVector4 nullBone(0.0f);
	for (int vertexIndex = 0; vertexIndex < numVertices; vertexIndex++)
	{
		AxVector3 position(this->FbxToAxVector3(verticesFbx[vertexIndex]));
		//AxVector3::Transform(position, position, geometryTransform);
		deviceMesh->SetVertexPosition(vertexIndex, position);

		deviceMesh->SetVertexBones(vertexIndex, nullBone, nullBone);
	}
}

void AxFbxScene::LoadMesh_TextureCoords(AxDeviceMesh *deviceMesh, FbxMesh *fbxMesh)
{
	int numLayers = fbxMesh->GetLayerCount();
	for (int layerIndex = 0; layerIndex < numLayers; layerIndex++)
	{
		FbxLayer *layer = fbxMesh->GetLayer(layerIndex);
		
		FbxLayerElementUV *layerUV = layer->GetUVs();
        if (layerUV != 0)
		{
			FbxLayerElement::EMappingMode mappingMode = layerUV->GetMappingMode();
			FbxLayerElement::EReferenceMode referenceMode = layerUV->GetReferenceMode();

			const bool useIndex = referenceMode != FbxGeometryElement::eDirect;
			const int numPolygons = fbxMesh->GetPolygonCount();

			if (mappingMode == FbxGeometryElement::eByControlPoint )
			{
				int numVertices = fbxMesh->GetControlPointsCount();
				for (int vertexIndex = 0; vertexIndex < numVertices; vertexIndex++)
				{
					int texCoordsIndex = useIndex ? layerUV->GetIndexArray().GetAt(vertexIndex) : vertexIndex;
					AxVector2 texCoords = this->FbxToAxVector2(layerUV->GetDirectArray().GetAt(texCoordsIndex));
					
					deviceMesh->SetVertexTexCoords(vertexIndex, texCoords);
				}
			}

			// If uv is per polygon means each polygon has own uv mapping. Normally, this means polygons that share the same vertex position may have different uv, which requires to create a new vertex, but instead we'll only write to the existing vertices
			else if (mappingMode == FbxGeometryElement::eByPolygonVertex)
			{
				const int numIndices = (useIndex) ? layerUV->GetIndexArray().GetCount() : 0;
				
				int index = 0;
				for (int polyIndex = 0; polyIndex < numPolygons; polyIndex++)
				{
					const int numPolyVertices = fbxMesh->GetPolygonSize(polyIndex);
					for (int polyVertexIndex = 0; polyVertexIndex < numPolyVertices; polyVertexIndex++)
					{
						if (index < numIndices)
						{
							int texCoordsIndex = useIndex ? layerUV->GetIndexArray().GetAt(index) : index;
							AxVector2 texCoords = this->FbxToAxVector2(layerUV->GetDirectArray().GetAt(texCoordsIndex));

							deviceMesh->SetVertexTexCoords(fbxMesh->GetPolygonVertex(polyIndex, polyVertexIndex), texCoords);

							index++;
						}
					}
				}
			}
		}
	}
}

bool AxFbxScene::LoadMesh_Animation(AxDeviceMesh *deviceMesh, FbxMesh *fbxMesh)
{
	bool result = false;
	int numDeformers = fbxMesh->GetDeformerCount();
	FbxSkin* skin = (FbxSkin*)fbxMesh->GetDeformer(0, FbxDeformer::eSkin);
	if (skin != 0)
	{
		// Max 64 bones. Parameterize this, or make renderer bone limit to be read
		int boneCount = AxMath::Min(skin->GetClusterCount(), 64);
		int validBoneIndex = 0;
		for (int boneIndex = 0; boneIndex < boneCount; boneIndex++)
		{
			FbxCluster* cluster = skin->GetCluster(boneIndex);
			FbxNode* bone = cluster->GetLink();

			AxResource *axBone;
			if (this->resourceMap.GetValue(bone, axBone))
			{
				result = true;
				AxTransform *boneTransform = (AxTransform*)axBone;
				boneTransform->Process(0); // TODO: What's the reason for Process()?
				boneTransform->properties[AxTransform::propertyIndex_BoneIndex]->SetInt(boneIndex);
				boneTransform->properties[AxTransform::propertyIndex_Bone]->SetBool(true);

				FbxAMatrix bindPoseMatrix;
				cluster->GetTransformLinkMatrix(bindPoseMatrix);
				this->FbxToAxMatrix(boneTransform->boneBindPoseInverse, &bindPoseMatrix);
				AxMatrix::Invert(boneTransform->boneBindPoseInverse, boneTransform->boneBindPoseInverse);

				int *boneVertexIndices = cluster->GetControlPointIndices();
				double *boneVertexWeights = cluster->GetControlPointWeights();

				int numBoneVertexIndices = cluster->GetControlPointIndicesCount();
				for (int boneVertexIndex = 0; boneVertexIndex < numBoneVertexIndices; boneVertexIndex++) 
				{
					AxVector4 boneIndices, boneWeights;
					deviceMesh->GetVertexBones(boneVertexIndices[boneVertexIndex], boneIndices, boneWeights);
					if (boneWeights.x == 0.0f)
					{
						boneIndices.x = (float)validBoneIndex;
						boneWeights.x = (float)boneVertexWeights[boneVertexIndex];
						deviceMesh->SetVertexBones(boneVertexIndices[boneVertexIndex], boneIndices, boneWeights);
					}
					else if (boneWeights.y == 0.0f)
					{
						boneIndices.y = (float)validBoneIndex;
						boneWeights.y = (float)boneVertexWeights[boneVertexIndex];
						deviceMesh->SetVertexBones(boneVertexIndices[boneVertexIndex], boneIndices, boneWeights);
					}
					else if (boneWeights.z == 0.0f)
					{
						boneIndices.z = (float)validBoneIndex;
						boneWeights.z = (float)boneVertexWeights[boneVertexIndex];
						deviceMesh->SetVertexBones(boneVertexIndices[boneVertexIndex], boneIndices, boneWeights);
					}
					else if (boneWeights.w == 0.0f)
					{
						boneIndices.w = (float)validBoneIndex;
						boneWeights.w = (float)boneVertexWeights[boneVertexIndex];
						deviceMesh->SetVertexBones(boneVertexIndices[boneVertexIndex], boneIndices, boneWeights);
					}
				}

				validBoneIndex++;
			}
			else
			{
				// Debug tests
				AxString boneName = bone->GetName();

				// Get default attribute
				FbxNodeAttribute *nodeAttributeFbx = bone->GetNodeAttribute();
				if (nodeAttributeFbx != 0)
					FbxNodeAttribute::EType attributeType = nodeAttributeFbx->GetAttributeType();

				// Get all attributes
				int numAttributes = bone->GetNodeAttributeCount();
				for (int i = 0; i < numAttributes; i++)
				{
					FbxNodeAttribute *nodeAttributeFbx = bone->GetNodeAttributeByIndex(i);

					AxString name = AxString(bone->GetName());

					FbxNodeAttribute::EType attributeType = nodeAttributeFbx->GetAttributeType();
				}

			}
		}
	}

	return result;
}

AxSmoothingGroups* AxFbxScene::LoadMesh_CreateSmoothGrouping(FbxMesh *fbxMesh)
{
	// If no smoothing information is present, use fbx to generate it - first, compute soft/hard edge info from mesh normals, then convert the soft/hard edge info to smoothing group info
	FbxGeometryElementSmoothing *smoothing = fbxMesh->GetElementSmoothing();
	if (smoothing == 0)
	{
		FbxGeometryConverter geometryConverter(this->fbxManager);
		geometryConverter.ComputeEdgeSmoothingFromNormals(fbxMesh);
	    geometryConverter.ComputePolygonSmoothingFromEdgeSmoothing(fbxMesh);
		
		smoothing = fbxMesh->GetElementSmoothing();
	}
	// If smoothing information is present, but mapping is by edge, convert the soft/hard edge info to smoothing group info.
	else
	{
		if (smoothing->GetMappingMode() == FbxGeometryElement::eByEdge)
		{
			FbxGeometryConverter geometryConverter(this->fbxManager);
			geometryConverter.ComputePolygonSmoothingFromEdgeSmoothing(fbxMesh);
		}
	}

	int numPolygons = fbxMesh->GetPolygonCount();
	int numVertices = fbxMesh->GetControlPointsCount();
	
	if (smoothing != 0)
	{
		FbxLayerElement::EMappingMode mappingMode = smoothing->GetMappingMode();
		FbxLayerElement::EReferenceMode referenceMode = smoothing->GetReferenceMode();

		bool useIndexing = referenceMode != FbxGeometryElement::eDirect;

		if (mappingMode == FbxGeometryElement::eByPolygon)
		{
			AxSmoothingGroups *smoothGrouping = new AxSmoothingGroups(numPolygons, numVertices);

			for (int polyIndex = 0; polyIndex < numPolygons; polyIndex++)
			{
				int smoothingIndex = useIndexing ? smoothing->GetIndexArray().GetAt(polyIndex) : polyIndex;
				int smoothingGroupFlags = smoothing->GetDirectArray().GetAt(smoothingIndex);

				smoothGrouping->SetFaceSmoothingGroups(polyIndex, smoothingGroupFlags);

				int numPolygonVertices = fbxMesh->GetPolygonSize(polyIndex);
				for (int polyVertexIndex = 0; polyVertexIndex < numPolygonVertices; polyVertexIndex++)
				{
					smoothGrouping->AddFaceVertex(polyIndex, fbxMesh->GetPolygonVertex(polyIndex, polyVertexIndex));
				}
			}

			return smoothGrouping;
		}
	}

	
	// Backup case - if smooth grouping could not be created because of some reason, create a default one with all faces of the same smoothing group
	AxSmoothingGroups *smoothGrouping = new AxSmoothingGroups(numPolygons, numVertices);
	for (int polyIndex = 0; polyIndex < numPolygons; polyIndex++)
	{
		smoothGrouping->SetFaceSmoothingGroups(polyIndex, 1);

		int numPolygonVertices = fbxMesh->GetPolygonSize(polyIndex);
		for (int polyVertexIndex = 0; polyVertexIndex < numPolygonVertices; polyVertexIndex++)
		{
			smoothGrouping->AddFaceVertex(polyIndex, fbxMesh->GetPolygonVertex(polyIndex, polyVertexIndex));
		}
	}

	return smoothGrouping;
}

void AxFbxScene::LoadMesh(AxEntitySet *entitySet, FbxMesh *fbxMesh, AxMatrix &geometryTransform)
{
	AxDeviceIndependentMesh *vertexDataMesh= new AxDeviceIndependentMesh();

	// Load vertices' position. Faces will be load at the end, as they might have more than 3 vertices
	this->LoadMesh_Vertices(vertexDataMesh, fbxMesh, geometryTransform);
	
	// Load vertices' texture coordinates
	this->LoadMesh_TextureCoords(vertexDataMesh, fbxMesh);

	// Load vertices bone indices and weights, if any, and setup the transforms of the skeleton. The transforms of the skeleton must be already loaded
	bool isAnimated = this->LoadMesh_Animation(vertexDataMesh, fbxMesh);

	// Load the smoothing groups data
	AxSmoothingGroups *smoothGrouping = this->LoadMesh_CreateSmoothGrouping(fbxMesh);

	// Create a temporary mesh, whith geometry affected by the smoothing groups and calculate its normals.
	// This mesh will later be used to get vertex information for the final meshes, which will additionally be affected by sub materials
	AxDeviceIndependentMesh smoothGroupedMesh;
	// Fill vertices
	AxVector2 v2;
	AxVector3 v3;
	AxVector4 v41, v42;
	smoothGroupedMesh.CreateVertexBuffer(smoothGrouping->vertexIndices.count);
	for (int vertexIndex = 0; vertexIndex < smoothGrouping->vertexIndices.count; vertexIndex++)
	{
		int originalVertexIndex = smoothGrouping->vertexIndices[vertexIndex].index;
						
		vertexDataMesh->GetVertexPosition(originalVertexIndex, v3);
		smoothGroupedMesh.SetVertexPosition(vertexIndex, v3);

		vertexDataMesh->GetVertexNormal(originalVertexIndex, v3);
		smoothGroupedMesh.SetVertexNormal(vertexIndex, v3);

		vertexDataMesh->GetVertexTexCoords(originalVertexIndex, v2);
		smoothGroupedMesh.SetVertexTexCoords(vertexIndex, v2);

		vertexDataMesh->GetVertexBones(originalVertexIndex, v41, v42);
		smoothGroupedMesh.SetVertexBones(vertexIndex, v41, v42);
	}
	int numTriangles = 0;
	for (int faceIndex = 0; faceIndex < smoothGrouping->faces.count; faceIndex++)
		numTriangles += smoothGrouping->faces[faceIndex].indices.count - 2;
	// Now fill the triangles
	smoothGroupedMesh.CreateIndexBuffer(numTriangles * 3);
	int index = 0;
	for (int faceIndex = 0; faceIndex < smoothGrouping->faces.count; faceIndex++)
	{
		int numFaceVertices = smoothGrouping->faces[faceIndex].indices.count;

		// As in the native obj scene
		// In case of polygons with any number of sides, use the first vertex to form a triangle fan with the rest vertices
		int zeroVertexIndex = smoothGrouping->faces[faceIndex].indices[0];
		for (int polyVertexIndex = 2; polyVertexIndex < numFaceVertices; polyVertexIndex++)
		{
			smoothGroupedMesh.SetIndex(index++, zeroVertexIndex);
			smoothGroupedMesh.SetIndex(index++, smoothGrouping->faces[faceIndex].indices[polyVertexIndex]);
			smoothGroupedMesh.SetIndex(index++, smoothGrouping->faces[faceIndex].indices[polyVertexIndex - 1]);
		}
	}
	AxNormalsGenerator(*(AxDeviceMesh*)(&smoothGroupedMesh));
	delete vertexDataMesh;


	// Materials and submaterials. Divide the mesh into sub meshes for the sub materials
	// Materials for a mesh may be mapped onto the whole mesh (FbxLayerElement::EMappingMode::eAllSame) or a number of materials may be mapped to each polygon (FbxLayerElement::EMappingMode::eByPolygon)
	// If it's a whole mesh material, we simply put the material into Axis, followed by the mesh
	// If however, there are multiple materials mapped to different polygons of the mesh, we split the mesh into submeshes, each of the submeshes with its respective material
	int numLayers = fbxMesh->GetLayerCount();
	for (int layerIndex = 0; layerIndex < numLayers; layerIndex++)
	{
		FbxLayer *layer = fbxMesh->GetLayer(layerIndex);

		FbxLayerElementMaterial *layerMaterial = layer->GetMaterials();
		if (layerMaterial != 0)
		{
			FbxLayerElement::EMappingMode mappingMode = layerMaterial->GetMappingMode();
			FbxLayerElement::EReferenceMode referenceMode = layerMaterial->GetReferenceMode();
			
			switch (mappingMode)
			{
				// One material to the whole mesh
				case FbxLayerElement::EMappingMode::eAllSame:
				{
					FbxLayerElementArrayTemplate<int> &polygonMaterialIndices = layerMaterial->GetIndexArray();
					FbxSurfaceMaterial *meshMaterial = fbxMesh->GetNode()->GetMaterial(polygonMaterialIndices.GetAt(0));
					
					// Add the material
					AxMaterial *material = this->AcquireNodeMaterial(meshMaterial);
					entitySet->references.Add(material);

					// Add skinning operation to material, if the mesh is animated
					if (isAnimated)
					{
						material->InsertShadingLayer(AxShadingOperation_Skinning, AxColorOperation_None, 0);
						material->UpdateShader();
					}
					
					// Add the mesh
					AxMesh *mesh = (AxMesh*)entitySet->references.Add(this->context->AddResource(new AxMesh(this->context)));
					mesh->name = this->context->AcquireResourceName(AxString(fbxMesh->GetNode()->GetName()), mesh->resourceType);

					// Copy smoothgrouped mesh
					int numVertices = smoothGroupedMesh.GetVertexCount();
					mesh->deviceMesh->CreateVertexBuffer(numVertices);
					for (int vertexIndex = 0; vertexIndex < numVertices; vertexIndex++)
					{
						smoothGroupedMesh.GetVertexPosition(vertexIndex, v3);
						mesh->deviceMesh->SetVertexPosition(vertexIndex, v3);

						smoothGroupedMesh.GetVertexNormal(vertexIndex, v3);
						mesh->deviceMesh->SetVertexNormal(vertexIndex, v3);

						smoothGroupedMesh.GetVertexTexCoords(vertexIndex, v2);
						mesh->deviceMesh->SetVertexTexCoords(vertexIndex, v2);

						smoothGroupedMesh.GetVertexBones(vertexIndex, v41, v42);
						mesh->deviceMesh->SetVertexBones(vertexIndex, v41, v42);
					}


					// Now fill the triangles
					mesh->deviceMesh->CreateIndexBuffer(smoothGroupedMesh.GetIndexCount());
					int numIndices = smoothGroupedMesh.GetIndexCount();
					for (int index = 0; index < numIndices; index++)
					{
						mesh->deviceMesh->SetIndex(index, smoothGroupedMesh.GetIndex(index));
					}

					// ComputeNormals should not be called, since the normals are computed with smoothGroupedMesh and copied
					mesh->ComputeBounds();

					mesh->deviceMesh->UpdateVertices(0, mesh->deviceMesh->GetVertexCount());
					mesh->deviceMesh->UpdateIndices(0, mesh->deviceMesh->GetIndexCount());

					break;
				}
				
				// Each polygon has some material
				case FbxLayerElement::EMappingMode::eByPolygon:
				{
					// Will hold the mapping of each sub material with its respective polygon indices
					AxDictionary<FbxSurfaceMaterial*, AxList<int>*> subMaterialsPolygonsMapping;

					if ((referenceMode == FbxLayerElementMaterial::eIndex) || (referenceMode == FbxLayerElementMaterial::eIndexToDirect))
					{
						FbxLayerElementArrayTemplate<int> &polygonMaterialIndices = layerMaterial->GetIndexArray();
						int numPolygons = polygonMaterialIndices.GetCount();
						for (int polygonIndex = 0; polygonIndex < numPolygons; polygonIndex++)
						{
							FbxSurfaceMaterial *polygonMaterial = fbxMesh->GetNode()->GetMaterial(polygonMaterialIndices.GetAt(polygonIndex));

							AxList<int> *subMaterialPolygons;
							if (subMaterialsPolygonsMapping.GetValue(polygonMaterial, subMaterialPolygons))
							{
								subMaterialPolygons->Add(polygonIndex);
							}
							else
							{
								subMaterialPolygons = new AxList<int>();
								subMaterialPolygons->Add(polygonIndex);
								subMaterialsPolygonsMapping.Add(polygonMaterial, subMaterialPolygons);
							}
						}
					}



					// Create the submeshes and free the materialPolygons dictionary
					AxSubMesh subMesh(smoothGrouping->vertexIndices.count);
					void* node = subMaterialsPolygonsMapping.GetNodeNext(0);
					while (node != 0)
					{
						FbxSurfaceMaterial *subMaterial = subMaterialsPolygonsMapping.GetNodeValue(node).key;
						AxList<int> *subMaterialPolygons = subMaterialsPolygonsMapping.GetNodeValue(node).value;
					
						// Add the material
						AxMaterial *material = this->AcquireNodeMaterial(subMaterial);
						entitySet->references.Add(material);

						// Add skinning operation to material, if the mesh is animated
						if (isAnimated)
						{
							material->InsertShadingLayer(AxShadingOperation_Skinning, AxColorOperation_None, 0);
							material->UpdateShader();
						}

						// Add the mesh which will hold the polygons of the material
						AxMesh *mesh = (AxMesh*)entitySet->references.Add(this->context->AddResource(new AxMesh(this->context)));
						mesh->name = this->context->AcquireResourceName(AxString(fbxMesh->GetNode()->GetName()) + " " + material->name, mesh->resourceType);

						// Generate the polygons and vertices as a submesh of the whole geometry
						// subMesh, as well as the original fbx mesh may have polygons with more then 3 vertices, so we'll count the number of triangles the submesh will have
						int numSubMaterialTriangles = 0;
						for (int subMaterialPolygonIndex = 0; subMaterialPolygonIndex < subMaterialPolygons->count; subMaterialPolygonIndex++)
						{
							// Get the submaterial's polygon index in the original fbx mesh. This index also matches the index of the polygon in the smooth grouping
							int polygonIndex = subMaterialPolygons->Get(subMaterialPolygonIndex);

							subMesh.AddFace();
							const int numPolygonVertices = smoothGrouping->faces[polygonIndex].indices.count;
							for (int polyVertexIndex = 0; polyVertexIndex < numPolygonVertices; polyVertexIndex++)
							{
								// As a vace vertex index of the sub mesh, add in index to a vertex of the smooth grouping
								int vertexIndex = smoothGrouping->faces[polygonIndex].indices[polyVertexIndex];
								subMesh.AddFaceVertex(subMaterialPolygonIndex, vertexIndex);
							}
							numSubMaterialTriangles += numPolygonVertices - 2;
						}


						// Fill vertices
						mesh->deviceMesh->CreateVertexBuffer(subMesh.vertexIndices.count);
						for (int vertexIndex = 0; vertexIndex < subMesh.vertexIndices.count; vertexIndex++)
						{
							int originalVertexIndex = subMesh.vertexIndices[vertexIndex];
						
							smoothGroupedMesh.GetVertexPosition(originalVertexIndex, v3);
							mesh->deviceMesh->SetVertexPosition(vertexIndex, v3);

							smoothGroupedMesh.GetVertexNormal(originalVertexIndex, v3);
							mesh->deviceMesh->SetVertexNormal(vertexIndex, v3);

							smoothGroupedMesh.GetVertexTexCoords(originalVertexIndex, v2);
							mesh->deviceMesh->SetVertexTexCoords(vertexIndex, v2);

							smoothGroupedMesh.GetVertexBones(originalVertexIndex, v41, v42);
							mesh->deviceMesh->SetVertexBones(vertexIndex, v41, v42);
						}


						// Now fill the triangles
						mesh->deviceMesh->CreateIndexBuffer(numSubMaterialTriangles * 3);
						int index = 0;
						for (int faceIndex = 0; faceIndex < subMesh.faces.count; faceIndex++)
						{
							int numFaceVertices = subMesh.faces[faceIndex]->indices.count;

							// As in the native obj scene
							// In case of polygons with any number of sides, use the first vertex to form a triangle fan with the rest vertices
							int zeroVertexIndex = subMesh.faces[faceIndex]->indices[0];
							for (int polyVertexIndex = 2; polyVertexIndex < numFaceVertices; polyVertexIndex++)
							{
								mesh->deviceMesh->SetIndex(index++, zeroVertexIndex);
								mesh->deviceMesh->SetIndex(index++, subMesh.faces[faceIndex]->indices[polyVertexIndex]);
								mesh->deviceMesh->SetIndex(index++, subMesh.faces[faceIndex]->indices[polyVertexIndex - 1]);
							}

						}

						// ComputeNormals should not be called, since the normals are computed with smoothGroupedMesh and copied
						mesh->ComputeBounds();

						mesh->deviceMesh->UpdateVertices(0, mesh->deviceMesh->GetVertexCount());
						mesh->deviceMesh->UpdateIndices(0, mesh->deviceMesh->GetIndexCount());

						// Reset the submesh to make it ready for another go
						subMesh.Reset();
					
						delete subMaterialPolygons;
						node = subMaterialsPolygonsMapping.GetNodeNext(node);
					}

					break;
				}
			}
		}
	}

	if (smoothGrouping != 0)
		delete smoothGrouping;
}



void AxFbxScene::LoadNode(AxEntitySet *entitySet, FbxNode* fbxNode, FbxNodeAttribute::EType loadType)
{
	if (loadType == FbxNodeAttribute::eSkeleton)
	{
	entitySet = (AxEntitySet*)entitySet->references.Add(this->context->AddResource(new AxEntitySet()));
	entitySet->name = this->context->AcquireResourceName(AxString(fbxNode->GetName()), entitySet->resourceType);

	// Transform
	AxTransform *transform = (AxTransform*)entitySet->references.Add(this->context->AddResource(new AxTransform()));
	transform->name = this->context->AcquireResourceName(AxString(fbxNode->GetName()), transform->resourceType);
	if (!this->LoadNodeKeyframeAnimation(entitySet, fbxNode, transform))
		this->LoadNodeDefaultTransform(transform, fbxNode);

	this->resourceMap.Add(fbxNode, transform);
	}

	AxString name = AxString(fbxNode->GetName());

	int numAttributes = fbxNode->GetNodeAttributeCount();
	for (int i = 0; i < numAttributes; i++)
	{
		FbxNodeAttribute *nodeAttributeFbx = fbxNode->GetNodeAttributeByIndex(i);

		AxString attributeName = AxString(nodeAttributeFbx->GetName());

		FbxNodeAttribute::EType attributeType = nodeAttributeFbx->GetAttributeType();

		if (attributeType == loadType)
		{
			switch (attributeType)
			{
				case FbxNodeAttribute::eCamera:
				{
					FbxCamera *fbxCamera = (FbxCamera*)nodeAttributeFbx;

					// Transform
					AxTransform *transform = (AxTransform*)entitySet->references.Add(this->context->AddResource(new AxTransform()));
					transform->name = this->context->AcquireResourceName(AxString(fbxNode->GetName()), transform->resourceType);
					//AxMatrix::CreateOrientedPosition(transform->manual, this->FbxToAxVector3(fbxCamera->Position.Get()), this->FbxToAxVector3(fbxCamera->InterestPosition.Get()), AxVector3(0.0f, 0.0f, 1.0f)); // Get both position and orientation
					AxVector3 position = this->FbxToAxVector3(fbxCamera->Position.Get());
					AxVector3 target = this->FbxToAxVector3(fbxCamera->InterestPosition.Get());
					AxVector3 orientationVector;
					AxVector3::Subtract(orientationVector, target, position);
					AxMatrix::CreateOrientedPosition(transform->transform, position, target, AxVector3(0.0f, 0.0f, 1.0f)); // Get only orientation from position and target


					AxCamera *camera = (AxCamera*)entitySet->references.Add(this->context->AddResource(new AxCamera(this->context)));
					camera->name = this->context->AcquireResourceName(AxString(fbxNode->GetName()), AxResourceType_Camera);

					camera->properties[AxCamera::propertyIndex_FOV]->SetFloat((float)fbxCamera->FieldOfView * AxMath::Pi / 180.0f);
					
					break;
				}

				case FbxNodeAttribute::eLight:
				{
					FbxLight *fbxLight = (FbxLight*)nodeAttributeFbx;

					// Transform
					AxTransform *transform = (AxTransform*)entitySet->references.Add(this->context->AddResource(new AxTransform()));
					transform->name = this->context->AcquireResourceName(AxString(fbxNode->GetName()), transform->resourceType);
					if (!this->LoadNodeKeyframeAnimation(entitySet, fbxNode, transform))
						this->LoadNodeDefaultTransform(transform, fbxNode);

					AxLight *light = (AxLight*)entitySet->references.Add(this->context->AddResource(new AxLight()));
					light->name = this->context->AcquireResourceName(AxString(fbxNode->GetName()), AxResourceType_Light);

					light->properties[AxLight::propertyIndex_Color]->SetColor(AxVector4(this->FbxToAxVector3(fbxLight->Color.Get()), 1.0f));
					light->properties[AxLight::propertyIndex_Intensity]->SetFloat((float)fbxLight->FarAttenuationEnd.Get());
					
					break;
				}

				case FbxNodeAttribute::eMesh:
				{
					entitySet = (AxEntitySet*)entitySet->references.Add(this->context->AddResource(new AxEntitySet()));
					entitySet->name = this->context->AcquireResourceName(AxString(fbxNode->GetName()), entitySet->resourceType);

					// Transform
					AxTransform *transform = (AxTransform*)entitySet->references.Add(this->context->AddResource(new AxTransform()));
					transform->name = this->context->AcquireResourceName(AxString(fbxNode->GetName()), AxResourceType_Transform);
					if (!this->LoadNodeKeyframeAnimation(entitySet, fbxNode, transform))
						this->LoadNodeDefaultTransform(transform, fbxNode);

					// Geometric transform required to be applied on vertex positions
					AxMatrix geometryTransform;
					this->GetNodeGeometryTransform(geometryTransform, fbxNode);

					// Load mesh data
					this->LoadMesh(entitySet, (FbxMesh*)nodeAttributeFbx, geometryTransform);

					break;
				}

				case FbxNodeAttribute::eSkeleton:
				{
					//FbxSkeleton *fbxSkeleton = (FbxSkeleton*)nodeAttributeFbx;

					//FbxSkeleton::EType skeletonType = fbxSkeleton->GetSkeletonType();


					break;
				}
			}
		}
	}

	int numChildren = fbxNode->GetChildCount();
	for(int i = 0; i < numChildren; i++)
	{
		this->LoadNode(entitySet, fbxNode->GetChild(i), loadType);
	}
}

bool AxFbxScene::Load(AxStream &source)
{
	AxString fileName = this->context->serializationParameters.fullSourceName;
	
    // Initialize the SDK manager. This object handles all our memory management.
    this->fbxManager = FbxManager::Create();
    
    // Create the IO settings object.
    FbxIOSettings *ioSettings = FbxIOSettings::Create(this->fbxManager, IOSROOT);
    this->fbxManager->SetIOSettings(ioSettings);

    // Create an importer using the SDK manager.
    FbxImporter* importer = FbxImporter::Create(this->fbxManager, "AxFbxManager");
    
    // Use the first argument as the filename for the importer.
	if (!importer->Initialize(fileName.GetCharContents(), -1, this->fbxManager->GetIOSettings()))
	{
		this->fbxManager->Destroy();
		return false;
	}

    // Create a new scene so that it can be populated by the imported file.
    this->fbxScene = FbxScene::Create(this->fbxManager, "AxFbxScene");

    // Import the contents of the file into the scene.
	if (!importer->Import(this->fbxScene))
	{
	    this->fbxManager->Destroy();
		return false;
	}

	// The file is imported; so get rid of the importer.
    importer->Destroy();


	AxEntitySet *parent = (AxEntitySet*)this->context->root;
	
	AxEntitySet *importSet = (AxEntitySet*)parent->references.Add(this->context->AddResource(new AxEntitySet()));
	importSet->name = this->context->AcquireResourceName(this->context->serializationParameters.friendlyName, importSet->resourceType);

	FbxNode* fbxRootNode = this->fbxScene->GetRootNode();
    if (fbxRootNode != 0)
	{
		// Add an animation frame mechanism
		this->animationFrame = (AxLinearMechanism*)this->context->AddResource(new AxLinearMechanism(0.0f, 0.0f, 1.0f, false, false, false));
		this->animationFrame->name = this->context->AcquireResourceName(importSet->name + " frame", AxResourceType_Mechanism);
		importSet->references.Add(this->animationFrame);

		// Cameras first
		this->LoadNode(importSet, fbxRootNode, FbxNodeAttribute::eCamera);

		// Lights
		this->LoadNode(importSet, fbxRootNode, FbxNodeAttribute::eLight);

		// Add a transform to act as a conversion between fbx and ax coord systems, as well as a glabal transform for the whole fbx content
		AxTransform *transform = (AxTransform*)importSet->references.Add(this->context->AddResource(new AxTransform()));
		transform->name = this->context->AcquireResourceName(importSet->name, transform->resourceType);
		AxMatrix::CreateRotationX(transform->transform, -AxMath::PiHalf);

		this->LoadNode(importSet, fbxRootNode, FbxNodeAttribute::eSkeleton);
		this->LoadNode(importSet, fbxRootNode, FbxNodeAttribute::eMesh);
    }


    // Destroy the SDK manager and all the other objects it was handling.
	this->fbxManager->Destroy();

	return true;
}

bool AxFbxScene::Save(AxStream &destination, const char *format)
{
	return false;
}
