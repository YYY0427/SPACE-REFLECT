#include "DamageFlash.h"
#include "../Application.h"
#include <DxLib.h>

// �R���X�g���N�^
DamageFlash::DamageFlash() :
	m_frame(0),
	m_color(0xffffff),
	m_alpha(0),
	m_speed(0.0f)
{

}

// �f�X�g���N�^
DamageFlash::~DamageFlash()
{
}

// �X�V
void DamageFlash::Update()
{
	// �t���[����0�ȉ��Ȃ珈�����s��Ȃ�
	if (m_frame <= 0) return;

	// �A���t�@�l�����炷
	m_alpha -= m_speed;

	// �A���t�@�l��0�ȉ��ɂȂ�����0�ɂ���
	if (m_alpha <= 0)
	{
		m_alpha = 0;
		m_frame = 0;
	}
}

// �`��
void DamageFlash::Draw()
{
	// ��ʂ̃T�C�Y���擾
	auto& screenSize = Application::GetInstance()->GetWindowSize();

	// �F�̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
	DrawBox(0, 0, screenSize.width, screenSize.height, m_color, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// �t���b�V���̊J�n
void DamageFlash::Start(const int frame, const int alpha, const unsigned int color)
{
	// �l�̐ݒ�
	m_frame = frame;
	m_alpha = alpha;
	m_color = color;

	// ���x�̌v�Z
	m_speed = static_cast<float>(alpha) / static_cast<float>(frame);
}
