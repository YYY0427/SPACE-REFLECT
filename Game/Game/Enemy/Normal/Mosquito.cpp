#include "Mosquito.h"
#include "../../../Model.h"
#include "../../Player.h"
#include "../../../ModelHandleManager.h"
#include <DxLib.h>

namespace
{
	// �A�j���[�V�����ԍ�
	constexpr int idle_anim_num = 1;	// �ҋ@

	// ���[�U�[�̔��ˈʒu�̃t���[��
	constexpr int laser_fire_frame_pos = 50;

	// �ړI�n�ɓ��B�������ǂ����̔���
	// �����臒l�i�K�؂Ȓl�ɒ�������K�v�j
	constexpr float distance_threshold = 50.0f;  

	// ���f���̏����̌����Ă������
	const Vector3 init_model_direction = { 0, 0, -1 };

	// �����蔻��̔��a
	constexpr float hit_radius = 70.0f;

	// �s�����x�̍ő�l
	constexpr float max_opacity = 1.0f;

	// �A�j���[�V�����̕ύX�t���[��
	constexpr int change_anim_frame = 8;

	// �֊s��
	constexpr float outline_width   = 1.08f;
}

// �R���X�g���N�^
Mosquito::Mosquito(const EnemyData& data, 
				   const std::shared_ptr<Player>& pPlayer, 
				   const std::shared_ptr<LaserManager>& pLaserManager) :
	m_idleFrame(0),
	m_laserKey(-1)
{
	// ������
	m_pPlayer         = pPlayer;
	m_pLaserManager   = pLaserManager;
	m_actionDataList  = data.actionDataList;
	m_isEnabled       = true;
	m_hp              = data.hp;
	m_moveSpeed       = data.speed;
	m_attackPower     = data.attack;
	m_scale           = { data.scale, data.scale, data.scale };
	m_opacity         = max_opacity;
	m_collisionRadius = hit_radius * m_scale.x;

	
	// 0.0(near)�`1.0(far)�͈̔͂�Z���W��ݒ�
	float z = (fabs(GetCameraPosition().z - m_pPlayer->GetPos().z) + data.pos.z) / GetCameraFar();

	// ���W�̐ݒ�
	m_pos = Vector3::FromDxLibVector3(ConvScreenPosToWorldPos_ZLinear({ data.pos.x, data.pos.y, z }));

	// �v���C���[�������悤�ɉ�]�s���ݒ�
	auto rotMtx = Math::Matrix::GetRotationMatrix(init_model_direction, (m_pPlayer->GetPos() - m_pos).Normalized());

	// �X�e�[�g�}�V���̐ݒ�
	m_state.AddState(State::IDLE, {}, [this](){ UpdateIdle(); }, {});
	m_state.AddState(State::MOVE, [this]() { EntarMove(); }, [this](){ UpdateMove(); }, {});
	m_state.AddState(State::ATTACK, {}, [this](){ UpdateAttack(); }, {});
	m_state.SetState(State::MOVE);

	// ���f���̃C���X�^���X�̍쐬
	m_pModel = std::make_shared<Model>(ModelHandleManager::GetInstance().GetHandle("Mosquito"));

	// ���f���̐ݒ�
	m_pModel->SetOpacity(m_opacity);
	m_pModel->SetRotMtx(rotMtx);
	m_pModel->SetScale(m_scale);
	m_pModel->SetPos(m_pos);
	m_pModel->ChangeAnimation(idle_anim_num, true, false, change_anim_frame);
	m_pModel->Update();
}

// �f�X�g���N�^
Mosquito::~Mosquito()
{
}

// �ړ���Ԃɓ���O�̏���
void Mosquito::EntarMove()
{
	// �ړ��|�C���g�̃C���f�b�N�X���s���f�[�^�̃T�C�Y�𒴂��Ă�����
	if (m_movePointIndex >= m_actionDataList.size())
	{
		// ���݃t���O��������
		m_isEnabled = false;

		// ���[�U�[�𔭎˂��Ă�����
		if (m_laserKey != -1)
		{
			// ���[�U�[�̍폜
			m_pLaserManager->DeleteLaser(m_laserKey);
		}
		return;
	}

	// �ړI�n�̎擾
	SetGoalPos();

	// ������
	m_isGoal = false;
}

