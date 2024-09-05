#include "Meteor.h"
#include "../Model.h"
#include "../Game/Player.h"
#include "../Application.h"
#include "../Math/Vector2.h"
#include "../Math/MathUtil.h"
#include "../ModelHandleManager.h"
#include "../DataType/PlayerParamDataType.h"
#include <DxLib.h>

namespace
{
	// ���f���̊g�嗦
	constexpr float normal_meteor_max_scale = 5.0f;	// �ő�g�嗦
	constexpr float normal_meteor_min_scale = 1.0f;	// �ŏ��g�嗦
	constexpr float small_meteor_max_scale = 10.0f;	// �ő�g�嗦
	constexpr float small_meteor_min_scale = 5.0f;	// �ŏ��g�嗦

	// ���f���̉�]���x
	constexpr float max_delta_rot = 1.0f;  // �ő�
	constexpr float min_delta_rot = 0.0f;  // �ŏ�

	// �ړ����x
	constexpr float meteor_move_speed = 10.0f;
	constexpr float small_meteor_move_speed = 2.7f;

	// �X�N���[�����W���烏�[���h���W�ɕϊ��Ɏg�p����Z���W�̐��`
	constexpr float z_linear = 1.0f;

	// �v���C���[�̍��W��0�Ƃ����Ƃ���覐΂�Z���W
	constexpr float meteor_z = 5000.0f;
}

// �R���X�g���N�^
// �����_���ɐ����A�v���C���[�̕����Ɉړ�
Meteor::Meteor(const MeteorType type, const std::shared_ptr<Player>& pPlayer, const float playerRelativePos) :
	m_rot({ 0, 0, 0 }),
	m_isEnabled(true),
	m_moveVec({ 0, 0, 0 }),
	m_rotVec({ 0, 0, 0 }),
	m_type(type),
	m_opacity(1.0f),
	m_pPlayer(pPlayer)
{
	// ���f���̃C���X�^���X�̍쐬
	m_pModel = std::make_unique<Model>(ModelHandleManager::GetInstance().GetHandle("Meteor"));

	// 1�t���[���ɉ�]����ʂ�0�x����`1�x�̊Ԃ���擾
	m_rotVec.x = Math::Util::ToRadian(Math::Util::GetRandFloat(min_delta_rot, max_delta_rot));
	m_rotVec.y = Math::Util::ToRadian(Math::Util::GetRandFloat(min_delta_rot, max_delta_rot));
	m_rotVec.z = Math::Util::ToRadian(Math::Util::GetRandFloat(min_delta_rot, max_delta_rot));

	// 覐΂̎�ނɂ���ď�����ς���
	// ������覐�
	if (type == MeteorType::SMALL)
	{
		// 覐΂̊g�嗦�������_���ɐݒ�
		float scale = Math::Util::GetRandFloat(small_meteor_min_scale, small_meteor_max_scale);
		m_scale = { scale, scale, scale };
	}
	// �ʏ��覐�
	else if (type == MeteorType::NORMAL)
	{
		// 覐΂̊g�嗦�������_���ɐݒ�
		float scale = Math::Util::GetRandFloat(normal_meteor_min_scale, normal_meteor_max_scale);
		m_scale = { scale, scale, scale };

		// �v���C���[�Ɍ������x�N�g�����擾
		Math::Vector3 distance = m_pPlayer->GetPos() - m_pos;
		distance.Normalize();
		distance *= meteor_move_speed;
		m_moveVec = distance;
	}

	m_pos.x = m_pos.y = 0.0f;
	m_pos.z = m_pPlayer->GetPos().z + playerRelativePos;

	// ��ʓ��Ƀ����_���ɐ���
	Math::Vector3 screenPos{};
	screenPos.x = Math::Util::GetRandFloat(0.0f, Application::GetInstance()->GetWindowSize().width);
	screenPos.y = Math::Util::GetRandFloat(0.0f, Application::GetInstance()->GetWindowSize().height);
	screenPos.z = z_linear;
	Math::Vector3 targetPos = Math::Vector3::FromDxLibVector3(ConvScreenPosToWorldPos_ZLinear(screenPos.ToDxLibVector3()));

	// �x�N�g���̍쐬
	m_moveVec = (targetPos - m_pos).Normalized() * small_meteor_move_speed;

	// ���f���̐ݒ�
	m_pModel->SetOpacity(m_opacity);	// �����x
	m_pModel->SetPos(m_pos);			// �ʒu
	m_pModel->SetRot(m_rot);			// ��]
	m_pModel->SetScale(m_scale);		// �g�嗦
	m_pModel->Update();					// �X�V
}

