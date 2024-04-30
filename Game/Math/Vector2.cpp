#include "Vector2.h"
#include <cmath>

// コンストラクタ
Vector2::Vector2() :
	x(0.0f),
	y(0.0f)
{
}

// コンストラクタ
Vector2::Vector2(const float x, const float y):
	x(x),
	y(y)
{
}

// Vec2 = (Vec2 += Vec2)
void Vector2::operator+=(const Vector2& rval)
{
	x += rval.x;
	y += rval.y;
}

// Vec2 = (Vec2 -= Vec2)
void Vector2::operator-=(const Vector2& rval)
{
	x -= rval.x;
	y -= rval.y;
}

// Vec2 = Vec2 * float
Vector2 Vector2::operator*(const float scale) const
{
	return { x * scale, y * scale };
}

// Vec2 = Vec2 / float
Vector2 Vector2::operator/(const float div) const
{
	return { x / div, y / div };
}

// Vec2 = (Vec2 *= float)
void Vector2::operator*=(const float scale)
{
	x *= scale;
	y *= scale;
}

// Vec2 = (Vec2 /= float)
void Vector2::operator/=(const float div)
{
	x /= div;
	y /= div;
}

// Vec2 == Vec2
bool Vector2::operator==(const Vector2& rval) const
{
	return x == rval.x && y == rval.y;
}

// Vec2 != Vec2
bool Vector2::operator!=(const Vector2& rval) const
{
	return x != rval.x || y != rval.y;
}

// Vec2 = -Vec2
Vector2 Vector2::operator-() const
{
	return { -x, -y };
}

/// ベクトルの大きさを返す
float Vector2::Length() const
{
	return hypotf(x, y);
}

// ベクトルの大きさの2乗を返す
float Vector2::SQLength() const
{
	return x * x + y * y;
}

// ベクトルの正規化
void Vector2::Normalize()
{
	auto len = Length();
	x /= len;
	y /= len;
}

// 正規化したベクトルを返す
Vector2 Vector2::Normalized() const
{
	auto len = Length();
	return { x / len, y / len };
}