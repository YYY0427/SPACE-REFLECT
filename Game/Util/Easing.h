#pragma once

/// <summary>
/// イージング関数をまとめたクラス
/// https://easings.net/ja
/// </summary>
class Easing
{
public:
	/// <param name="t">現在の時間</param>
	/// <param name="b">初期値</param>
	/// <param name="c">変化量</param>
	/// <param name="d">総時間</param>
	static float EaseIn(float t, float b, float c, float d);
	static float EaseOut(float t, float b, float c, float d);
	static float EaseInOut(float t, float b, float c, float d);
	static float EaseInCubic(float t, float b, float c, float d);
	static float EaseOutCubic(float t, float b, float c, float d);
	static float EaseInOutCubic(float t, float b, float c, float d);
	static float EaseInQuint(float t, float b, float c, float d);
	static float EaseOutQuint(float t, float b, float c, float d);
	static float EaseInOutQuint(float t, float b, float c, float d);
	static float EaseInCirc(float t, float b, float c, float d);
	static float EaseOutCirc(float t, float b, float c, float d);
	static float EaseInOutCirc(float t, float b, float c, float d);
	static float EaseInBack(float t, float b, float c, float d);
	static float EaseOutBack(float t, float b, float c, float d);
	static float EaseInOutBack(float t, float b, float c, float d);
	static float EaseInElastic(float t, float b, float c, float d);
	static float EaseOutElastic(float t, float b, float c, float d);
	static float EaseInOutElastic(float t, float b, float c, float d);
	static float EaseInBounce(float t, float b, float c, float d);
	static float EaseOutBounce(float t, float b, float c, float d);
	static float EaseInOutBounce(float t, float b, float c, float d);
	static float EaseInQuad(float t, float b, float c, float d);
	static float EaseOutQuad(float t, float b, float c, float d);
	static float EaseInOutQuad(float t, float b, float c, float d);
	static float EaseInQuart(float t, float b, float c, float d);
	static float EaseOutQuart(float t, float b, float c, float d);
	static float EaseInOutQuart(float t, float b, float c, float d);
	static float EaseInSine(float t, float b, float c, float d);
	static float EaseOutSine(float t, float b, float c, float d);
	static float EaseInOutSine(float t, float b, float c, float d);
	static float EaseInExpo(float t, float b, float c, float d);
	static float EaseOutExpo(float t, float b, float c, float d);
	static float EaseInOutExpo(float t, float b, float c, float d);

	// 以下は自作
	static float EaseOutInExpo(float t, float b, float c1, float c2, float d);
};