#pragma once
#include <DxLib.h>

namespace Math
{
	// �v���g�^�C�v�錾
	class Matrix;

	/// <summary>
	/// Vector3
	/// </summary>
	class Vector3
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		Vector3();
		Vector3(const float x, const float y, const float z);

		/// <summary>
		/// Vec3 = Vec3 + Vec3 
		/// </summary>
		/// <param name="rval">�E��</param>
		/// <returns>�v�Z����</returns>
		Vector3 operator+ (const Vector3& rval) const;

		/// <summary>
		/// Vec3 = Vec3 - Vec3 
		/// </summary>
		/// <param name="rval">�E��</param>
		/// <returns>�v�Z����</returns>
		Vector3 operator- (const Vector3& rval) const;

		/// <summary>
		/// Vec3 = Vec3 * Vec3 
		/// </summary>
		/// <param name="rval">�E��</param>
		/// <returns>�v�Z����</returns>
		Vector3 operator* (const Vector3& rval) const;

		/// <summary>
		/// Vec3 = Vec3 / Vec3 
		/// </summary>
		/// <param name="rval">�E��</param>
		/// <returns>�v�Z����</returns>
		Vector3 operator/ (const Vector3& rval) const;

		/// <summary>
		/// Vec3 = (Vec3 += Vec3) 
		/// </summary>
		/// <param name="rval">�E��</param>
		void operator+= (const Vector3& rval);

		/// <summary>
		/// Vec3 = (Vec3 -= Vec3) 
		/// </summary>
		/// <param name="rval">�E��</param>
		void operator-= (const Vector3& rval);

		/// <summary>
		/// Vec3 = Vec3 + float 
		/// </summary>
		/// <param name="add">������</param>
		/// <returns>�v�Z����</returns>
		Vector3 operator+(const float add)const;

		/// <summary>
		/// Vec3 = Vec3 - float 
		/// </summary>
		/// <param name="sub">������</param>
		/// <returns>�v�Z����</returns>
		Vector3 operator-(const float sub)const;

		/// <summary>
		/// Vec3 = Vec3 * float 
		/// </summary>
		/// <param name="scale">�X�P�[��</param>
		/// <returns>�v�Z����</returns>
		Vector3 operator*(const float scale)const;

		/// <summary>
		/// Vec3 = Vec3 / floats 
		/// </summary>
		/// <param name="div">���Z</param>
		/// <returns>�v�Z����</returns>
		Vector3 operator/(const float div)const;

		/// <summary>
		/// Vec3 = (Vec3 += float) 
		/// </summary>
		/// <param name="add">������</param>
		void operator+=(const float add);

		/// <summary>
		/// Vec3 = (Vec3 -= float) 
		/// </summary>
		/// <param name="sub">������</param>
		void operator-=(const float sub);

		/// <summary>
		/// Vec3 = (Vec3 *= float) 
		/// </summary>
		/// <param name="scale">�X�P�[��</param>
		void operator*=(const float scale);

		/// <summary>
		/// Vec3 = (Vec3 /= float) 
		/// </summary>
		/// <param name="div">���Z</param>
		void operator/=(const float div);

		/// <summary>
		/// �x�N�^�[�t�](-) 
		/// </summary>
		/// <returns>�v�Z����</returns>
		Vector3 operator-() const;

		/// <summary>
		/// �x�N�g����� 
		/// </summary>
		/// <param name="vec">�������x�N�g��</param>
		/// <returns>�v�Z����</returns>
		Vector3 operator=(const Vector3& vec);

		/// <summary>
		/// �������Z�q�̔�r 
		/// </summary>
		/// <param name="vec">��r����x�N�g��</param>
		/// <returns>�v�Z����</returns>
		bool operator==(const Vector3& vec);

		/// <summary>
		/// �s�������Z�q�̔�r 
		/// </summary>
		/// <param name="vec">��r����x�N�g��</param>
		/// <returns>�v�Z����</returns>
		bool operator!=(const Vector3& vec);

		/// <summary>
		/// ���Ȃ�C�R�[�����Z�q�̔�r 
		/// </summary>
		/// <param name="vec">��r����x�N�g��</param>
		/// <returns>�v�Z����</returns>
		bool operator<=(const Vector3& vec);

		/// <summary>
		/// ��Ȃ�C�R�[�����Z�q�̔�r 
		/// </summary>
		/// <param name="vec">��r����x�N�g��</param>
		/// <returns>�v�Z����</returns>
		bool operator>=(const Vector3& vec);

		/// <summary>
		/// �x�N�g���̑傫����Ԃ�
		/// </summary>
		/// <returns>�x�N�g���̑傫��</returns>
		float Length() const;

