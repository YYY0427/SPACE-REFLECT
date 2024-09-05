#include "ImageUI.h"
#include "../Util/DrawFunctions.h"
#include <DxLib.h>

// �R���X�g���N�^
ImageUI::ImageUI(const std::string& filePath) :
	m_rot(0.0f),
	m_scale(1.0f, 1.0f),
	m_alpha(255),
	m_center(0.0f, 0.0f)
{
	// �摜�̓ǂݍ���
	m_imgHandle = my::MyLoadGraph(filePath.c_str());
}

// �f�X�g���N�^
ImageUI::~ImageUI()
{
	// �摜�̍폜
	DeleteGraph(m_imgHandle);
}

// �`��
void ImageUI::Draw()
{
	// �A���t�@�u�����h�̐ݒ�
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);

	// �摜�̕`��
	DrawRotaGraph3(
		m_pos.x, m_pos.y,		// �`�悷����W
		m_center.x, m_center.y, // �摜�̒��S���W
		m_scale.x, m_scale.y,	// �g�嗦
		m_rot,					// ��]�p�x
		m_imgHandle,			// �摜�n���h��
		true);

	// �`��ݒ�����ɖ߂�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// �ʒu�̐ݒ�
void ImageUI::SetPos(const Math::Vector2& pos)
{
	m_pos = pos;
}

// ��]�̐ݒ�
void ImageUI::SetRot(const float rot)
{
	m_rot = rot;
}

// ���S���W�̐ݒ�
void ImageUI::SetCenter(const Math::Vector2& center)
{
	m_center = center;
}

// �g�嗦�̐ݒ�
void ImageUI::SetScale(const Math::Vector2& scale)
{
	m_scale = scale;
}

// �����x�̐ݒ�
void ImageUI::SetAlpha(const float alpha)
{
	m_alpha = alpha;
}

// �ʒu�̎擾
const Math::Vector2& ImageUI::GetPos() const
{
	return m_pos;
}

// ��]�̎擾
float ImageUI::GetRot() const
{
	return m_rot;
}

// ���S���W�̎擾
const Math::Vector2& ImageUI::GetCenter() const
{
	return m_center;
}

// �g�嗦�̎擾
const Math::Vector2& ImageUI::GetScale() const
{
	return m_scale;
}

// �����x�̎擾
float ImageUI::GetAlpha() const
{
	return m_alpha;
}