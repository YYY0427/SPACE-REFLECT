#pragma once

namespace Math
{
	/// <summary>
	/// Vector2
	/// </summary>
	class Vector2
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		Vector2();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="x">x�̒l</param>
		/// <param name="y">y�̒l</param>
		Vector2(const float x, const float y);

		/// <summary>
		/// Vec2 = Vec2 + Vec2 
		/// </summary>
		/// <param name="rval">�E��</param>
		/// <returns>�v�Z����</returns>
		Vector2 operator+ (const Vector2& rval) const
		{
			return { x + rval.x, y + rval.y };
		}

		/// <summary>
		/// Vec2 = Vec2 - Vec2 
		/// </summary>
		/// <param name="rval">�E��</param>
		/// <returns>�v�Z����</returns>
		Vector2 operator- (const Vector2& rval) const
		{
			return { x - rval.x, y - rval.y };
		}

		/// <summary>
		/// Vec2 = (Vec2 += Vec2) 
		/// </summary>
		/// <param name="rval">�E��</param>
		void operator+= (const Vector2& rval);

		/// <summary>
		/// Vec2 = (Vec2 += float)
		/// </summary>
		/// <param name="rval">�E��</param>
		void operator+= (const float rval);

		/// <summary>
		/// Vec2 = (Vec2 -= Vec2) 
		/// </summary>
		/// <param name="rval">�E��</param>
		void operator-= (const Vector2& rval);

		/// <summary>
		/// Vec2 = Vec2 * float 
		/// </summary>
		/// <param name="scale">�X�P�[��</param>
		/// <returns>�v�Z����</returns>
		Vector2 operator*(const float scale)const;

		/// <summary>
		/// Vec2 = Vec2 / floats 
		/// </summary>
		/// <param name="div">���Z</param>
		///	<returns>�v�Z����</returns>
		Vector2 operator/(const float div)const;

		/// <summary>
		/// Vec2 = (Vec2 *= float) 
		/// </summary>
		/// <param name="scale">�X�P�[��</param>
		void operator*=(const float scale);

		/// <summary>
		/// Vec2 = (Vec2 /= float) 
		/// </summary>
		/// <param name="div">���Z</param>
		void operator/=(const float div);

		/// <summary>
		/// Vec2 == Vec2 
		/// </summary>
		/// <param name="rval">�E��</param>
		/// <returns>�v�Z����</returns>
		bool operator==(const Vector2& rval) const;

		/// <summary>
		/// Vec2 != Vec2
		/// </summary>
		/// <param name="rval">�E��</param>
		/// <returns>�v�Z����</returns>
		bool operator!=(const Vector2& rval) const;

		/// <summary>
		/// �x�N�^�[�t�](-) 
		/// </summary>
		/// <returns>�v�Z����</returns>
		Vector2 operator-() const;

		/// <summary>
		/// �x�N�g���̑傫����Ԃ�
		/// </summary>
		/// <returns>�傫��</returns>
		float Length() const;

		/// <summary>
		/// �x�N�g���̑傫����2���Ԃ� 
		/// </summary>
		/// <returns>�傫����2��</returns>
		float SQLength() const;

		/// <summary>
		/// �x�N�g���̐��K�� 
		/// </summary>
		void Normalize();

		/// <summary>
		/// ���K�������x�N�g����Ԃ� 
		/// </summary>
		/// <returns>���K�������x�N�g��</returns>
		Vector2 Normalized() const;

	public:
		float x, y;
	};
}