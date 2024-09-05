#include "IrisTransitor.h"
#include "../Application.h"
#include "DxLib.h"
#include <cmath>

// �R���X�g���N�^
IrisTransitor::IrisTransitor(bool irisOut, int interval, bool isTiled, int gHandle) :
	TransitorBase(interval),
	m_irisOut(irisOut),
	m_isTiled(isTiled),
	m_handle(gHandle)
{
	// �E�B���h�E�T�C�Y�̎擾
	const auto& size = Application::GetInstance()->GetWindowSize();

	// �}�X�N�X�N���[���̍쐬
	m_handleForMaskScreen = MakeScreen(size.width, size.height, true);
	m_maskH = CreateMaskScreen();

	// �Ίp���̒������v�Z
	m_diagonalLength = std::hypotf(size.width, size.height) / 2.0f;
}

// �f�X�g���N�^
IrisTransitor::~IrisTransitor()
{
	// �}�X�N�X�N���[���̍폜
	DeleteMaskScreen();
}

// �X�V
void IrisTransitor::Update()
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
void IrisTransitor::Draw()
{
	// ���o���I�����Ă�����`�悵�Ȃ�
	if (IsEnd()) return;

	// �E�B���h�E�T�C�Y�̎擾
	const auto& size = Application::GetInstance()->GetWindowSize();

	// �������v�Z
	auto rate = static_cast<float>(m_frame) / static_cast<float>(m_interval);

	// �n���h���̕ۑ�
	int backScene = m_oldScene;
	int maskedScene = m_nextScene;

	// �A�C���X�A�E�g�̏ꍇ�͋t�ɂ���
	if (m_irisOut) 
	{
		backScene = m_nextScene;
		maskedScene = m_oldScene;
		rate = 1.0f - rate;
	}

	// �~�̔��a���v�Z
	float radius = (m_diagonalLength) * rate;

	// �}�X�N�X�N���[����`���ɂ���
	SetDrawScreen(m_handleForMaskScreen);

	// ��ʂ̃N���A
	ClearDrawScreen();

	// �摜�n���h�����Ȃ��ꍇ�͉~��`��
	if (m_handle == -1)
	{
		// �^�C����ɕ`�悷��ꍇ
		if (m_isTiled) 
		{
			// �^�C���̐����v�Z
			int xDiv = (size.width / 50) + 1;
			int yDiv = (size.width / 50) + 1;

			// �~�̔��a���v�Z
			radius = (50 * m_diagonalLength / 320) * rate;

			// �^�C����ɉ~��`��
			for (int y = 0; y < xDiv; y++) 
			{
				for (int x = 0; x < xDiv; x++) 
				{
					DrawCircleAA(50 + x * 100, 50 + y * 100, radius, 32, 0xffffff, true);
				}
			}
		}
		else 
		{
			// �~��`��
			DrawCircleAA(size.width / 2.0f, size.height / 2.0f, radius, 32, 0xffffff, true);
		}
	}
	else 
	{
		// �摜�n���h��������ꍇ�͉摜��`��
		DrawRotaGraphFast(size.width / 2.0f, size.height / 2.0f, rate * 3.5f, rate * 2.0f, m_handle, true);
	}

	// �B���֐�(���݂̃O���t�B�b�N�n���h�����}�X�N�X�N���[���ɓ]��)
	SetMaskScreenGraph(m_handleForMaskScreen);

	// �`��̈�𔽓]����
	SetMaskReverseEffectFlag(true);

	// �`���𗠉�ʂɐ؂�ւ� 
	SetDrawScreen(DX_SCREEN_BACK);

	// �}�X�N��K�p
	SetUseMaskScreenFlag(false);

	// �؂�ւ��O��ʂ�`��
	DrawGraph(0, 0, backScene, true);

	// �}�X�N��K�p
	SetUseMaskScreenFlag(true);

	// �؂�ւ����ʂ�`��
	DrawGraph(0, 0, maskedScene, true);

	// �}�X�N��K�p���Ȃ�
	SetUseMaskScreenFlag(false);
}