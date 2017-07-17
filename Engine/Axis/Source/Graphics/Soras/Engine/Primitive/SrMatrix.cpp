//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "SrMatrix.h"

void SrMatrix::Identity(SrMatrix &result)
{
	result._11 = 1.0f;
	result._12 = 0.0f;
	result._13 = 0.0f;
	result._14 = 0.0f;
	result._21 = 0.0f;
	result._22 = 1.0f;
	result._23 = 0.0f;
	result._24 = 0.0f;
	result._31 = 0.0f;
	result._32 = 0.0f;
	result._33 = 1.0f;
	result._34 = 0.0f;
	result._41 = 0.0f;
	result._42 = 0.0f;
	result._43 = 0.0f;
	result._44 = 1.0f;
};

void SrMatrix::InvertTranspose3x3(SrMatrix &result, SrMatrix &source)
{
    float det = source._12 * source._23 * source._31 +
                source._13 * source._21 * source._32 -
                source._11 * source._23 * source._32 -
                source._12 * source._21 * source._33 -
                source._13 * source._22 * source._31;

    if (det == 0.0f)
        return;

    det = 1.0f / det;

    float _11 = det * (source._22 * source._33 - source._23 * source._32);
    float _12 = det * (source._13 * source._32 - source._12 * source._33);
    float _13 = det * (source._12 * source._23 - source._13 * source._22);
    float _21 = det * (source._23 * source._31 - source._21 * source._33);
    float _22 = det * (source._11 * source._33 - source._13 * source._31);
    float _23 = det * (source._13 * source._21 - source._11 * source._23);
    float _31 = det * (source._21 * source._32 - source._22 * source._31);
    float _32 = det * (source._12 * source._31 - source._11 * source._32);
    float _33 = det * (source._11 * source._22 - source._12 * source._21);

	result._11 = _11;
	result._12 = _21;
	result._13 = _31;
	result._21 = _12;
	result._22 = _22;
	result._23 = _32;
	result._31 = _13;
	result._32 = _23;
	result._33 = _33;
};