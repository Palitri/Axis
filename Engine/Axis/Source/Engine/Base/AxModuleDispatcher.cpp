//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxModuleDispatcher.h"

AxModuleDispatcher::AxModuleDispatcher(void)
{
}


AxModuleDispatcher::~AxModuleDispatcher(void)
{
}

bool AxModuleDispatcher::IsFeatureSupported(unsigned int featureInfoId, AxString &featureName, bool ignoreCase)
{
	int testedFeatureTagIndex = 0;
	AxString testedFeatureName;
	AxString featureNameNormalized = ignoreCase ? featureName.ToLower() : AxString(featureName);
	while (this->GetInfo(featureInfoId, testedFeatureTagIndex, testedFeatureName))
		if (featureNameNormalized.Equals(ignoreCase ? testedFeatureName.ToLower() : AxString(testedFeatureName)))
			return true;
		else
			testedFeatureTagIndex++;

	return false;
}
