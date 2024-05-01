#include "Vector3.h"
#include "Matrix.h"
#include <cmath>

// コンストラクタ
Vector3::Vector3() :
	x(0.0f),
	y(0.0f),
	z(0.0f)
{
}

// コンストラクタ
Vector3::Vector3(const float x, const float y, const float z) :
	x(x),
	y(y),
	z(z)
{
}

// Vec3 = Vec3 + Vec3
Vector3 Vector3::operator+(const Vector3& rval) const
{
	return { x + rval.x, y + rval.y, z + rval.z };
}

// Vec3 = Vec3 - Vec3
Vector3 Vector3::operator-(const Vector3& rval) const
{
	return { x - rval.x, y - rval.y, z - rval.z };
}

// Vec3 = Vec3 * Vec3
Vector3 Vector3::operator*(const Vector3& rval) const
{
	return { x * rval.x, y * rval.y, z * rval.z };
}

// Vec3 = Vec3 / Vec3
Vector3 Vector3::operator/(const Vector3& rval) const
{
	return { x / rval.x, y / rval.y, z / rval.z };
}

// Vec3 = (Vec3 += Vec3)
void Vector3::operator+=(const Vector3& rval)
{
	x += rval.x;
	y += rval.y;
	z += rval.z;
}

// Vec3 = (Vec3 -= Vec3)
void Vector3::operator-=(const Vector3& rval)
{
	x -= rval.x;
	y -= rval.y;
	z -= rval.z;
}

// Vec3 = Vec3 + float
Vector3 Vector3::operator+(const float add) const
{
	return { x + add, y + add, z + add };
}

// Vec3 = Vec3 - float
Vector3 Vector3::operator-(const float sub) const
{
	return { x - sub, y - sub, z - sub };
}

// Vec3 = Vec3 * float
Vector3 Vector3::operator*(const float scale) const
{
	return { x * scale, y * scale, z * scale };
}

// Vec3 = Vec3 / float
Vector3 Vector3::operator/(const float div) const
{
	return { x / div, y / div, z / div };
}

// Vec3 = (Vec3 += float)
void Vector3::operator+=(const float add)
{
	x += add;
	y += add;
	z += add;
}

// Vec3 = (Vec3 -= float)
void Vector3::operator-=(const float sub)
{
	x -= sub;
	y -= sub;
	z -= sub;
}

// Vec3 = (Vec3 *= float)
void Vector3::operator*=(const float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
}

// Vec3 = (Vec3 /= float)
void Vector3::operator/=(const float div)
{
	x /= div;
	y /= div;
	z /= div;
}

// Vec3 = -Vec3
Vector3 Vector3::operator-()const
{
	return { -x, -y, -z };
}

// Vec3 = Vec3
Vector3 Vector3::operator=(const Vector3& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}

// Vec3 == Vec3
bool Vector3::operator==(const Vector3& vec)
{
	return (x == vec.x && y == vec.y && z == vec.z);
}

// Vec3 != Vec3
bool Vector3::operator!=(const Vector3& vec)
{
	return (x != vec.x || y != vec.y || z != vec.z);
}

// Vec3 <= Vec3
bool Vector3::operator<=(const Vector3& vec)
{
	return (x <= vec.x && y <= vec.y && z <= vec.z);
}

// Vec3 >= Vec3
bool Vector3::operator>=(const Vector3& vec)
{
	return (x >= vec.x && y >= vec.y && z >= vec.z);
}

/// ベクトルの大きさを返す
float Vector3::Length() const
{
//	return sqrtf(x * x + y * y + z * z);
	return VSize(ToDxLibVector3());
//	return pow((x * x) + (y * y) + (z * z), 0.5);
}

// ベクトルの大きさの2乗を返す
float Vector3::SQLength() const
{
	return x * x + y * y + z * z;
}

// 2つのベクトル間のユークリッド距離を計算
float Vector3::Distance(const Vector3& other) const
{
	return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2) + std::pow(z - other.z, 2));
}

// 2つのベクトルのなす角度を求める
float Vector3::Angle(const Vector3& other) const
{
	return std::acos(Dot(*this, other) / (Length() * other.Length()));
}

// ベクトルの正規化
void Vector3::Normalize()
{
	float length = Length();
	if(length == 0.0f)
	{
		return;
	}
	x /= length;
	y /= length;
	z /= length;
}

// 正規化したベクトルを返す
Vector3 Vector3::Normalized() const
{
	float length = Length();
	if(length == 0.0f)
	{
		return { 0.0f, 0.0f, 0.0f };
	}
	return { x / length, y / length, z / length };
}

// DXライブラリのベクトルに変換
VECTOR Vector3::ToDxLibVector3() const
{
	return VGet(x, y, z);
}

// ベクトルの内積を返す
float Vector3::Dot(const Vector3& v1, const Vector3& v2) 
{
	/*return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;*/

	return VDot(v1.ToDxLibVector3(), v2.ToDxLibVector3());
}

// DXライブラリのベクトルからベクトルに変換
Vector3 Vector3::FromDxLibVector3(const VECTOR& vec)
{
	return { vec.x, vec.y, vec.z };
}

// ベクトルの外積を返す
Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2) 
{
	/*return { v1.y * v2.z - v1.z * v2.y,
			 v1.z * v2.x - v1.x * v2.z,
			 v1.x * v2.y - v1.y * v2.x };*/

	VECTOR result = VCross(v1.ToDxLibVector3(), v2.ToDxLibVector3());
	return { result.x, result.y, result.z };
}

// 反射ベクトルを返す
Vector3 Vector3::Reflect(const Vector3& vec, const Vector3& normal)
{
	return vec - normal * 2.0f * Dot(vec, normal);
}

// スライドベクトルを返す
Vector3 Vector3::Slide(const Vector3& vec, const Vector3& normal)
{
	return vec - normal * Dot(vec, normal);
}

// 線形補間
Vector3 Vector3::Lerp(const Vector3& start, const Vector3& end, float value) 
{
	return start + (end - start) * value;
}

// エルミート補間
Vector3 Vector3::Hermite(const Vector3& startPos, const Vector3& startTangent, const Vector3& endPos, const Vector3& endTangent, float value)
{
	float t2_3 = value * value * value;
	float t3_2 = value * value;
	float h1 = 2.0f * t2_3 - 3.0f * t3_2 + 1.0f;
	float h2 = -2.0f * t2_3 + 3.0f * t3_2;
	float h3 = t2_3 - 2.0f * t3_2 + value;
	float h4 = t2_3 - t3_2;

	return startPos * h1 + endPos * h2 + startTangent * h3 + endTangent * h4;
}

// 行列を使ったベクトルの変換
Vector3 Vector3::Transform(const Vector3& vec, const Matrix& mat) 
{
	/*return { vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + mat.m[3][0],
			 vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + mat.m[3][1],
			 vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + mat.m[3][2] };*/

	VECTOR result = VTransform(vec.ToDxLibVector3(), mat.ToDxLibMatrix());
	return { result.x, result.y, result.z };
}

// ベクトルのNaNチェック
bool Vector3::CheckNaN()
{
	return std::isnan(x) || std::isnan(y) || std::isnan(z);
}