// �X�V
void Mosquito::Update()
{
	// �X�e�[�g�}�V���̍X�V
	m_state.Update();

	// �v���C���[�������Ă�����
	if (m_pPlayer->IsEnabled())
	{
		// ���[�U�[�̔��ˈʒu�̍X�V
		m_laserFirePos = Vector3::FromDxLibVector3(
			MV1GetFramePosition(m_pModel->GetModelHandle(), laser_fire_frame_pos));

		// �v���C���[�������悤�ɉ�]�s���ݒ�
		auto rotMtx = Math::Matrix::GetRotationMatrix(init_model_direction, (m_pPlayer->GetPos() - m_pos).Normalized());
		m_pModel->SetRotMtx(rotMtx);
	}

	// ���f���̐ݒ�
	m_pModel->RestoreAllMaterialDifColor();	// �f�B�t���[�Y�J���[�����ɖ߂�
	m_pModel->SetOpacity(m_opacity);
	m_pModel->SetPos(m_pos);
	m_pModel->Update();
}

// �ҋ@��Ԃ̍X�V
void Mosquito::UpdateIdle()
{
	// Z���W�̍X�V
	m_pos.z += m_pPlayer->GetMoveVec().z;

	// �ҋ@�t���[�����I�������
	if (m_actionData.idleFrame-- <= 0)
	{
		// �ړ���ԂɑJ��
		m_state.SetState(State::MOVE);
	}
}

// �ړ���Ԃ̍X�V
void Mosquito::UpdateMove()
{
	// �ړI�n�ɓ��B�������̔���
	if (m_pos.Distance(m_goalPos) < distance_threshold &&
		!m_isGoal)
	{
		// ���B�����ꍇ�A��񂵂��ʂ��Ăق����Ȃ��ׁA
		// �t���O�𗧂Ă�
		m_isGoal = true;

		// ���݂̈ړ��|�C���g�̃C�e���[�^�[�̎擾
		auto itr = m_actionDataList.begin();
		std::advance(itr, m_movePointIndex);

		// �s���f�[�^�̐ݒ�
		m_actionData = *itr;

		// ���݂̈ړ��|�C���g�̃��[�U�[���˃t���O�������Ă�����
		// ���[�U�[���ˏ�ԂɑJ��
		if (itr->isLaser)
		{
			// ���[�U�[����
			m_state.SetState(State::ATTACK);
		}
		else
		{
			// ���[�U�[���˃t���O�������Ă��Ȃ�������
			m_state.SetState(State::IDLE);
		}
		// ���̈ړ��|�C���g��
		m_movePointIndex++;	
	}
	else
	{
		// �ړI�n��Z���W�̍X�V

		// ���݂̈ړ��|�C���g�̃C�e���[�^�[�̎擾
		auto itr = m_actionDataList.begin();
		std::advance(itr, m_movePointIndex);

		// 0.0(near)�`1.0(far)�͈̔͂�Z���W��ݒ�
		float z = (fabs(GetCameraPosition().z - m_pPlayer->GetPos().z) + itr->goalPos.z) / GetCameraFar();

		// �ړI�n�̐ݒ�
		Vector3 goalPos = Vector3::FromDxLibVector3(ConvScreenPosToWorldPos_ZLinear({ itr->goalPos.x, itr->goalPos.y, z }));
		m_goalPos.z = goalPos.z;

		// �ړ��x�N�g���̐ݒ�
		m_moveVec = (m_goalPos - m_pos).Normalized() * m_moveSpeed;

		// ���W�̍X�V
		m_pos += m_moveVec;
	}
}

