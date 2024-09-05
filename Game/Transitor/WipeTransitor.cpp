#include "WipeTransitor.h"
#include "../Application.h"
#include "DxLib.h"
#include <array>
#include <cassert>

// �R���X�g���N�^
WipeTransitor::WipeTransitor(TransitDirection dir, int interval) :
	TransitorBase(interval)
{
	// ���C�v��������ɂ���ăO���f�[�V�����̐F��ς���
	float left, right, top, bottom;
	left = right = top = bottom = 1.0f;

	// ���C�v��������ɂ͐F��t���Ȃ�
	switch (dir) 
	{
	case TransitDirection::UP:
		bottom = 0.0f;
		break;
	case TransitDirection::DOWN:
		top = 0.0f;
		break;
	case TransitDirection::LEFT:
		right = 0.0f;
		break;
	case TransitDirection::RIGHT:
		left = 0.0f;
		break;
	default:
		assert(false);
		break;
	}

	// �E�B���h�E�T�C�Y�̎擾
	const auto& size = Application::GetInstance()->GetWindowSize();

	// �J���[��ݒ�
	std::array<COLOR_U8, 4> colors{};	
	colors[0].r = colors[0].g = colors[0].b = colors[0].a = 255 * left * top;		// ����
	colors[1].r = colors[1].g = colors[1].b = colors[1].a = 255 * right * top;		// �E��
	colors[2].r = colors[2].g = colors[2].b = colors[2].a = 255 * left * bottom;	// ����
	colors[3].r = colors[3].g = colors[3].b = colors[3].a = 255 * right * bottom;	// �E��


	// ����̒��_�̏����Z�b�g( 1�|���S���ڂ̑�1���_ )
	std::array<VERTEX2D, 6> vertices{};
	vertices[0].pos.x = 0;
	vertices[0].pos.y = 0;
	vertices[0].pos.z = 0.0f;
	vertices[0].rhw = 1.0f;
	vertices[0].dif = colors[0];
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;

	// �E��̒��_�̏����Z�b�g( 1�|���S���ڂ̑�2���_ )
	vertices[1].pos.x = size.width;
	vertices[1].pos.y = 0;
	vertices[1].pos.z = 0.0f;
	vertices[1].rhw = 1.0f;
	vertices[1].dif = colors[1];
	vertices[1].u = 0.0f;
	vertices[1].v = 0.0f;

	// �����̒��_�̏����Z�b�g( 1�|���S���ڂ̑�3���_ )
	vertices[2].pos.x = 0;
	vertices[2].pos.y = size.height;
	vertices[2].pos.z = 0.0f;
	vertices[2].rhw = 1.0f;
	vertices[2].dif = colors[2];
	vertices[2].u = 0.0f;
	vertices[2].v = 0.0f;

	// �E���̒��_�̏����Z�b�g( 2�|���S���ڂ̑�1���_ )
	vertices[3].pos.x = size.width;
	vertices[3].pos.y = size.height;
	vertices[3].pos.z = 0.0f;
	vertices[3].rhw = 1.0f;
	vertices[3].dif = colors[3];
	vertices[3].u = 0.0f;
	vertices[3].v = 0.0f;

	// 2�|���S���ڂ̑�2���_�͍����̒��_�Ȃ̂ŃR�s�[
	vertices[4] = vertices[2];

	// 2�|���S���ڂ̑�3���_�͉E��̒��_�Ȃ̂ŃR�s�[
	vertices[5] = vertices[1];
	m_gradationH = MakeScreen(size.width, size.height, true);
	auto bkScrH = GetDrawScreen();

	//�����_�\�^�[�Q�b�g��ύX���A�O���f�[�V�����̕`��
	SetDrawScreen(m_gradationH);
	DrawPolygon2D(vertices.data(), 2, DX_NONE_GRAPH, true);

	//�O���f�[�V�����`������܂����ɖ߂�
	SetDrawScreen(bkScrH);
}

// �f�X�g���N�^
WipeTransitor::~WipeTransitor()
{
	// �O���f�[�V�����̃n���h�����폜
	DeleteGraph(m_gradationH);
}

// �X�V
void WipeTransitor::Update()
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
void WipeTransitor::Draw()
{
	// ���o���I�����Ă�����`�悵�Ȃ�
	if (IsEnd()) return;

	// �`���𗠉�ʂɕύX
	SetDrawScreen(DX_SCREEN_BACK);

	// �؂�ւ���̉�ʂ�`��
	DrawGraph(0, 0, m_nextScene, true);

	// �����̌v�Z
	auto rate = static_cast<float>(m_frame) / static_cast<float>(m_interval);

	// �؂�ւ��O�̉�ʂ��O���f�[�V�����Ńu�����h���ĕ`��
	DrawBlendGraph(0, 0, m_oldScene, true, m_gradationH, 255 * rate, 64);
}