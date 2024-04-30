#include "Matrix.h"
#include "Vector3.h"
#include <cmath>

// コンストラクタ
Matrix::Matrix()
{
	// 単位行列に初期化
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

// コンストラクタ
Matrix::Matrix(const float m00, const float m01, const float m02, const float m03, const float m10, 
			   const float m11, const float m12, const float m13, const float m20, const float m21, 
			   const float m22, const float m23, const float m30, const float m31, const float m32, const float m33)
{
	// 行列の初期化
	m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
	m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
	m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
	m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
}

// 行列の掛け算
Matrix Matrix::operator*(const Matrix& rval) const
{
	Matrix result;

	// 行列の掛け算
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = m[i][0] * rval.m[0][j] + m[i][1] * rval.m[1][j] + m[i][2] * rval.m[2][j] + m[i][3] * rval.m[3][j];
		}
	}

	return result;
}

// 行列の掛け算
Matrix Matrix::operator*=(const Matrix& rval)
{
	Matrix result;
	return result = *this * rval;
}

// 行列のスカラー倍
Matrix Matrix::operator*(const float scale) const
{
	Matrix result;

	// 行列のスカラー倍
	result.m[0][0] = m[0][0] * scale; result.m[0][1] = m[0][1] * scale; result.m[0][2] = m[0][2] * scale; result.m[0][3] = m[0][3] * scale;
	result.m[1][0] = m[1][0] * scale; result.m[1][1] = m[1][1] * scale; result.m[1][2] = m[1][2] * scale; result.m[1][3] = m[1][3] * scale;
	result.m[2][0] = m[2][0] * scale; result.m[2][1] = m[2][1] * scale; result.m[2][2] = m[2][2] * scale; result.m[2][3] = m[2][3] * scale;
	result.m[3][0] = m[3][0] * scale; result.m[3][1] = m[3][1] * scale; result.m[3][2] = m[3][2] * scale; result.m[3][3] = m[3][3] * scale;

	return result;
}

// 行列のスカラー倍
Matrix Matrix::operator*=(const float scale)
{
	Matrix result;
	return result = *this * scale;
}

// 行列のスカラー除算
Matrix Matrix::operator/(const float div) const
{
	Matrix result;

	// 行列のスカラー除算
	result.m[0][0] = m[0][0] / div; result.m[0][1] = m[0][1] / div; result.m[0][2] = m[0][2] / div; result.m[0][3] = m[0][3] / div;
	result.m[1][0] = m[1][0] / div; result.m[1][1] = m[1][1] / div; result.m[1][2] = m[1][2] / div; result.m[1][3] = m[1][3] / div;
	result.m[2][0] = m[2][0] / div; result.m[2][1] = m[2][1] / div; result.m[2][2] = m[2][2] / div; result.m[2][3] = m[2][3] / div;
	result.m[3][0] = m[3][0] / div; result.m[3][1] = m[3][1] / div; result.m[3][2] = m[3][2] / div; result.m[3][3] = m[3][3] / div;

	return result;
}

// 行列のスカラー除算
Matrix Matrix::operator/=(const float div)
{
	Matrix result;
	return result = *this / div;
}

// 行列の転置
Matrix Matrix::Transpose() const
{
	//Matrix result;

	//// 行列の転置
	//result.m[0][0] = m[0][0]; result.m[0][1] = m[1][0]; result.m[0][2] = m[2][0]; result.m[0][3] = m[3][0];
	//result.m[1][0] = m[0][1]; result.m[1][1] = m[1][1]; result.m[1][2] = m[2][1]; result.m[1][3] = m[3][1];
	//result.m[2][0] = m[0][2]; result.m[2][1] = m[1][2]; result.m[2][2] = m[2][2]; result.m[2][3] = m[3][2];
	//result.m[3][0] = m[0][3]; result.m[3][1] = m[1][3]; result.m[3][2] = m[2][3]; result.m[3][3] = m[3][3];

	//return result;

	MATRIX result = MTranspose(ToDxLibMatrix());
	return { result.m[0][0], result.m[0][1], result.m[0][2], result.m[0][3],
			 result.m[1][0], result.m[1][1], result.m[1][2], result.m[1][3],
			 result.m[2][0], result.m[2][1], result.m[2][2], result.m[2][3],
			 result.m[3][0], result.m[3][1], result.m[3][2], result.m[3][3] };
}

