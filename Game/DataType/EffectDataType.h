#pragma once
#include <string>

namespace DataType
{
	// エフェクトのパラメータの種類
	enum class EffectType
	{
		SCALE_X,
		SCALE_Y,
		SCALE_Z,
		ROTATION_X,
		ROTATION_Y,
		ROTATION_Z,
		NUM
	};

	// エフェクトのパラメータのキー
	static const std::string EffectKey[static_cast<int>(EffectType::NUM)] =
	{
		"scaleX",
		"scaleY",
		"scaleZ",
		"rotationX",
		"rotationY",
		"rotationZ"
	};
}