// �R���X�g���N�^
// �z�u�f�[�^�ʂ�ɐ����A�ړ����Ȃ�
Meteor::Meteor(const UnityGameObject& data, const std::shared_ptr<Player>& pPlayer):
	m_pos(data.pos),
	m_rot(data.rot),
	m_scale(data.scale),
	m_isEnabled(true),
	m_moveVec({ 0, 0, 0 }),
	m_rotVec({ 0, 0, 0 }),
	m_type(MeteorType::NORMAL),
	m_opacity(1.0f),
	m_pPlayer(pPlayer)
{
	// ���f���̃C���X�^���X�̍쐬
	m_pModel = std::make_unique<Model>(ModelHandleManager::GetInstance().GetHandle("Meteor"));

	// 1�t���[���ɉ�]����ʂ������_���Ɏ擾
	m_rotVec.x = Math::Util::ToRadian(Math::Util::GetRandFloat(min_delta_rot, max_delta_rot));
	m_rotVec.y = Math::Util::ToRadian(Math::Util::GetRandFloat(min_delta_rot, max_delta_rot));
	m_rotVec.z = Math::Util::ToRadian(Math::Util::GetRandFloat(min_delta_rot, max_delta_rot));

	// ���f���̐ݒ�
	m_pModel->SetOpacity(m_opacity);	// �����x
	m_pModel->SetPos(m_pos);			// �ʒu
	m_pModel->SetRot(m_rot);			// ��]
	m_pModel->SetScale(m_scale);		// �g�嗦
	m_pModel->Update();					// �X�V
}

// �f�X�g���N�^
Meteor::~Meteor()
{
}

// �X�V
void Meteor::Update(const Math::Vector3& cameraPos)
{
	// �J���������ɍs������
	if (cameraPos.z > m_pos.z)
	{
		// ����
		m_isEnabled = false;
	}

	// �ړ�
	m_pos += m_moveVec;

	// ��]
	m_rot += m_rotVec;

	// ���f���̐ݒ�
	m_pModel->SetOpacity(m_opacity);	// �����x
	m_pModel->SetPos(m_pos);			// �ʒu
	m_pModel->SetRot(m_rot);			// ��]
	m_pModel->Update();					// �X�V
}

// �X�^�[�g���o���̍X�V
void Meteor::UpdateStart()
{
	// �ړ�
	m_pos.z += (m_pPlayer->GetMoveVec().z - 
				m_pPlayer->GetParameter(DataType::PlayerParamType::MOVE_SPEED_Z));

	// ��]
	m_rot += m_rotVec;

	// ���f���̐ݒ�
	m_pModel->SetPos(m_pos);	// �ʒu
	m_pModel->SetRot(m_rot);	// ��]
	m_pModel->Update();			// �X�V
}

// �`��
void Meteor::Draw()
{
	m_pModel->Draw();
}

// ���݃t���O�̎擾
bool Meteor::IsEnabled() const
{
	return m_isEnabled;
}

// ���f���̃n���h���̎擾
int Meteor::GetModelHandle() const
{
	return m_pModel->GetModelHandle();
}

// 覐΂̎�ނ̎擾
MeteorType Meteor::GetType() const
{
	return m_type;
}