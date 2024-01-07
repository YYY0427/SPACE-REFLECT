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
	Matrix();
	Matrix(float m00, float m01, float m02, float m03,
		   float m10, float m11, float m12, float m13,
		   float m20, float m21, float m22, float m23,
		   float m30, float m31, float m32, float m33);

	// 行列の掛け算
	Matrix operator*(const Matrix& rval) const;
	Matrix operator*=(const Matrix& rval);

	// 行列のスカラー倍
	Matrix operator*(float scale) const;
	Matrix operator*=(float scale);

	// 行列のスカラー除算
	Matrix operator/(float div) const;
	Matrix operator/=(float div);

	// 行列の転置
	Matrix Transpose() const;

	// 行列からオイラー角に変換
	Vector3 ToEulerAngle() const;

	// DXライブラリの行列に変換
	MATRIX ToDxLibMatrix() const;

	/* 静的メンバ関数 */
	// ある向きからある向きへ変換する回転行列を取得する
	static Matrix GetRotationMatrix(const Vector3& from, const Vector3& to);

	// 平行移動行列を取得する
	static Matrix GetTranslate(const Vector3& v);

	// 拡大縮小行列を取得する
	static Matrix GetScale(const Vector3& v);

	// x軸回転行列を取得する
	static Matrix GetRotationX(float angle);

	// y軸回転行列を取得する
	static Matrix GetRotationY(float angle);

	// z軸回転行列を取得する
	static Matrix GetRotationZ(float angle);

	// 行列からオイラー角に変換
	static Vector3 ToEulerAngle(const Matrix& m);

public:
	float m[4][4];
};