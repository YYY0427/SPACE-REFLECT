#pragma once

/// <summary>
/// イージング関数をまとめたクラス
/// https://easings.net/ja
/// </summary>
class Easing
{
public:
	/// <summary>
	/// イージング関数
	/// </summary>
	/// <param name="time">現在の時間</param>
	/// <param name="totalTime">総時間</param>
	/// <param name="max">終了値</param>
	/// <param name="min">開始値</param>
	/// <returns></returns>
	static float EaseOutCubic(float time, float totalTime, float max, float min);
};
