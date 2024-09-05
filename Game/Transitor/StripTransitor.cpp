#include "StripTransitor.h"
#include "../Application.h"
#include "DxLib.h"

// �R���X�g���N�^
StripTransitor::StripTransitor(int width, int interval) :
	TransitorBase(interval),
	m_width(width)
{
}

// �f�X�g���N�^
StripTransitor::~StripTransitor()
{
}

// �X�V
void StripTransitor::Update()
{
	// ���o���ԓ��Ȃ�
	if (m_frame < m_interval) 
	{
		// �t���[����i�߂�
		m_frame++;

		// �`����؂�ւ���ɕύX
		SetDrawScreen(m_nextScene);
	}
	else if (m_frame == m_interval) 
	{
		// �`���𗠉�ʂɕύX
		SetDrawScreen(DX_SCREEN_BACK);
	}
}

// �`��
void StripTransitor::Draw()
{
	// ���o���I�����Ă�����`�悵�Ȃ�
	if (IsEnd()) return;

	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	// �E�B���h�E�T�C�Y�̎擾
	const auto& size = Application::GetInstance()->GetWindowSize();

	// �������v�Z
	auto rate = static_cast<float>(m_frame) / static_cast<float>(m_interval);

	// �Z���̐����v�Z
	int lp = (size.width / m_width) + 1;

	// �؂�ւ��O�̉�ʂ�`��
	DrawRectGraph(0, 0, 0, 0, size.width, size.height, m_oldScene, true);

	// �؂�ւ���̉�ʂ�Z����ɕ`��
	for (int i = 0; i < lp; i++)
	{
		if (i % 2 == 0) 
		{
			DrawRectGraph(i * m_width, size.height * (rate - 1.0f), i * m_width, 0, m_width, size.height, m_nextScene, true);
		}
		else 
		{
			DrawRectGraph(i * m_width, size.height * (1.0f - rate), i * m_width, 0, m_width, size.height, m_nextScene, true);
		}
	}
}