#pragma once
#include <DxLib.h>

class Matrix;

/// <summary>
/// Vector3
/// </summary>
class Vector3
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Vector3();
	Vector3(const float x, const float y, const float z);

	/// <summary>
	/// Vec3 = Vec3 + Vec3 
	/// </summary>
	/// <param name="rval">右辺</param>
	/// <returns>計算結果</returns>
	Vector3 operator+ (const Vector3& rval) const;

	/// <summary>
	/// Vec3 = Vec3 - Vec3 
	/// </summary>
	/// <param name="rval">右辺</param>
	/// <returns>計算結果</returns>
	Vector3 operator- (const Vector3& rval) const;

	/// <summary>
	/// Vec3 = Vec3 * Vec3 
	/// </summary>
	/// <param name="rval">右辺</param>
	/// <returns>計算結果</returns>
	Vector3 operator* (const Vector3& rval) const;

	/// <summary>
	/// Vec3 = Vec3 / Vec3 
	/// </summary>
	/// <param name="rval">右辺</param>
	/// <returns>計算結果</returns>
	Vector3 operator/ (const Vector3& rval) const;

	/// <summary>
	/// Vec3 = (Vec3 += Vec3) 
	/// </summary>
	/// <param name="rval">右辺</param>
	void operator+= (const Vector3& rval);

	/// <summary>
	/// Vec3 = (Vec3 -= Vec3) 
	/// </summary>
	/// <param name="rval">右辺</param>
	void operator-= (const Vector3& rval);

	/// <summary>
	/// Vec3 = Vec3 + float 
	/// </summary>
	/// <param name="add">足す数</param>
	/// <returns>計算結果</returns>
	Vector3 operator+(const float add)const;

	/// <summary>
	/// Vec3 = Vec3 - float 
	/// </summary>
	/// <param name="sub">引く数</param>
	/// <returns>計算結果</returns>
	Vector3 operator-(const float sub)const;

	/// <summary>
	/// Vec3 = Vec3 * float 
	/// </summary>
	/// <param name="scale">スケール</param>
	/// <returns>計算結果</returns>
	Vector3 operator*(const float scale)const;

	/// <summary>
	/// Vec3 = Vec3 / floats 
	/// </summary>
	/// <param name="div">除算</param>
	/// <returns>計算結果</returns>
	Vector3 operator/(const float div)const;

	/// <summary>
	/// Vec3 = (Vec3 += float) 
	/// </summary>
	/// <param name="add">足す数</param>
	void operator+=(const float add);

	/// <summary>
	/// Vec3 = (Vec3 -= float) 
	/// </summary>
	/// <param name="sub">引く数</param>
	void operator-=(const float sub);

	/// <summary>
	/// Vec3 = (Vec3 *= float) 
	/// </summary>
	/// <param name="scale">スケール</param>
	void operator*=(const float scale);

	/// <summary>
	/// Vec3 = (Vec3 /= float) 
	/// </summary>
	/// <param name="div">除算</param>
	void operator/=(const float div);

	/// <summary>
	/// ベクター逆転(-) 
	/// </summary>
	/// <returns>計算結果</returns>
	Vector3 operator-() const;

	/// <summary>
	/// ベクトル代入 
	/// </summary>
	/// <param name="vec">代入するベクトル</param>
	/// <returns>計算結果</returns>
	Vector3 operator=(const Vector3& vec);

	/// <summary>
	/// 等号演算子の比較 
	/// </summary>
	/// <param name="vec">比較するベクトル</param>
	/// <returns>計算結果</returns>
	bool operator==(const Vector3& vec);

	/// <summary>
	/// 不等号演算子の比較 
	/// </summary>
	/// <param name="vec">比較するベクトル</param>
	/// <returns>計算結果</returns>
	bool operator!=(const Vector3& vec);

	/// <summary>
	/// 小なりイコール演算子の比較 
	/// </summary>
	/// <param name="vec">比較するベクトル</param>
	/// <returns>計算結果</returns>
	bool operator<=(const Vector3& vec);

	/// <summary>
	/// 大なりイコール演算子の比較 
	/// </summary>
	/// <param name="vec">比較するベクトル</param>
	/// <returns>計算結果</returns>
	bool operator>=(const Vector3& vec);

	/// <summary>
	/// ベクトルの大きさを返す
	/// </summary>
	/// <returns>ベクトルの大きさ</returns>
	float Length() const;

    /// <summary>
    /// ベクトルの大きさの2乗を返す 
    /// </summary>
	/// <returns>ベクトルの大きさの2乗</returns>
    float SQLength() const;

	/// <summary>
	/// 2つのベクトル間のユークリッド距離を計算 
	/// </summary>
	/// <param name="other">もう一つのベクトル</param>
	/// <returns>ユークリッド距離</returns>
	float Distance(const Vector3& other) const;

	/// <summary>
	/// 2つのベクトルのなす角度を求める 
	/// </summary>
	/// <param name="other">もう一つのベクトル</param>
	/// <returns>角度</returns>
	float Angle(const Vector3& other) const;

	/// <summary>
	/// ベクトルの正規化 
	/// </summary>
	void Normalize();

	/// <summary>
	/// 正規化したベクトルを返す 
	/// </summary>
	/// <returns>正規化されたベクトル</returns>
	Vector3 Normalized() const;

	/// <summary>
	/// DXライブラリのベクトルに変換 
	/// </summary>
	/// <returns>DXライブラリのベクトル</returns>
	VECTOR ToDxLibVector3() const;

	/// <summary>
	/// ベクトルのNaNチェック
	/// </summary>
	/// <returns>true : NaN、false : NaNではない</returns>
	bool CheckNaN();


	//// 静的メンバ関数 ////

	/// <summary>
	/// DXライブラリのベクトルからベクトルに変換 
	/// </summary>
	/// <param name="vec">DXライブラリのベクトル</param>
	/// <returns>変換されたベクトル</returns>
	static Vector3 FromDxLibVector3(const VECTOR& vec);

	/// <summary>
	/// 外積 
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// 内積
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static float Dot(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// 反射ベクトル 
	/// </summary>
	/// <param name="vec">ベクトル</param>
	/// <param name="normal">法線ベクトル</param>
	/// <returns>反射ベクトル</returns>
	static Vector3 Reflect(const Vector3& vec, const Vector3& normal);

	/// <summary>
	/// スライドベクトル 
	/// </summary>
	/// <param name="vec">ベクトル</param>
	/// <param name="normal">法線ベクトル</param>
	/// <returns>スライドベクトル</returns>
	static Vector3 Slide(const Vector3& vec, const Vector3& normal);

	/// <summary>
	/// 線形補間 
	/// </summary>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <param name="value">補間値</param>
	/// <returns>補間されたベクトル</returns>
	static Vector3 Lerp(const Vector3& start, const Vector3& end, float value);

	/// <summary>
	/// エルミート補間 
	/// </summary>
	/// <param name="startPos">曲線の始点の位置ベクトル</param>
	/// <param name="startTangent">始点での接ベクトル</param>
	/// <param name="endPos">曲線の終点の位置ベクトル</param>
	/// <param name="endTangent">終点での接ベクトル</param>
	/// <param name="value">補完値(0～1)</param>
	/// <returns>補完されたベクトル</returns>
	static Vector3 Hermite(const Vector3& startPos, const Vector3& startTangent, const Vector3& endPos, const Vector3& endTangent, float value);
	
	/// <summary>
	/// ベクトルを行列で変換 
	/// </summary>
	/// <param name="vec">変換するベクトル</param>
	/// <param name="mat">適用する変換行列</param>
	/// <returns>変換後のベクトル</returns>
	static Vector3 Transform(const Vector3& vec, const Matrix& mat);

public:
	float x, y, z;
};