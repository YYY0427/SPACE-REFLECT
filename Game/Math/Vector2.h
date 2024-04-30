#pragma once

/// <summary>
/// Vector2
/// </summary>
class Vector2
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Vector2();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="x">xの値</param>
	/// <param name="y">yの値</param>
	Vector2(const float x, const float y);

	/// <summary>
	/// Vec2 = Vec2 + Vec2 
	/// </summary>
	/// <param name="rval">右辺</param>
	/// <returns>計算結果</returns>
	Vector2 operator+ (const Vector2& rval) const
	{
		return { x + rval.x, y + rval.y };
	}

	/// <summary>
	/// Vec2 = Vec2 - Vec2 
	/// </summary>
	/// <param name="rval">右辺</param>
	/// <returns>計算結果</returns>
	Vector2 operator- (const Vector2& rval) const
	{
		return { x - rval.x, y - rval.y };
	}

	/// <summary>
	/// Vec2 = (Vec2 += Vec2) 
	/// </summary>
	/// <param name="rval">右辺</param>
	void operator+= (const Vector2& rval);

	/// <summary>
	/// Vec2 = (Vec2 -= Vec2) 
	/// </summary>
	/// <param name="rval">右辺</param>
	void operator-= (const Vector2& rval);

	/// <summary>
	/// Vec2 = Vec2 * float 
	/// </summary>
	/// <param name="scale">スケール</param>
	/// <returns>計算結果</returns>
	Vector2 operator*(const float scale)const;

	/// <summary>
	/// Vec2 = Vec2 / floats 
	/// </summary>
	/// <param name="div">除算</param>
	///	<returns>計算結果</returns>
	Vector2 operator/(const float div)const;

	/// <summary>
	/// Vec2 = (Vec2 *= float) 
	/// </summary>
	/// <param name="scale">スケール</param>
	void operator*=(const float scale);

	/// <summary>
	/// Vec2 = (Vec2 /= float) 
	/// </summary>
	/// <param name="div">除算</param>
	void operator/=(const float div);

	/// <summary>
	/// Vec2 == Vec2 
	/// </summary>
	/// <param name="rval">右辺</param>
	/// <returns>計算結果</returns>
	bool operator==(const Vector2& rval) const;

	/// <summary>
	/// Vec2 != Vec2
	/// </summary>
	/// <param name="rval">右辺</param>
	/// <returns>計算結果</returns>
	bool operator!=(const Vector2& rval) const;

	/// <summary>
	/// ベクター逆転(-) 
	/// </summary>
	/// <returns>計算結果</returns>
	Vector2 operator-() const;

	/// <summary>
	/// ベクトルの大きさを返す
	/// </summary>
	/// <returns>大きさ</returns>
	float Length() const;

	/// <summary>
	/// ベクトルの大きさの2乗を返す 
	/// </summary>
	/// <returns>大きさの2乗</returns>
	float SQLength() const;

	/// <summary>
	/// ベクトルの正規化 
	/// </summary>
	void Normalize();

	/// <summary>
	/// 正規化したベクトルを返す 
	/// </summary>
	/// <returns>正規化したベクトル</returns>
	Vector2 Normalized() const;

public:
	float x, y;
};