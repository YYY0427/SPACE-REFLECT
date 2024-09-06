#pragma once
#include <string>

namespace Resource::Type
{
	// �G�t�F�N�g�̃p�����[�^�̎��
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

	// �G�t�F�N�g�̃p�����[�^�̃L�[
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