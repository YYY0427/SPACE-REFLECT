#pragma once
#include <cmath>
#include "Vector2.h"
#include "Vector3.h"

/// <summary>
/// ���w�֘A�̃��[�e�B���e�B�N���X
/// </summary>
namespace Math::Util
{
	/// <summary>
	/// ���W�A������x�ɕϊ�
	/// </summary>
	/// <param name="radian">���W�A��</param>
	/// <returns>�x</returns>
	float ToDegree(const float radian);

	/// <summary>
	/// �x���烉�W�A���ɕϊ�
	/// </summary>
	/// <param name="degree">�x</param>
	/// <returns>���W�A��</returns>
	float ToRadian(const float degree);

	/// <summary>
	/// �����̃����_���Ȓl���擾
	/// </summary>
	/// <param name="min">�ŏ��l</param>
	/// <param name="max">�ő�l</param>
	/// <returns>�����̃����_���Ȓl</returns>
	int GetRandInt(const int min, const int max);

	/// <summary>
	/// �����̃����_���Ȓl���擾
	/// </summary>
	/// <param name="min">�ŏ��l</param>
	/// <param name="max">�ő�l</param>
	/// <returns>�����̃����_���Ȓl</returns>
	float GetRandFloat(const float min, const float max);

	/// <summary>
	/// ���`���
	/// </summary>
	/// <param name="start">�J�n�l</param>
	/// <param name="end">�I���l</param>
	/// <param name="value">��Ԓl</param>
	/// <returns>��Ԍ���</returns>
	float Lerp(const float start, const float end, const float value);
}