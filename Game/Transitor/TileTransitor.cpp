#include "TileTransitor.h"
#include "../Application.h"
#include "DxLib.h"

// �R���X�g���N�^
TileTransitor::TileTransitor(int cellSize, int interval) :
	TransitorBase(interval),
	m_cellSize(cellSize)
{
	// �E�B���h�E�T�C�Y�̎擾
	const auto& size = Application::GetInstance()->GetWindowSize();

	// �Z���̐����v�Z
	int xNum = (size.width / m_cellSize) + 1;
	int yNum = (size.height / m_cellSize) + 1;

	// �Z���̐����̃��������m��
	m_tiles.reserve(xNum * yNum);

	// �z��ɃZ���̃C���f�b�N�X���i�[
	for (int yIdx = 0; yIdx < yNum; yIdx++)
	{
		for (int xIdx = 0; xIdx < xNum; xIdx++)
		{
			m_tiles.push_back({xIdx, yIdx});
		}
	}
}

// �X�V
void TileTransitor::Update()
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

	// ���o���I�����Ă�����X�V���Ȃ�
	if(IsEnd()) return;

	// �z��̕��т��V���b�t��
	std::shuffle(m_tiles.begin(), m_tiles.end(), m_mt);

	// �E�B���h�E�T�C�Y�̎擾
	const auto& size = Application::GetInstance()->GetWindowSize();

	// �Z���̐����v�Z
	int xNum = (size.width / m_cellSize) + 1;
	int yNum = (size.height / m_cellSize) + 1;

	// �����Z���̐����v�Z
	int eraseNum = ((xNum * yNum) / m_interval);

	// �����Z���̐����z��̃T�C�Y���傫���ꍇ
	if (m_tiles.size() > eraseNum)
	{
		// �z��̖�������eraseNum�������폜
		m_tiles.erase(m_tiles.end() - eraseNum, m_tiles.end());
	}
	else
	{
		// �z����N���A
		m_tiles.clear();
	}
}

void TileTransitor::Draw()
{
	// ���o���I�����Ă�����`�悵�Ȃ�
	if(IsEnd()) return;

	// �`���𗠉�ʂɕύX
	SetDrawScreen(DX_SCREEN_BACK);

	// �E�B���h�E�T�C�Y�̎擾
	const auto& size = Application::GetInstance()->GetWindowSize();

	// �؂�ւ���̉�ʂ�`��
	DrawRectGraph(0, 0, 0, 0, size.width, size.height, m_nextScene, true);

	// �؂�ւ��O�̉�ʂ��^�C���ŕ`��
	for (const auto& tile : m_tiles)
	{
		DrawRectGraph(
			tile.xIdx * m_cellSize, 
			tile.yIdx * m_cellSize,
			tile.xIdx * m_cellSize, 
			tile.yIdx * m_cellSize, 
			m_cellSize, m_cellSize, 
			m_oldScene, true);
	}
}
