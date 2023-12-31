#pragma once
#include <cmath>
#include "Vector2.h"
#include "Vector3.h"

///<summary>
///数学関連のユーティリティクラス
/// </summary>
namespace MathUtil
{
	/// <summary>
	/// ラジアンから度に変換
	/// </summary>
	/// <param name="radian">ラジアン</param>
	/// <returns>度</returns>
	float ToDegree(float radian);

	/// <summary>
	/// 度からラジアンに変換
	/// </summary>
	/// <param name="degree">度</param>
	/// <returns>ラジアン</returns>
	float ToRadian(float degree);
}