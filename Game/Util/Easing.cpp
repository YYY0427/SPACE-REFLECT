#include "Easing.h"
#include <DxLib.h>	
#include <cmath>

float Easing::EaseIn(float t, float b, float c, float d)
{
	return c * (t /= d) * t + b;
}

float Easing::EaseOut(float t, float b, float c, float d)
{
	return -c * (t /= d) * (t - 2) + b;
}

float Easing::EaseInOut(float t, float b, float c, float d)
{
	if ((t /= d / 2) < 1) return c / 2 * t * t + b;
	return -c / 2 * ((--t) * (t - 2) - 1) + b;
	/*t /= d / 2.0;
	if (t < 1)
		return c / 2.0 * t * t + b;

	t = t - 1;
	return -c / 2.0 * (t * (t - 2) - 1) + b;*/
}

float Easing::EaseInCubic(float t, float b, float c, float d)
{
	return c * (t /= d) * t * t + b;
}

float Easing::EaseOutCubic(float t, float b, float c, float d)
{
	return c * ((t = t / d - 1) * t * t + 1) + b;
}

float Easing::EaseInOutCubic(float t, float b, float c, float d)
{
	if ((t /= d / 2) < 1) return c / 2 * t * t * t + b;
	return c / 2 * ((t -= 2) * t * t + 2) + b;
}

float Easing::EaseInQuint(float t, float b, float c, float d)
{
	return c * (t /= d) * t * t * t * t + b;
}

float Easing::EaseOutQuint(float t, float b, float c, float d)
{
	return c * ((t = t / d - 1) * t * t * t * t + 1) + b;
}

float Easing::EaseInOutQuint(float t, float b, float c, float d)
{
	if ((t /= d / 2) < 1) return c / 2 * t * t * t * t * t + b;
	return c / 2 * ((t -= 2) * t * t * t * t + 2) + b;
}

float Easing::EaseInCirc(float t, float b, float c, float d)
{
	return -c * (sqrt(1 - (t /= d) * t) - 1) + b;
}

float Easing::EaseOutCirc(float t, float b, float c, float d)
{
	return c * sqrt(1 - (t = t / d - 1) * t) + b;
}

float Easing::EaseInOutCirc(float t, float b, float c, float d)
{
	if ((t /= d / 2) < 1) return -c / 2 * (sqrt(1 - t * t) - 1) + b;
	return c / 2 * (sqrt(1 - (t -= 2) * t) + 1) + b;
}

float Easing::EaseInBack(float t, float b, float c, float d)
{
	float s = 1.70158f;
	return c * (t /= d) * t * ((s + 1) * t - s) + b;
}

float Easing::EaseOutBack(float t, float b, float c, float d)
{
	float s = 1.70158f;
	return c * ((t = t / d - 1) * t * ((s + 1) * t + s) + 1) + b;
}

float Easing::EaseInOutBack(float t, float b, float c, float d)
{
	float s = 1.70158f;
	if ((t /= d / 2) < 1) return c / 2 * (t * t * (((s *= (1.525f)) + 1) * t - s)) + b;
	return c / 2 * ((t -= 2) * t * (((s *= (1.525f)) + 1) * t + s) + 2) + b;
}

float Easing::EaseInElastic(float t, float b, float c, float d)
{
	if (t == 0) return b;  if ((t /= d) == 1) return b + c;
	float p = d * .3f;
	float a = c;
	float s = p / 4;
	float postFix = a * pow(2, 10 * (t -= 1));
	return -(postFix * sin((t * d - s) * (2 * DX_PI_F) / p)) + b;
}

float Easing::EaseOutElastic(float t, float b, float c, float d)
{
	if (t == 0) return b;  if ((t /= d) == 1) return b + c;
	float p = d * .3f;
	float a = c;
	float s = p / 4;
	return (a * pow(2, -10 * t) * sin((t * d - s) * (2 * DX_PI_F) / p) + c + b);
}

