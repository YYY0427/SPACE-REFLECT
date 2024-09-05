#include "Vector2.h"
#include <cmath>

namespace Math
{
	// �R���X�g���N�^
	Vector2::Vector2() :
		x(0.0f),
		y(0.0f)
	{
	}

	// �R���X�g���N�^
	Vector2::Vector2(const float x, const float y) :
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

	/// �x�N�g���̑傫����Ԃ�
	float Vector2::Length() const
	{
		return hypotf(x, y);
	}

	// �x�N�g���̑傫����2���Ԃ�
	float Vector2::SQLength() const
	{
		return x * x + y * y;
	}

	// �x�N�g���̐��K��
	void Vector2::Normalize()
	{
		auto len = Length();
		x /= len;
		y /= len;
	}

	// ���K�������x�N�g����Ԃ�
	Vector2 Vector2::Normalized() const
	{
		auto len = Length();
		return { x / len, y / len };
	}
}