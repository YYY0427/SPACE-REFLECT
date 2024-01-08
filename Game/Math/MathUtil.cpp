#include "MathUtil.h"
#include "DxLib.h"

// ラジアンから度に変換
float MathUtil::ToDegree(float radian)
{
	return radian * 180.0f / DX_PI_F;
}

// 度からラジアンに変換
float MathUtil::ToRadian(float degree)
{
	return degree * DX_PI_F / 180.0f;
}