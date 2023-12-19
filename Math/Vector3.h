#pragma once

class Matrix;

class Vector3
{
public:
	Vector3();
	Vector3(float x, float y, float z);

	// Vec3 = Vec3 + Vec3
	Vector3 operator+ (const Vector3& rval) const;

	// Vec3 = Vec3 - Vec3
	Vector3 operator- (const Vector3& rval) const;

	// Vec3 = (Vec3 += Vec3)
	void operator+= (const Vector3& rval);

	// Vec3 = (Vec3 -= Vec3)
	void operator-= (const Vector3& rval);

	// Vec3 = Vec3 * float
	Vector3 operator*(float scale)const;

	// Vec3 = Vec3 / floats
	Vector3 operator/(float div)const;

	// Vec3 = (Vec3 *= float)
	void operator*=(float scale);

	// Vec3 = (Vec3 /= float)
	void operator/=(float div);

	// ベクター逆転(-)
	Vector3 operator-() const;

	// ベクトル代入
	Vector3 operator=(const Vector3& vec);

	// ベクトル同士の比較
	bool operator==(const Vector3& vec);

	// ベクトル同士の比較
	bool operator!=(const Vector3& vec);

	/// ベクトルの大きさを返す
	float Length() const;

	// ベクトルの大きさの2乗を返す
    float SQLength() const;

	// ベクトルの正規化
	void Normalize();

	// 正規化したベクトルを返す
	Vector3 Normalized() const;

	// 外積
	Vector3 Cross(const Vector3& v1, const Vector3& v2) const;

	// 内積
	float Dot(const Vector3& v1, const Vector3& v2) const;

	// 反射ベクトル
	Vector3 Reflect(const Vector3& vec, const Vector3& normal) const;

	// スライドベクトル
	Vector3 Slide(const Vector3& vec, const Vector3& normal) const;

	// 線形補間
	Vector3 Lerp(const Vector3& start, const Vector3& end, float value) const;

	// エルミート補間
	Vector3 Hermite(const Vector3& startPos, const Vector3& startTangent, const Vector3& endPos, const Vector3& endTangent, float value) const;
	
	// 行列を使ったベクトルの変換
	Vector3 Transform(const Vector3& vec, const Matrix& mat) const;

public:
	float x, y, z;
};