		/// <summary>
		/// �x�N�g���̑傫����2���Ԃ� 
		/// </summary>
		/// <returns>�x�N�g���̑傫����2��</returns>
		float SQLength() const;

		/// <summary>
		/// 2�̃x�N�g���Ԃ̃��[�N���b�h�������v�Z 
		/// </summary>
		/// <param name="other">������̃x�N�g��</param>
		/// <returns>���[�N���b�h����</returns>
		float Distance(const Vector3& other) const;

		/// <summary>
		/// 2�̃x�N�g���̂Ȃ��p�x�����߂� 
		/// </summary>
		/// <param name="other">������̃x�N�g��</param>
		/// <returns>�p�x</returns>
		float Angle(const Vector3& other) const;

		/// <summary>
		/// �x�N�g���̐��K�� 
		/// </summary>
		void Normalize();

		/// <summary>
		/// ���K�������x�N�g����Ԃ� 
		/// </summary>
		/// <returns>���K�����ꂽ�x�N�g��</returns>
		Vector3 Normalized() const;

		/// <summary>
		/// DX���C�u�����̃x�N�g���ɕϊ� 
		/// </summary>
		/// <returns>DX���C�u�����̃x�N�g��</returns>
		VECTOR ToDxLibVector3() const;

		/// <summary>
		/// �x�N�g����NaN�`�F�b�N
		/// </summary>
		/// <returns>true : NaN�Afalse : NaN�ł͂Ȃ�</returns>
		bool CheckNaN();


		//// �ÓI�����o�֐� ////

		/// <summary>
		/// DX���C�u�����̃x�N�g������x�N�g���ɕϊ� 
		/// </summary>
		/// <param name="vec">DX���C�u�����̃x�N�g��</param>
		/// <returns>�ϊ����ꂽ�x�N�g��</returns>
		static Vector3 FromDxLibVector3(const VECTOR& vec);

		/// <summary>
		/// �O�� 
		/// </summary>
		/// <param name="v1"></param>
		/// <param name="v2"></param>
		/// <returns></returns>
		static Vector3 Cross(const Vector3& v1, const Vector3& v2);

		/// <summary>
		/// ����
		/// </summary>
		/// <param name="v1"></param>
		/// <param name="v2"></param>
		/// <returns></returns>
		static float Dot(const Vector3& v1, const Vector3& v2);

		/// <summary>
		/// ���˃x�N�g�� 
		/// </summary>
		/// <param name="vec">�x�N�g��</param>
		/// <param name="normal">�@���x�N�g��</param>
		/// <returns>���˃x�N�g��</returns>
		static Vector3 Reflect(const Vector3& vec, const Vector3& normal);

		/// <summary>
		/// �X���C�h�x�N�g�� 
		/// </summary>
		/// <param name="vec">�x�N�g��</param>
		/// <param name="normal">�@���x�N�g��</param>
		/// <returns>�X���C�h�x�N�g��</returns>
		static Vector3 Slide(const Vector3& vec, const Vector3& normal);

		/// <summary>
		/// ���`��� 
		/// </summary>
		/// <param name="start">�J�n�l</param>
		/// <param name="end">�I���l</param>
		/// <param name="value">��Ԓl</param>
		/// <returns>��Ԃ��ꂽ�x�N�g��</returns>
		static Vector3 Lerp(const Vector3& start, const Vector3& end, float value);

		/// <summary>
		/// �G���~�[�g��� 
		/// </summary>
		/// <param name="startPos">�Ȑ��̎n�_�̈ʒu�x�N�g��</param>
		/// <param name="startTangent">�n�_�ł̐ڃx�N�g��</param>
		/// <param name="endPos">�Ȑ��̏I�_�̈ʒu�x�N�g��</param>
		/// <param name="endTangent">�I�_�ł̐ڃx�N�g��</param>
		/// <param name="value">�⊮�l(0�`1)</param>
		/// <returns>�⊮���ꂽ�x�N�g��</returns>
		static Vector3 Hermite(const Vector3& startPos, const Vector3& startTangent, const Vector3& endPos, const Vector3& endTangent, float value);

		/// <summary>
		/// �x�N�g�����s��ŕϊ� 
		/// </summary>
		/// <param name="vec">�ϊ�����x�N�g��</param>
		/// <param name="mat">�K�p����ϊ��s��</param>
		/// <returns>�ϊ���̃x�N�g��</returns>
		static Vector3 Transform(const Vector3& vec, const Math::Matrix& mat);

	public:
		float x, y, z;
	};
}