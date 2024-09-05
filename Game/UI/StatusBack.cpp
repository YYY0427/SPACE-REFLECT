#include "StatusBack.h"
#include "ImageUI.h"
#include "../Math/Vector2.h"
#include <string>

namespace
{
	// �摜�̃t�@�C���p�X
	const std::string image_file_path = "Data/Image/PlayerStatusBased.png";

	// �ŏI�g�嗦
	constexpr float final_scale = 0.65f;

	// �X�^�[�g���o�̊g�呬�x
	constexpr float start_scale_speed = 0.05f;

	// �����ʒu
	const Math::Vector2 init_pos = { 30, 550 };
}

// �R���X�g���N�^
StatusBack::StatusBack() :
	m_isStartAnimation(false)
{
	// ������
	m_pos = init_pos;

	// �摜�̓ǂݍ���
	m_pImageUI = std::make_unique<ImageUI>(image_file_path);

	// �摜�̐ݒ�
	m_pImageUI->SetPos(m_pos);			// �ʒu
	m_pImageUI->SetScale({ 0, 0 });			// �g�嗦
}

// �f�X�g���N�^
StatusBack::~StatusBack()
{
	
}

// �X�V
void StatusBack::Update()
{
	// �ʒu�̍X�V
	m_pImageUI->SetPos(m_pos);
}

// �X�^�[�g���o�̍X�V
void StatusBack::UpdateStartAnimation()
{
	// �X�^�[�g���o�̊g��
	// X���̊g��
	if (m_pImageUI->GetScale().x < final_scale)
	{
		m_pImageUI->SetScale({ m_pImageUI->GetScale().x + start_scale_speed, 0.0f });
	}
	// Y���̊g��
	else if (m_pImageUI->GetScale().x >= final_scale && m_pImageUI->GetScale().y < final_scale)
	{
		m_pImageUI->SetScale({ final_scale, m_pImageUI->GetScale().y + start_scale_speed });
	}
	// �g��I��
	else
	{
		m_pImageUI->SetScale({final_scale, final_scale });
		m_isStartAnimation = true;
	}
}

// �`��
void StatusBack::Draw()
{
	// �摜�̕`��
	m_pImageUI->Draw();
}

// �X�^�[�g���o�t���O�̎擾
bool StatusBack::IsStartAnimation() const
{
	return m_isStartAnimation;
}