float Easing::EaseInOutElastic(float t, float b, float c, float d)
{
	if (t == 0) return b;  if ((t /= d / 2) == 2) return b + c;
	float p = d * (.3f * 1.5f);
	float a = c;
	float s = p / 4;
	if (t < 1) {
		float postFix = a * pow(2, 10 * (t -= 1));
		return -.5f * (postFix * sin((t * d - s) * (2 * DX_PI_F) / p)) + b;
	}
	float postFix = a * pow(2, -10 * (t -= 1));
	return postFix * sin((t * d - s) * (2 * DX_PI_F) / p) * .5f + c + b;
}

float Easing::EaseInBounce(float t, float b, float c, float d)
{
	return c - EaseOutBounce(d - t, 0, c, d) + b;
}

float Easing::EaseOutBounce(float t, float b, float c, float d)
{
	if ((t /= d) < (1 / 2.75f)) {
		return c * (7.5625f * t * t) + b;
	}
	else if (t < (2 / 2.75f)) {
		float postFix = t -= (1.5f / 2.75f);
		return c * (7.5625f * (postFix)* t + .75f) + b;
	}
	else if (t < (2.5 / 2.75)) {
		float postFix = t -= (2.25f / 2.75f);
		return c * (7.5625f * (postFix)* t + .9375f) + b;
	}
	else {
		float postFix = t -= (2.625f / 2.75f);
		return c * (7.5625f * (postFix)* t + .984375f) + b;
	}
}

float Easing::EaseInOutBounce(float t, float b, float c, float d)
{
	if (t < d / 2) return EaseInBounce(t * 2, 0, c, d) * .5f + b;
	else return EaseOutBounce(t * 2 - d, 0, c, d) * .5f + c * .5f + b;
}

float Easing::EaseInQuad(float t, float b, float c, float d)
{
	return c * (t /= d) * t + b;
}

float Easing::EaseOutQuad(float t, float b, float c, float d)
{
	return -c * (t /= d) * (t - 2) + b;
}

float Easing::EaseInOutQuad(float t, float b, float c, float d)
{
	if ((t /= d / 2) < 1) return ((c / 2) * (t * t)) + b;
	return -c / 2 * (((t - 2) * (--t)) - 1) + b;
}

float Easing::EaseInQuart(float t, float b, float c, float d)
{
	return c * (t /= d) * t * t * t + b;
}

float Easing::EaseOutQuart(float t, float b, float c, float d)
{
	return -c * ((t = t / d - 1) * t * t * t - 1) + b;
}

float Easing::EaseInOutQuart(float t, float b, float c, float d)
{
	if ((t /= d / 2) < 1) return c / 2 * t * t * t * t + b;
	return -c / 2 * ((t -= 2) * t * t * t - 2) + b;
}

float Easing::EaseInSine(float t, float b, float c, float d)
{
	return -c * cos(t / d * (DX_PI_F / 2)) + c + b;
}

float Easing::EaseOutSine(float t, float b, float c, float d)
{
	return c * sin(t / d * (DX_PI_F / 2)) + b;
}

float Easing::EaseInOutSine(float t, float b, float c, float d)
{
	return -c / 2 * (cos(DX_PI_F * t / d) - 1) + b;
}

float Easing::EaseInExpo(float t, float b, float c, float d)
{
	return (t == 0) ? b : c * pow(2, 10 * (t / d - 1)) + b;
}

float Easing::EaseOutExpo(float t, float b, float c, float d)
{
	return (t == d) ? b + c : c * (-pow(2, -10 * t / d) + 1) + b;
}

float Easing::EaseInOutExpo(float t, float b, float c, float d)
{
	if (t == 0) return b;
	if (t == d) return b + c;
	if ((t /= d / 2) < 1) return c / 2 * pow(2, 10 * (t - 1)) + b;
	return c / 2 * (-pow(2, -10 * --t) + 2) + b;
}

float Easing::EaseOutInExpo(float t, float b, float c1, float c2, float d)
{
	if(t < d / 2)
		return EaseOutExpo(t, b, c1, d / 2);
	else
		t -= d / 2;
		return EaseInExpo(t, c1, c2, d / 2);
}

