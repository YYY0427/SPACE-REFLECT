#pragma once
#include <DxLib.h>
#include <string>
#include <array>
#include "Math/Vector3.h"

/// <summary>
/// 3D�摜�N���X
/// </summary>
class Image3D
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="filePath">�摜�̃t�@�C���p�X</param>
	Image3D(const std::string& filePath);

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="imgHandle">�摜�n���h��</param>
	Image3D(const int imgHandle);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Image3D();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �摜�n���h���̎擾 
	/// </summary>
	/// <returns>�摜�n���h��</returns>
	int GetImageHandle() const;

	/// <summary>
	/// �摜�̒��_���̎擾 
	/// </summary>
	/// <returns>�摜�̒��_���</returns>
	const std::array<VERTEX3D, 6>& GetVertex() const;

	/// <summary>
	/// �ʒu�̐ݒ� 
	/// </summary>
	/// <param name="pos">�ʒu</param>
	void SetPos(const Math::Vector3& pos);

	/// <summary>
	/// ��]�̐ݒ�
	/// </summary>
	/// <param name="rot">��]</param>
	void SetRot(const Math::Vector3& rot);

	/// <summary>
	/// �摜�̉����̐ݒ� 
	/// </summary>
	/// <param name="width">����</param>
	void SetImgWidth(const float width);

	/// <summary>
	/// �摜�̏c���̐ݒ� 
	/// </summary>
	///	<param name="height">�c��</param>
	void SetImgHeight(const float height);

	/// <summary>
	/// �摜�̓����x�̐ݒ� 
	/// </summary>
	/// <param name="alpha">�����x</param>
	void SetAlpha(const int alpha);

	/// <summary>
	/// �摜�̉����Ɨ����̊g�嗦�����̉摜�̃T�C�Y�ɖ߂� 
	/// </summary>
	void SetImgDafualtScale();

private:
	// �摜�̒��_
	std::array<VERTEX3D, 6> m_vertex;

	// �摜�̈ʒu
	Math::Vector3 m_pos;

	// �摜�̉�]
	Math::Vector3 m_rot;

	// �摜�̓����x
	int m_alpha;

	// �摜�̃n���h��
	int m_imgHandle;

	// �摜�̉���
	float m_imgWidth;

	// �摜�̏c��
	float m_imgHeight;
};