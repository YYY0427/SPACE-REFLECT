#include "MathUtil.h"
#include <DxLib.h>

namespace Math::Util
{
	// ラジアンから度に変換
	float Math::Util::ToDegree(const float radian)
	{
		return radian * 180.0f / DX_PI_F;
	}

	// 度からラジアンに変換
	float Math::Util::ToRadian(const float degree)
	{
		return degree * DX_PI_F / 180.0f;
	}

	// 整数のランダムな値を取得
	int Math::Util::GetRandInt(const int min, const int max)
	{
		return GetRand(max - min) + min;
	}

	// 少数のランダムな値を取得
	float Math::Util::GetRandFloat(const float min, const float max)
	{
		int   randInt = GetRand(static_cast<int>(max * 1000) - static_cast<int>(min * 1000));
		float randFloat = static_cast<float>(randInt + static_cast<int>(min * 1000)) / 1000.0f;
		return randFloat;
	}

	// 線形補間
	float Math::Util::Lerp(const float start, const float end, const float value)
	{
		return start + (end - start) * value;
	}

}