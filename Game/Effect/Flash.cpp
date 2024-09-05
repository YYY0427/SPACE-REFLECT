#include "Flash.h"
#include "../Application.h"
#include <DxLib.h>
#include <algorithm>

namespace
{
	// �g�呬�x
	constexpr float scale_speed = 100.0f;

	// �����x�̌������x
	constexpr int alpha_speed = 10;

	// �t���b�V���̏����F
	constexpr unsigned int default_color = 0xffffff;

	//�t���b�V���̏��������x(0�`255)
	constexpr int default_alpha = 255;
}

namespace Effect
{
	// �R���X�g���N�^
	Flash::Flash(const int enableFrame) :
		m_enableFrame(enableFrame),
		m_color(default_color),
		m_alpha(default_alpha)
	{
	}

	// �f�X�g���N�^
	Flash::~Flash()
	{
	}

	// �X�V
	void Flash::Update(const unsigned int color)
	{
		// �F��ݒ�
		m_color = color;

		// ���t���[���o�߂����瓧���x��������
		if (m_enableFrame-- <= 0)
		{
			// �����x��������
			m_alpha -= alpha_speed;
			m_alpha = (std::max)(m_alpha, 0);
		}
	}

	// �`��
	void Flash::Draw()
	{
		// �����x��K�p���Ċۂ�`��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
		auto& screenSize = Application::GetInstance()->GetWindowSize();
		DrawBox(0, 0, screenSize.width, screenSize.height, m_color, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// �I��������
	bool Flash::IsEnd() const
	{
		return m_alpha <= 0;
	}
}