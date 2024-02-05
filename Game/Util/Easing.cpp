#include "Easing.h"
#include <DxLib.h>	
#include <cmath>

float Easing::EaseOutCubic(float time, float totalTime, float max, float min)
{
	max -= min;
	time = time / totalTime - 1;
	return max * (time * time * time + 1) + min;
}