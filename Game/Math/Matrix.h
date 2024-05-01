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

	/// <summary>
	/// 行列のスカラー倍 
	/// </summary>
	/// <param name="scale">スカラー</param>
	/// <returns>計算結果</returns>
	Matrix operator*(const float scale) const;

	/// <summary>
	/// 行列のスカラー倍
	/// </summary>
	/// <param name="scale">スカラー</param>
	/// <returns>計算結果</returns>
	Matrix operator*=(const float scale);

	/// <summary>
	/// 行列のスカラー除算 
	/// </summary>
	/// <param name="div">スカラー</param>
	/// <returns>計算結果</returns>
	Matrix operator/(const float div) const;

	/// <summary>
	/// 行列のスカラー除算
	/// </summary>
	/// <param name="div">スカラー</param>
	/// <returns>計算結果</returns>
	Matrix operator/=(const float div);

	/// <summary>
	/// 行列の転置 
	/// </summary>
	Matrix Transpose() const;

	/// <summary>
	/// 行列からオイラー角に変換 
	/// </summary>
	/// <returns>オイラー角</returns>
	Vector3 ToEulerAngle() const;

	/// <summary>
	/// DXライブラリの行列に変換 
	/// </summary>
	/// <returns></returns>
	MATRIX ToDxLibMatrix() const;


	//// 静的メンバ関数 ////
	
	/// <summary>
	/// ある向きからある向きへ変換する回転行列を取得する 
	/// </summary>
	/// <param name="from">向く前の方向</param>
	/// <param name="to">向きたい方向</param>
	/// <returns>変換後の回転行列</returns>
	static Matrix GetRotationMatrix(const Vector3& from, const Vector3& to);

	/// <summary>
	/// 平行移動行列を取得する 
	/// </summary>
	/// <param name="v">平行移動量</param>
	/// <returns>平行移動行列</returns>
	static Matrix GetTranslate(const Vector3& v);

	/// <summary>
	/// 拡大縮小行列を取得する 
	/// </summary>
	/// <param name="v">拡大縮小量</param>
	/// <returns>拡大縮小行列</returns>
	static Matrix GetScale(const Vector3& v);

	/// <summary>
	/// x軸回転行列を取得する 
	/// </summary>
	/// <param name="angle">回転角度</param>
	/// <returns>回転行列</returns>
	static Matrix GetRotationX(const float angle);

	/// <summary>
	/// y軸回転行列を取得する 
	/// </summary>
	/// <param name="angle">回転角度</param>
	/// <returns>回転行列</returns>
	static Matrix GetRotationY(const float angle);

	/// <summary>
	/// z軸回転行列を取得する 
	/// </summary>
	/// <param name="angle">回転角度</param>
	/// <returns>回転行列</returns>
	static Matrix GetRotationZ(const float angle);

	/// <summary>
	/// 行列からオイラー角に変換
	/// </summary>
	/// <param name="m">行列</param>
	/// <returns>オイラー角</returns>
	static Vector3 ToEulerAngle(const Matrix& m);

public:
	float m[4][4];
};