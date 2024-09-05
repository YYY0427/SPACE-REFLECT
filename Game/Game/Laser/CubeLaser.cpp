#include "CubeLaser.h"
#include "../Player.h"
#include "../Camera.h"
#include "../../Application.h"
#include "../../Model.h"
#include "../../Math/MathUtil.h"
#include "../../ModelHandleManager.h"
#include <DxLib.h>
#include <cassert>

namespace
{
	// �g�呬�x
	constexpr float scale_speed = 0.00025f;

	// �s�����x�̏㏸���x
	constexpr float opacity_speed = 0.005f;

	// �s�����x�̍ő�l
	constexpr float max_opacity = 1.0f;

	// 1�t���[���ɉ�]����p�x
	const float max_delta_rot = 1.0f;  // �ő�
	const float min_delta_rot = 0.1f;  // �ŏ�

	// ���[�U�[�̊g�嗦
	constexpr float max_scale = 5.0f;	// �ő�
	constexpr float min_scale = 1.0f;	// �ŏ�

	// �X�N���[���T�C�Y
	const auto& screenSize = Application::GetInstance()->GetWindowSize();

	// ���f���̈ʒu���������炷�͈�
	constexpr float model_pos_range = 5.0f;

	// ���f���̕s�����x�������_���ŕς���͈�
	constexpr float model_max_opacity_range = 0.7f;
	constexpr float model_min_opacity_range = 0.0f;

	// �K�E�X�̎g�p�s�N�Z����(8, 16, 32 �̉��ꂩ)
	constexpr int gauss_pixel_width = 8;

	// �K�E�X�̂ڂ����p�����[�^(100 �Ŗ�1�s�N�Z�����̕�)
	constexpr int gauss_param = 200;

	// �����f����`�悷�邩
	constexpr int model_num = 10;

	// ���[�U�[�̒��S�̐F
	const COLOR_F laser_center_color = GetColorF(255, 255, 255, 255);

	// ���[�U�[�̒��S�̐F�̕s�����x
	constexpr float laser_center_opacity = 1.0f;

	// ���Z�����̉�
	constexpr int add_blend_num = 2;

	// ���Z�����̊���
	constexpr int add_blend_rate = 255;
}

// �R���X�g���N�^
CubeLaser::CubeLaser(const Math::Vector3& firePos,
				     const float laserSpeed, 
					 const std::shared_ptr<Player>& pPlayer,
					 const std::shared_ptr<Camera>& pCamera)
{
	// ������
	m_pos     = firePos;
	m_pPlayer = pPlayer;
	m_pCamera = pCamera;
	m_opacity = 0.0f;	
	m_moveVec = (m_pPlayer->GetPos() - m_pos).Normalized() * laserSpeed;

	// 1�t���[���ɉ�]����ʂ������_���Ŏ擾
	m_deltaRot.x = Math::Util::ToRadian(Math::Util::GetRandFloat(min_delta_rot, max_delta_rot));
	m_deltaRot.y = Math::Util::ToRadian(Math::Util::GetRandFloat(min_delta_rot, max_delta_rot));
	m_deltaRot.z = Math::Util::ToRadian(Math::Util::GetRandFloat(min_delta_rot, max_delta_rot));

	// �ő�g�嗦�������_���Ŏ擾
	m_maxScale.x = Math::Util::ToRadian(Math::Util::GetRandFloat(min_scale, max_scale));
	m_maxScale.y = Math::Util::ToRadian(Math::Util::GetRandFloat(min_scale, max_scale));
	m_maxScale.z = Math::Util::ToRadian(Math::Util::GetRandFloat(min_scale, max_scale));

	// �K�E�X�p�X�N���[���̍쐬
	m_gaussScreen = MakeScreen(screenSize.width, screenSize.height);

	// ���f���ݒ�
	m_pModel = std::make_unique<Model>(ModelHandleManager::GetInstance().GetHandle("CubeLaser"));
	m_pModel->SetUseCollision(true);	// �����蔻����g�p����
	m_pModel->SetOpacity(m_opacity);	// �s�����x
	m_pModel->SetRot(m_rot);			// ��]���
	m_pModel->SetScale(m_scale);		// �g�嗦
	m_pModel->SetPos(m_pos);			// �ʒu���
	m_pModel->Update();					// �����蔻��̍X�V
}

// �f�X�g���N�^
CubeLaser::~CubeLaser()
{
	// �K�E�X�p�X�N���[���̍폜
	DeleteGraph(m_gaussScreen);
}

// �X�V
void CubeLaser::Update()
{
	// �s�����x�����Z
	m_opacity = (std::min)(m_opacity + opacity_speed, max_opacity);

	// �ړ�
	m_pos.z += m_pPlayer->GetMoveVec().z;
	m_pos   += m_moveVec;

	// �g��
	m_scale += scale_speed;

	// �g�嗦�̐���
	m_scale.x = (std::min)(m_scale.x, m_maxScale.x);
	m_scale.y = (std::min)(m_scale.y, m_maxScale.y);
	m_scale.z = (std::min)(m_scale.z, m_maxScale.z);

	// ��]
	m_rot += m_deltaRot;

	// ���f���ݒ�
	m_pModel->SetOpacity(m_opacity);	// �s�����x
	m_pModel->SetRot(m_rot);			// ��]���
	m_pModel->SetScale(m_scale);		// �g�嗦
	m_pModel->SetPos(m_pos);			// �ʒu���
	m_pModel->Update();					// �����蔻��̍X�V
}

// �`��
void CubeLaser::Draw()
{
	// �`��X�N���[�����擾���ĕۑ����Ă���
	int screen = GetDrawScreen();

	// �K�E�X�p�X�N���[���ɕ`��
	SetDrawScreen(m_gaussScreen);

	// ��ʂ��N���A
	ClearDrawScreen();

	for (int i = 0; i < model_num; i++)
	{
		// ���f���̈ʒu���������炵�ĕ`��
		Math::Vector3 pos;
		pos.x = m_pos.x + Math::Util::GetRandFloat(-model_pos_range, model_pos_range);
		pos.y = m_pos.y + Math::Util::GetRandFloat(-model_pos_range, model_pos_range);
		pos.z = m_pos.z + Math::Util::GetRandFloat(-model_pos_range, model_pos_range);
		m_pModel->SetPos(pos);

		// �s�����x�������ς��ĕ`��
		float opacity = Math::Util::GetRandFloat(model_min_opacity_range, model_max_opacity_range);
		m_pModel->SetOpacity(opacity);

		// ���f����`��
		m_pModel->Draw();

		// ���Ƃɖ߂�
		m_pModel->SetPos(m_pos);
		m_pModel->SetOpacity(m_opacity);
	}
	// �K�E�X�t�B���^�[�̓K�p
	GraphFilter(m_gaussScreen, DX_GRAPH_FILTER_GAUSS, gauss_pixel_width, gauss_param);

	// ���[�U�[�̔��������̕`��
	m_pModel->SetAllMaterialDifColor(laser_center_color);
	m_pModel->SetOpacity(laser_center_opacity);
	m_pModel->Draw();

	// ���Ƃɖ߂�
	m_pModel->RestoreAllMaterialDifColor();
	m_pModel->SetOpacity(m_opacity);

	// �`��X�N���[����߂�
	SetDrawScreen(screen);

	// ���Z����
	SetDrawBlendMode(DX_BLENDMODE_ADD, add_blend_rate);
	for (int i = 0; i < add_blend_num; i++)
	{
		// �������쐬�����K�E�X�摜��`��
		DrawGraph(0, 0, m_gaussScreen, true);
	}
	// ���Z����������
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}