#pragma once
#include <DxLib.h>

// プロトタイプ宣言
class Vector3;

/// <summary>
/// 行列クラス
/// </summary>
class Matrix
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Matrix();
	Matrix(const float m00, const float m01, const float m02, const float m03,
		   const float m10, const float m11, const float m12, const float m13,
		   const float m20, const float m21, const float m22, const float m23,
		   const float m30, const float m31, const float m32, const float m33);

	/// <summary>
	/// 行列の掛け算
	/// </summary>
	/// <param name="rval">右辺の行列</param>
	/// <returns>掛け算結果の行列</returns>
	Matrix operator*(const Matrix& rval) const;

	/// <summary>
	/// 行列の掛け算
	/// </summary>
	/// <param name="rval"></param>
	/// <returns></returns>
	Matrix operator*=(const Matrix& rval);

	// 行列のスカラー倍
	Matrix operator*(const float scale) const;
	Matrix operator*=(const float scale);

	// 行列のスカラー除算
	Matrix operator/(const float div) const;
	Matrix operator/=(const float div);

	// 行列の転置
	Matrix Transpose() const;

	// 行列からオイラー角に変換
	Vector3 ToEulerAngle() const;

	// DXライブラリの行列に変換
	MATRIX ToDxLibMatrix() const;


	//// 静的メンバ関数 ////
	
	// ある向きからある向きへ変換する回転行列を取得する
	static Matrix GetRotationMatrix(const Vector3& from, const Vector3& to);

	// 平行移動行列を取得する
	static Matrix GetTranslate(const Vector3& v);

	// 拡大縮小行列を取得する
	static Matrix GetScale(const Vector3& v);

	// x軸回転行列を取得する
	static Matrix GetRotationX(const float angle);

	// y軸回転行列を取得する
	static Matrix GetRotationY(const float angle);

	// z軸回転行列を取得する
	static Matrix GetRotationZ(const float angle);

	// 行列からオイラー角に変換
	static Vector3 ToEulerAngle(const Matrix& m);

public:
	float m[4][4];
};