// ある向きからある向きへ変換する回転行列を取得
Matrix Matrix::GetRotationMatrix(const Vector3& from, const Vector3& to)
{
	//// ある向きからある向きへ変換する回転行列を取得する
	//Vector3 axis = from.Cross(from, to);
	//axis.Normalize();
	//float angle = acosf(from.Dot(from, to));

	//// 回転行列の計算
	//Matrix result;
	//result.m[0][0] = cosf(angle) + axis.x * axis.x * (1.0f - cosf(angle));
	//result.m[0][1] = axis.x * axis.y * (1.0f - cosf(angle)) - axis.z * sinf(angle);
	//result.m[0][2] = axis.z * axis.x * (1.0f - cosf(angle)) + axis.y * sinf(angle);
	//result.m[0][3] = 0.0f;

	//result.m[1][0] = axis.x * axis.y * (1.0f - cosf(angle)) + axis.z * sinf(angle);
	//result.m[1][1] = cosf(angle) + axis.y * axis.y * (1.0f - cosf(angle));
	//result.m[1][2] = axis.y * axis.z * (1.0f - cosf(angle)) - axis.x * sinf(angle);
	//result.m[1][3] = 0.0f;

	//result.m[2][0] = axis.z * axis.x * (1.0f - cosf(angle)) - axis.y * sinf(angle);
	//result.m[2][1] = axis.y * axis.z * (1.0f - cosf(angle)) + axis.x * sinf(angle);
	//result.m[2][2] = cosf(angle) + axis.z * axis.z * (1.0f - cosf(angle));
	//result.m[2][3] = 0.0f;

	//result.m[3][0] = 0.0f;
	//result.m[3][1] = 0.0f;
	//result.m[3][2] = 0.0f;
	//result.m[3][3] = 1.0f;

	//return result;

	MATRIX result = MGetRotVec2(from.ToDxLibVector3(), to.ToDxLibVector3());
	
	return { result.m[0][0], result.m[0][1], result.m[0][2], result.m[0][3],
			 result.m[1][0], result.m[1][1], result.m[1][2], result.m[1][3],
			 result.m[2][0], result.m[2][1], result.m[2][2], result.m[2][3],
			 result.m[3][0], result.m[3][1], result.m[3][2], result.m[3][3] };
}

// 平行移動行列の取得
Matrix Matrix::GetTranslate(const Vector3& v)
{
	//// 平行移動行列を取得する
	//Matrix result;

	//// 平行移動行列の計算
	//result.m[0][0] = 1.0f; result.m[0][1] = 0.0f; result.m[0][2] = 0.0f; result.m[0][3] = v.x;
	//result.m[1][0] = 0.0f; result.m[1][1] = 1.0f; result.m[1][2] = 0.0f; result.m[1][3] = v.y;
	//result.m[2][0] = 0.0f; result.m[2][1] = 0.0f; result.m[2][2] = 1.0f; result.m[2][3] = v.z;
	//result.m[3][0] = 0.0f; result.m[3][1] = 0.0f; result.m[3][2] = 0.0f; result.m[3][3] = 1.0f;

	//return result;

	MATRIX result = MGetTranslate(v.ToDxLibVector3());
	return { result.m[0][0], result.m[0][1], result.m[0][2], result.m[0][3],
			 result.m[1][0], result.m[1][1], result.m[1][2], result.m[1][3],
			 result.m[2][0], result.m[2][1], result.m[2][2], result.m[2][3],
			 result.m[3][0], result.m[3][1], result.m[3][2], result.m[3][3] };
}

// 拡大縮小行列の取得
Matrix Matrix::GetScale(const Vector3& v)
{
	//// 拡大縮小行列を取得する
	//Matrix result;

	//// 拡大縮小行列の計算
	//result.m[0][0] = v.x; result.m[0][1] = 0.0f; result.m[0][2] = 0.0f; result.m[0][3] = 0.0f;
	//result.m[1][0] = 0.0f; result.m[1][1] = v.y; result.m[1][2] = 0.0f; result.m[1][3] = 0.0f;
	//result.m[2][0] = 0.0f; result.m[2][1] = 0.0f; result.m[2][2] = v.z; result.m[2][3] = 0.0f;
	//result.m[3][0] = 0.0f; result.m[3][1] = 0.0f; result.m[3][2] = 0.0f; result.m[3][3] = 1.0f;

	//return result;

	MATRIX result = MGetScale(v.ToDxLibVector3());
	return { result.m[0][0], result.m[0][1], result.m[0][2], result.m[0][3],
				 result.m[1][0], result.m[1][1], result.m[1][2], result.m[1][3],
				 result.m[2][0], result.m[2][1], result.m[2][2], result.m[2][3],
				 result.m[3][0], result.m[3][1], result.m[3][2], result.m[3][3] };
}

// X軸回転行列の取得
Matrix Matrix::GetRotationX(const float angle)
{
	//// x軸回転行列を取得する
	//Matrix result;

	//// x軸回転行列の計算
	//result.m[0][0] = 1.0f; result.m[0][1] = 0.0f; result.m[0][2] = 0.0f; result.m[0][3] = 0.0f;
	//result.m[1][0] = 0.0f; result.m[1][1] = cosf(angle); result.m[1][2] = -sinf(angle); result.m[1][3] = 0.0f;
	//result.m[2][0] = 0.0f; result.m[2][1] = sinf(angle); result.m[2][2] = cosf(angle); result.m[2][3] = 0.0f;
	//result.m[3][0] = 0.0f; result.m[3][1] = 0.0f; result.m[3][2] = 0.0f; result.m[3][3] = 1.0f;

	//return result;

	MATRIX result = MGetRotX(angle);
	return { result.m[0][0], result.m[0][1], result.m[0][2], result.m[0][3],
			 result.m[1][0], result.m[1][1], result.m[1][2], result.m[1][3],
			 result.m[2][0], result.m[2][1], result.m[2][2], result.m[2][3],
			 result.m[3][0], result.m[3][1], result.m[3][2], result.m[3][3] };
}

