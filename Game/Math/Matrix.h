#pragma once
#include <DxLib.h>

namespace Math
{
	// �v���g�^�C�v�錾
	class Vector3;

	/// <summary>
	/// �s��N���X
	/// </summary>
	class Matrix
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		Matrix();
		Matrix(const float m00, const float m01, const float m02, const float m03,
			const float m10, const float m11, const float m12, const float m13,
			const float m20, const float m21, const float m22, const float m23,
			const float m30, const float m31, const float m32, const float m33);

		/// <summary>
		/// �s��̊|���Z
		/// </summary>
		/// <param name="rval">�E�ӂ̍s��</param>
		/// <returns>�|���Z���ʂ̍s��</returns>
		Matrix operator*(const Matrix& rval) const;

		/// <summary>
		/// �s��̊|���Z
		/// </summary>
		/// <param name="rval"></param>
		/// <returns></returns>
		Matrix operator*=(const Matrix& rval);

		/// <summary>
		/// �s��̃X�J���[�{ 
		/// </summary>
		/// <param name="scale">�X�J���[</param>
		/// <returns>�v�Z����</returns>
		Matrix operator*(const float scale) const;

		/// <summary>
		/// �s��̃X�J���[�{
		/// </summary>
		/// <param name="scale">�X�J���[</param>
		/// <returns>�v�Z����</returns>
		Matrix operator*=(const float scale);

		/// <summary>
		/// �s��̃X�J���[���Z 
		/// </summary>
		/// <param name="div">�X�J���[</param>
		/// <returns>�v�Z����</returns>
		Matrix operator/(const float div) const;

		/// <summary>
		/// �s��̃X�J���[���Z
		/// </summary>
		/// <param name="div">�X�J���[</param>
		/// <returns>�v�Z����</returns>
		Matrix operator/=(const float div);

		/// <summary>
		/// �s��̓]�u 
		/// </summary>
		Matrix Transpose() const;

		/// <summary>
		/// �s�񂩂�I�C���[�p�ɕϊ� 
		/// </summary>
		/// <returns>�I�C���[�p</returns>
		Vector3 ToEulerAngle() const;

		/// <summary>
		/// DX���C�u�����̍s��ɕϊ� 
		/// </summary>
		/// <returns></returns>
		MATRIX ToDxLibMatrix() const;


		//// �ÓI�����o�֐� ////

		/// <summary>
		/// ����������炠������֕ϊ������]�s����擾���� 
		/// </summary>
		/// <param name="from">�����O�̕���</param>
		/// <param name="to">������������</param>
		/// <returns>�ϊ���̉�]�s��</returns>
		static Matrix GetRotationMatrix(const Vector3& from, const Vector3& to);

		/// <summary>
		/// ���s�ړ��s����擾���� 
		/// </summary>
		/// <param name="v">���s�ړ���</param>
		/// <returns>���s�ړ��s��</returns>
		static Matrix GetTranslate(const Vector3& v);

		/// <summary>
		/// �g��k���s����擾���� 
		/// </summary>
		/// <param name="v">�g��k����</param>
		/// <returns>�g��k���s��</returns>
		static Matrix GetScale(const Vector3& v);

		/// <summary>
		/// x����]�s����擾���� 
		/// </summary>
		/// <param name="angle">��]�p�x</param>
		/// <returns>��]�s��</returns>
		static Matrix GetRotationX(const float angle);

		/// <summary>
		/// y����]�s����擾���� 
		/// </summary>
		/// <param name="angle">��]�p�x</param>
		/// <returns>��]�s��</returns>
		static Matrix GetRotationY(const float angle);

		/// <summary>
		/// z����]�s����擾���� 
		/// </summary>
		/// <param name="angle">��]�p�x</param>
		/// <returns>��]�s��</returns>
		static Matrix GetRotationZ(const float angle);

		/// <summary>
		/// �s�񂩂�I�C���[�p�ɕϊ�
		/// </summary>
		/// <param name="m">�s��</param>
		/// <returns>�I�C���[�p</returns>
		static Vector3 ToEulerAngle(const Matrix& m);

	public:
		float m[4][4];
	};
}