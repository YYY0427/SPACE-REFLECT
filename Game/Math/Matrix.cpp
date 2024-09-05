#include "Matrix.h"
#include "Vector3.h"
#include <cmath>

namespace Math
{
	// �R���X�g���N�^
	Matrix::Matrix()
	{
		// �P�ʍs��ɏ�����
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	}

	// �R���X�g���N�^
	Matrix::Matrix(const float m00, const float m01, const float m02, const float m03, const float m10,
		const float m11, const float m12, const float m13, const float m20, const float m21,
		const float m22, const float m23, const float m30, const float m31, const float m32, const float m33)
	{
		// �s��̏�����
		m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
		m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
		m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
		m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
	}

	// �s��̊|���Z
	Matrix Matrix::operator*(const Matrix& rval) const
	{
		Matrix result;

		// �s��̊|���Z
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result.m[i][j] = m[i][0] * rval.m[0][j] + m[i][1] * rval.m[1][j] + m[i][2] * rval.m[2][j] + m[i][3] * rval.m[3][j];
			}
		}

		return result;
	}

	// �s��̊|���Z
	Matrix Matrix::operator*=(const Matrix& rval)
	{
		Matrix result;
		return result = *this * rval;
	}

	// �s��̃X�J���[�{
	Matrix Matrix::operator*(const float scale) const
	{
		Matrix result;

		// �s��̃X�J���[�{
		result.m[0][0] = m[0][0] * scale; result.m[0][1] = m[0][1] * scale; result.m[0][2] = m[0][2] * scale; result.m[0][3] = m[0][3] * scale;
		result.m[1][0] = m[1][0] * scale; result.m[1][1] = m[1][1] * scale; result.m[1][2] = m[1][2] * scale; result.m[1][3] = m[1][3] * scale;
		result.m[2][0] = m[2][0] * scale; result.m[2][1] = m[2][1] * scale; result.m[2][2] = m[2][2] * scale; result.m[2][3] = m[2][3] * scale;
		result.m[3][0] = m[3][0] * scale; result.m[3][1] = m[3][1] * scale; result.m[3][2] = m[3][2] * scale; result.m[3][3] = m[3][3] * scale;

		return result;
	}

	// �s��̃X�J���[�{
	Matrix Matrix::operator*=(const float scale)
	{
		Matrix result;
		return result = *this * scale;
	}

	// �s��̃X�J���[���Z
	Matrix Matrix::operator/(const float div) const
	{
		Matrix result;

		// �s��̃X�J���[���Z
		result.m[0][0] = m[0][0] / div; result.m[0][1] = m[0][1] / div; result.m[0][2] = m[0][2] / div; result.m[0][3] = m[0][3] / div;
		result.m[1][0] = m[1][0] / div; result.m[1][1] = m[1][1] / div; result.m[1][2] = m[1][2] / div; result.m[1][3] = m[1][3] / div;
		result.m[2][0] = m[2][0] / div; result.m[2][1] = m[2][1] / div; result.m[2][2] = m[2][2] / div; result.m[2][3] = m[2][3] / div;
		result.m[3][0] = m[3][0] / div; result.m[3][1] = m[3][1] / div; result.m[3][2] = m[3][2] / div; result.m[3][3] = m[3][3] / div;

		return result;
	}

	// �s��̃X�J���[���Z
	Matrix Matrix::operator/=(const float div)
	{
		Matrix result;
		return result = *this / div;
	}

	// �s��̓]�u
	Matrix Matrix::Transpose() const
	{
		//Matrix result;

		//// �s��̓]�u
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

	// ����������炠������֕ϊ������]�s����擾
	Matrix Matrix::GetRotationMatrix(const Vector3& from, const Vector3& to)
	{
		//// ����������炠������֕ϊ������]�s����擾����
		//Vector3 axis = from.Cross(from, to);
		//axis.Normalize();
		//float angle = acosf(from.Dot(from, to));

		//// ��]�s��̌v�Z
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

	// ���s�ړ��s��̎擾
	Matrix Matrix::GetTranslate(const Vector3& v)
	{
		//// ���s�ړ��s����擾����
		//Matrix result;

		//// ���s�ړ��s��̌v�Z
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

	// �g��k���s��̎擾
	Matrix Matrix::GetScale(const Vector3& v)
	{
		//// �g��k���s����擾����
		//Matrix result;

		//// �g��k���s��̌v�Z
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

	// X����]�s��̎擾
	Matrix Matrix::GetRotationX(const float angle)
	{
		//// x����]�s����擾����
		//Matrix result;

		//// x����]�s��̌v�Z
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

	// Y����]�s��̎擾
	Matrix Matrix::GetRotationY(const float angle)
	{
		//// y����]�s����擾����
		//Matrix result;

		//// y����]�s��̌v�Z
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

	// Z����]�s��̎擾
	Matrix Matrix::GetRotationZ(const float angle)
	{
		//// z����]�s����擾����
		//Matrix result;

		//// z����]�s��̌v�Z
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

	// �s�񂩂�I�C���[�p�ɕϊ�
	Vector3 Matrix::ToEulerAngle(const Matrix& m)
	{
		// �s�񂩂�I�C���[�p�ɕϊ�
		Vector3 result;

		// �I�C���[�p�̌v�Z
		result.x = atan2f(m.m[2][1], m.m[2][2]);
		result.y = atan2f(-m.m[2][0], sqrtf(m.m[2][1] * m.m[2][1] + m.m[2][2] * m.m[2][2]));
		result.z = atan2f(m.m[1][0], m.m[0][0]);

		return result;
	}

	// �s�񂩂�I�C���[�p�ɕϊ�
	Vector3 Matrix::ToEulerAngle() const
	{
		Vector3 result;
		const float Threshold = 0.0001f;

		// �W���o�����b�N����
		if (m[2][1] > 1.0f - Threshold || m[2][1] < -1.0f + Threshold)
		{
			result.x = (m[2][1] < 0 ? (DX_PI_F / 2) : -(DX_PI_F / 2));
			result.y = atan2f(-m[0][2], m[0][0]);
			result.z = 0.0f;
			return result;
		}

		result.x = -asinf(m[2][1]);
		result.z = asinf(m[0][1] / cosf(result.x));

		// �W���o�����b�N����
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

		//// �I�C���[�p�̌v�Z
		//result.x = atan2f(m[2][1], m[2][2]);
		//result.y = atan2f(-m[2][0], sqrtf(m[2][1] * m[2][1] + m[2][2] * m[2][2]));
		//result.z = atan2f(m[1][0], m[0][0]);

		return result;
	}

	// DX���C�u�����̍s��ɕϊ�
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
}