#pragma once

class Vector2
{
public:
	Vector2();
	Vector2(float x, float y);

	// Vec2 = Vec2 + Vec2
	Vector2 operator+ (const Vector2& rval) const
	{
		return { x + rval.x, y + rval.y };
	}

	// Vec2 = Vec2 - Vec2
	Vector2 operator- (const Vector2& rval) const
	{
		return { x - rval.x, y - rval.y };
	}

	// Vec2 = (Vec2 += Vec2)
	void operator+= (const Vector2& rval);

	// Vec2 = (Vec2 -= Vec2)
	void operator-= (const Vector2& rval);

	// Vec2 = Vec2 * float
	Vector2 operator*(float scale)const;

	// Vec2 = Vec2 / floats
	Vector2 operator/(float div)const;

	// Vec2 = (Vec2 *= float)
	void operator*=(float scale);

	// Vec2 = (Vec2 /= float)
	void operator/=(float div);

	// ベクター逆転(-)
	Vector2 operator-() const;

	/// ベクトルの大きさを返す
	float Length() const;

	// ベクトルの大きさの2乗を返す
	float SQLength() const;

	// ベクトルの正規化
	void Normalize();

	// 正規化したベクトルを返す
	Vector2 Normalized() const;

public:
	float x, y;
};