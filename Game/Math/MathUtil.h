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
	float ToDegree(const float radian);

	/// <summary>
	/// 度からラジアンに変換
	/// </summary>
	/// <param name="degree">度</param>
	/// <returns>ラジアン</returns>
	float ToRadian(const float degree);

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

	/// <summary>
	/// 線形補間
	/// </summary>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <param name="value">補間値</param>
	/// <returns>補間結果</returns>
	float Lerp(const float start, const float end, const float value);
}