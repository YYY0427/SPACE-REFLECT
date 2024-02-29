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

	/// <summary>
	/// 整数のランダムな値を取得
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns>整数のランダムな値</returns>
	int GetRandInt(const int min, const int max);

	/// <summary>
	/// 少数のランダムな値を取得
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns>少数のランダムな値</returns>
	float GetRandFloat(const float min, const float max);
}