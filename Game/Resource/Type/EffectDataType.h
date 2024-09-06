#pragma once
#include <string>

namespace Resource::Type
{
	// エフェクトのパラメータの種類
	enum class Effect
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
	static const std::string EffectKey[static_cast<int>(Effect::NUM)] =
	{
		"scaleX",
		"scaleY",
		"scaleZ",
		"rotationX",
		"rotationY",
		"rotationZ"
	};
}