// Y軸回転行列の取得
Matrix Matrix::GetRotationY(const float angle)
{
	//// y軸回転行列を取得する
	//Matrix result;

	//// y軸回転行列の計算
	//result.m[0][0] = cosf(angle); result.m[0][1] = 0.0f; result.m[0][2] = sinf(angle); result.m[0][3] = 0.0f;
	//result.m[1][0] = 0.0f; result.m[1][1] = 1.0f; result.m[1][2] = 0.0f; result.m[1][3] = 0.0f;
	//result.m[2][0] = -sinf(angle); result.m[2][1] = 0.0f; result.m[2][2] = cosf(angle); result.m[2][3] = 0.0f;
	//result.m[3][0] = 0.0f; result.m[3][1] = 0.0f; result.m[3][2] = 0.0f; result.m[3][3] = 1.0f;

	//return result;

	MATRIX result = MGetRotY(angle);
	return { result.m[0][0], result.m[0][1], result.m[0][2], result.m[0][3],
			 result.m[1][0], result.m[1][1], result.m[1][2], result.m[1][3],
			 result.m[2][0], result.m[2][1], result.m[2][2], result.m[2][3],
			 result.m[3][0], result.m[3][1], result.m[3][2], result.m[3][3] };
}

// Z軸回転行列の取得
Matrix Matrix::GetRotationZ(const float angle)
{
	//// z軸回転行列を取得する
	//Matrix result;

	//// z軸回転行列の計算
	//result.m[0][0] = cosf(angle); result.m[0][1] = -sinf(angle); result.m[0][2] = 0.0f; result.m[0][3] = 0.0f;
	//result.m[1][0] = sinf(angle); result.m[1][1] = cosf(angle); result.m[1][2] = 0.0f; result.m[1][3] = 0.0f;
	//result.m[2][0] = 0.0f; result.m[2][1] = 0.0f; result.m[2][2] = 1.0f; result.m[2][3] = 0.0f;
	//result.m[3][0] = 0.0f; result.m[3][1] = 0.0f; result.m[3][2] = 0.0f; result.m[3][3] = 1.0f;
	//return result;

	MATRIX result = MGetRotZ(angle);
	return { result.m[0][0], result.m[0][1], result.m[0][2], result.m[0][3],
			 result.m[1][0], result.m[1][1], result.m[1][2], result.m[1][3],
			 result.m[2][0], result.m[2][1], result.m[2][2], result.m[2][3],
			 result.m[3][0], result.m[3][1], result.m[3][2], result.m[3][3] };
}

// 行列からオイラー角に変換
Vector3 Matrix::ToEulerAngle(const Matrix& m)
{
	// 行列からオイラー角に変換
	Vector3 result;

	// オイラー角の計算
	result.x = atan2f(m.m[2][1], m.m[2][2]);
	result.y = atan2f(-m.m[2][0], sqrtf(m.m[2][1] * m.m[2][1] + m.m[2][2] * m.m[2][2]));
	result.z = atan2f(m.m[1][0], m.m[0][0]);

	return result;
}

// 行列からオイラー角に変換
Vector3 Matrix::ToEulerAngle() const
{
	Vector3 result;
	const float Threshold = 0.0001f;

	// ジンバルロック判定
	if (m[2][1] > 1.0f - Threshold || m[2][1] < -1.0f + Threshold)
	{
		result.x = (m[2][1] < 0 ? (DX_PI_F / 2) : -(DX_PI_F / 2));
		result.y = atan2f(-m[0][2], m[0][0]);
		result.z = 0.0f;
		return result;
	}

	result.x = -asinf(m[2][1]);
	result.z = asinf(m[0][1] / cosf(result.x));

	// ジンバルロック判定
	if ((isnan(result.z) != 0))
	{
		result.x = (m[2][1] < 0 ? (DX_PI_F / 2) : -(DX_PI_F / 2));
		result.y = atan2f(-m[0][2], m[0][0]);
		result.z = 0.0f;
		return result;
	}

	if (m[1][1] < 0.0f)
	{
		result.z = DX_PI_F - result.z;
	}

	result.y = atan2f(m[2][0], m[2][2]);

	//// オイラー角の計算
	//result.x = atan2f(m[2][1], m[2][2]);
	//result.y = atan2f(-m[2][0], sqrtf(m[2][1] * m[2][1] + m[2][2] * m[2][2]));
	//result.z = atan2f(m[1][0], m[0][0]);

	return result;
}

// DXライブラリの行列に変換
MATRIX Matrix::ToDxLibMatrix() const
{
	MATRIX result{};

	for (int i = 0; i < 4; i++)
	{
		result.m[i][0] = m[i][0];
		result.m[i][1] = m[i][1];
		result.m[i][2] = m[i][2];
		result.m[i][3] = m[i][3];
	}
	return result;
}