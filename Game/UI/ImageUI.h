#pragma once
#include "UIBase.h"
#include "../Math/Vector2.h"
#include <string>

/// <summary>
/// UI�̉摜�N���X
/// </summary>
class ImageUI : public UIBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="filePath">�摜�t�@�C���̃p�X</param>
	ImageUI(const std::string& filePath);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ImageUI();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override final {};

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override final;


	//// �Z�b�^�[ ////

	/// <summary>
	/// �ʒu�̐ݒ�
	/// </summary>
	/// <param name="pos">�ʒu</param>
	void SetPos(const Math::Vector2& pos);

	/// <summary>
	/// ��]�̐ݒ�
	/// </summary>
	/// <param name="rot">��]</param>
	void SetRot(const float rot);		

	/// <summary>
	/// ���S���W�̐ݒ�
	/// </summary>
	/// <param name="center">���S���W</param>
	void SetCenter(const Math::Vector2& center);

	/// <summary>
	/// �g�嗦�̐ݒ�
	/// </summary>
	/// <param name="scale">�g�嗦</param>
	void SetScale(const Math::Vector2& scale);

	/// <summary>
	/// �����x�̐ݒ�
	/// </summary>
	/// <param name="alpha">�����x</param>
	void SetAlpha(const float alpha);		


	//// �Q�b�^�[ ////

	/// <summary>
	/// �ʒu�̎擾
	/// </summary>
	/// <returns>�ʒu</returns>
	const Math::Vector2& GetPos() const;		

	/// <summary>
	/// ��]�̎擾
	/// </summary>
	/// <returns>��]</returns>
	float GetRot() const;		

	/// <summary>
	/// ���S���W�̎擾
	/// </summary>
	/// <returns>���S���W</returns>
	const Math::Vector2& GetCenter() const;	

	/// <summary>
	/// �g�嗦�̎擾
	/// </summary>
	/// <returns>�g�嗦</returns>
	const Math::Vector2& GetScale() const;

	/// <summary>
	/// �����x�̎擾
	/// </summary>
	/// <returns>�����x</returns>
	float GetAlpha() const;		

private:
	// �摜�n���h��
	int m_imgHandle;

	// �摜�̉�]��
	float m_rot;

	// �摜�̒��S���W
	Math::Vector2 m_center;

	// �摜�̊g�嗦
	Math::Vector2 m_scale;

	// �摜�̓����x
	float m_alpha;
};