// �U����Ԃ̍X�V
void Mosquito::UpdateAttack()
{
	// Z���W�̍X�V
	m_pos.z += m_pPlayer->GetMoveVec().z;

	// ���[�U�[���˂܂ł̑ҋ@�t���[�����I�������
	if (m_actionData.laserIdleFrame-- <= 0 && m_actionData.isLaser)
	{
		switch (m_actionData.laserType)
		{
		case LaserType::NORMAL:
			// ���[�U�[�̔���
			m_laserKey = m_pLaserManager->AddLaser(
				m_actionData.laserType,
				shared_from_this(),
				m_actionData.laserChargeFrame,
				m_actionData.laserFireFrame,
				m_actionData.laserSpeed,
				m_actionData.isPlayerFollowing);
			break;
		case LaserType::CUBE:
			// �L���[�u���[�U�[�̔���
			m_pLaserManager->AddCubeLaser(m_laserFirePos, m_actionData.laserSpeed);
			break;
		}

		// ���[�U�[�𔭎˂����̂Ńt���O��������
		m_actionData.isLaser = false;
	}
	// ���[�U�[���ˌ�A���[�U�[���˒��t���[���̍X�V
	else if (m_actionData.laserIdleFrame <= 0 && !m_actionData.isLaser)
	{
		// ���[�U�[���˒��t���[�����I�������
		if (m_actionData.laserFireFrame-- <= 0)
		{
			// �ҋ@��ԂɑJ��
			m_state.SetState(State::IDLE);

			// ���[�U�[�̍폜
			m_pLaserManager->DeleteLaser(m_laserKey);
		}
	}
}

// �ړI�n�̎擾
void Mosquito::SetGoalPos()
{
	// ���݂̈ړ��|�C���g�̃C�e���[�^�[�̎擾
	auto itr = m_actionDataList.begin();
	std::advance(itr, m_movePointIndex);

	// 0.0(near)�`1.0(far)�͈̔͂�Z���W��ݒ�
	float z = (fabs(GetCameraPosition().z - m_pPlayer->GetPos().z) + itr->goalPos.z) / GetCameraFar();

	// �ړI�n�̐ݒ�
	m_goalPos = Vector3::FromDxLibVector3(ConvScreenPosToWorldPos_ZLinear({ itr->goalPos.x, itr->goalPos.y, z }));

	// �ړ��x�N�g���̐ݒ�
	m_moveVec = (m_goalPos - m_pos).Normalized() * m_moveSpeed;
}

// �`��
void Mosquito::Draw()
{
	// �֊s���̕`��
	DrawOutLine();		

	// ���f���̕`��
	m_pModel->Draw();

#ifdef _DEBUG
	// �ړI�n�̕`��
	DrawSphere3D(m_goalPos.ToDxLibVector3(), 10.0f, 10, 0xff0000, 0xff0000, TRUE);

	// �����蔻��̕`��
	DrawSphere3D(m_pos.ToDxLibVector3(), hit_radius * m_scale.x, 10, 0xff0000, 0xff0000, FALSE);
#endif
}

// �֊s���̕`��
void Mosquito::DrawOutLine()
{
	// TODO : shader�ł��

	// ���X�̊g�嗦��ۑ�
	Vector3 scale = m_scale;

	// �֊s���ׂ̈Ɋg��
	m_pModel->SetScale({ scale.x * outline_width, scale.y * outline_width, 0.0f });

	// �}�e���A���̃f�B�t���[�Y�J���[��ݒ�
	m_pModel->SetAllMaterialDifColor(GetColorF(255.0f, 0.0f, 0.0f, 255.0f));

	// �`��
	m_pModel->Draw();

	// ���X�̃}�e���A���̃f�B�t���[�Y�J���[��ݒ�
	m_pModel->RestoreAllMaterialDifColor();

	// ���X�̊g�嗦�ɖ߂�
	m_pModel->SetScale(m_scale);
}

// ���W�̎擾
const Vector3& Mosquito::GetPos() const
{
	return m_pos;
}