#include "NormalLaser.h"
#include "../Enemy/EnemyBase.h"
#include "../Player.h"
#include "../../Math/Vector2.h"
#include "../../Application.h"
#include "../../Effect/Effekseer3DEffectManager.h"
#include "../../Game/Model.h"
#include "../../MyDebug/DebugText.h"
#include "../../Resource/Model/ModelResourceManager.h"
#include "../../Sound/SoundManager.h"
#include <DxLib.h>
#include <random>
#include <algorithm>

namespace
{
	// ���f��
	const Math::Vector3 init_model_direction = { 1.0f, 0.0f, 0.0f };	// ��������
	const Math::Vector3 laser_model_scale    = { 0.1f, 0.1f, 0.1f };	// �g�嗦

	// �G�t�F�N�g
	const Math::Vector3 init_laser_effect_direction    = { 0.0f, 0.0f, -1.0f };	// ��������
	const Math::Vector3 laser_effect_scale             = { 20.0f, 20.0f, 24.0f };	// �g�嗦
	constexpr float laser_effect_play_speed      = 1.0f;					// �Đ����x
	constexpr int init_laser_effect_charge_frame = 140;						// ���X�̃`���[�W�t���[��
	constexpr float laser_effect_z_length        = 300.0f;					// Z���̒���

	// �T�E���h
	constexpr int laser_charge_sound_start_fade_out_frame = 30;	// ���[�U�[�̃`���[�W�T�E���h�̃t�F�[�h�A�E�g�̊J�n�t���[����
	constexpr int laser_charge_sound_fade_out_frame       = 20;	// ���[�U�[�̃`���[�W�T�E���h�̃t�F�[�h�A�E�g�̃t���[����

	// �t���[��(�{�[��)
	constexpr int laser_end_frame_no = 0;	// ���[�U�[�̒[�̃t���[���ԍ�

	// �ړI�n�ɓ��B�������ǂ����̔���
	// �����臒l�i�K�؂Ȓl�ɒ�������K�v�j
	constexpr float distance_threshold = 5.0f;

	// ���˒��ɃV�[���h�ɉ��t���[���������Ă��Ȃ������甽�˂��������邩
	constexpr int reflect_collision_shield_frame = 5;

	// �v���C���[��Ǐ]���Ȃ��ꍇ�̌������ʒu
	const Math::Vector2 window_size =
		{ static_cast<float>(Application::GetInstance()->GetWindowSize().width), 
		  static_cast<float>(Application::GetInstance()->GetWindowSize().height )};
	const Math::Vector2 goal_pos[] =
	{
		{ 0 + 200, 0 + 200 },
		{ window_size.x - 200, 0 + 200 },
		{ 0 + 200, window_size.y - 200 },
		{ window_size.x - 200, window_size.y - 200 },
		{ window_size.x / 2, window_size.y / 2 },
	};
}

// �R���X�g���N�^
NormalLaser::NormalLaser(const std::shared_ptr<EnemyBase>& pEnemy, 
						 const std::shared_ptr<Player>& pPlayer, 
						 const int laserChargeFrame, 
						 const int laserFireFrame, 
						 const float laserSpeed, 
						 const bool isPlayerFollowing) :
	m_pEnemy(pEnemy),
	m_pPlayer(pPlayer),
	m_laserFireFrame(laserFireFrame),
	m_isPlayerFollowing(isPlayerFollowing),
	m_normalFireMovePointIndex(0),
	m_reflectFrame(0)
{
	// ������
	m_pos   = m_pEnemy->GetLaserFirePos();
	m_scale = laser_model_scale;
	m_speed = laserSpeed;

	// �G�t�F�N�g�̃`���[�W�t���[����ݒ�
	m_chargeEffectFrame = laserChargeFrame;

	// �G�t�F�N�g�̃`���[�W�t���[������Đ����x���Z�o
	float effectSpeed = static_cast<float>(init_laser_effect_charge_frame) / static_cast<float>(m_chargeEffectFrame);

	// �v���C���[��Ǐ]���Ȃ��ꍇ
	if(!m_isPlayerFollowing)
	{
		// ���[�U�[�̖ړI�n�����X�g�ɒǉ�
		for (auto& movePoint : goal_pos)
		{
			m_normalFireMovePointList.push_back(movePoint);
		}

		// �����_���ŖړI�n��ݒ�(�z��̒��g���V���b�t��)
		std::random_device seed;
		std::mt19937 engine(seed());
		std::shuffle(m_normalFireMovePointList.begin(), m_normalFireMovePointList.end(), engine);

		// �ړI�n��ݒ�
		Math::Vector2 screenPos = m_normalFireMovePointList.front();
		float z = (fabs(GetCameraPosition().z - m_pPlayer->GetPos().z)) / GetCameraFar();
		m_normalFireGoalPos = Math::Vector3::FromDxLibVector3(ConvScreenPosToWorldPos_ZLinear({ screenPos.x,screenPos.y, z }));

	}
	// ���[�U�[�̌������W��ݒ�
	m_directionPos = m_pPlayer->GetPosLogTable().back();

	// �x�N�g�������̉�]�s����쐬
	m_rotMtx = Math::Matrix::GetRotationMatrix(init_model_direction, (m_directionPos - m_pos).Normalized());

	// �x�N�g�������̉�]�s�񂩂�I�C���[�p���o��
	auto effectRotMtx = Math::Matrix::GetRotationMatrix(init_laser_effect_direction, (m_directionPos - m_pos).Normalized());
	Math::Vector3 effectRot = effectRotMtx.ToEulerAngle();

	// �G�t�F�N�g�̍Đ�
	Effect::Effekseer3DManager::GetInstance()->PlayEffectFollow(
		m_laserEffectHandle,
		"NormalLaser",
		&m_pos,
		laser_effect_scale,
		effectSpeed,
		effectRot);

	// ��Ԃ̒ǉ�
	m_stateMachine.AddState(State::CHARGE, [this]() { EnterCharge(); }, [this]() { UpdateCharge(); }, {});
	m_stateMachine.AddState(State::FIRE_PLYER_FOLLOWING, {}, [this]() { UpdateFirePlayerFollowing(); }, {});
	m_stateMachine.AddState(State::NORMAL_FIRE, {}, [this]() { UpdateNormalFire(); }, {});
	m_stateMachine.SetState(State::CHARGE);

	// �����蔻��Ɏg�p���郂�f���̐ݒ�
	m_pModel = std::make_unique<Model>(Resource::Model::Manager::GetInstance()->GetHandle("Laser"));	// �C���X�^���X����
	m_pModel->SetUseCollision(true);// �����蔻��ݒ�
	m_pModel->SetScale(m_scale);	// �g�嗦
	m_pModel->SetRotMtx(m_rotMtx);	// ��]�s��
	m_pModel->SetPos(m_pos);		// �ʒu
	m_pModel->SetOpacity(m_opacity);// �s�����x
	m_pModel->Update();				// �����蔻��̍X�V
}


// �f�X�g���N�^
NormalLaser::~NormalLaser()
{
	// �G�t�F�N�g�̍폜
	Effect::Effekseer3DManager::GetInstance()->DeleteEffect(m_laserEffectHandle);

	// ���[�U�[�̉��̒�~
	Sound::Manager::GetInstance()->StopSound("Laser");
}

// �X�V
void NormalLaser::Update()
{
	// ���ˌ��̓G������ł�����
	if (!m_pEnemy->IsEnabled())
	{
		// ���݃t���O��������
		m_isEnabled = false;
		return;
	}

	// �X�e�[�g���`���[�W��ԂłȂ��ꍇ
	if(m_stateMachine.GetCurrentState() != State::CHARGE)
	{
		// �G�t�F�N�g�̍Đ����x�̐ݒ�
		Effect::Effekseer3DManager::GetInstance()->SetEffectSpeed(m_laserEffectHandle, laser_effect_play_speed);

		// ���˃��[�U�[�̉����Đ�����Ă��Ȃ��ꍇ
		const auto& soundManager = Sound::Manager::GetInstance();
		if (!soundManager->IsPlaySound("ReflectLaser"))
		{
			// �ʏ탌�[�U�[�̉����Đ�����Ă��Ȃ��ꍇ
			if (!soundManager->IsPlaySound("Laser"))
			{
				// ���[�U�[���̍Đ�
				soundManager->PlaySE("Laser");
			}
		}

		// ���[�U�[�̔��˃t���[����0�ȉ��ɂȂ�����
		if (m_laserFireFrame-- <= 0)
		{
			// ���݃t���O��������
			m_isEnabled = false;
		}
	}
	
	// ���[�U�[�����ˏ�ԂȂ�
	if (m_isReflect)
	{
		// �t���[�������Z
		// ���t���[���o�߂����甽�ˏ�Ԃ�����
		if (m_reflectFrame++ > reflect_collision_shield_frame)
		{
			// ���ˏ�Ԃ�����
			UndoReflect();
		}
	}
	
	// ��Ԃ̍X�V
	m_stateMachine.Update();

	// ��ɔ��ˌ��̈ʒu�ɍ��킹��
	m_pos = m_pEnemy->GetLaserFirePos();

	// ���[�U�[�̐�[�̍��W��ݒ�
	m_endPos = Math::Vector3::FromDxLibVector3(
		MV1GetFramePosition(m_pModel->GetModelHandle(), laser_end_frame_no));

	// �x�N�g�������̉�]�s����쐬
	m_rotMtx = Math::Matrix::GetRotationMatrix(init_model_direction, (m_directionPos - m_pos).Normalized());

	// �x�N�g�������̉�]�s�񂩂�I�C���[�p���o��
	auto  effectRotMtx   = Math::Matrix::GetRotationMatrix(init_laser_effect_direction, (m_directionPos - m_pos).Normalized());
	Math::Vector3 effectRot    = effectRotMtx.ToEulerAngle();

	// �G�t�F�N�g�̉�]����ݒ�
	Effect::Effekseer3DManager::GetInstance()->SetEffectRot(m_laserEffectHandle, effectRot);

	// ���[�U�[�̔��˃t���O�������Ă��Ȃ��ꍇ
	if (!m_isReflect)
	{
		// �G�t�F�N�g�̊g�嗦��ݒ�
		Effect::Effekseer3DManager::GetInstance()->SetEffectScale(m_laserEffectHandle, laser_effect_scale);
	}

	// ���f���̐ݒ�
	m_pModel->SetRotMtx(m_rotMtx);	// ��]�s��
	m_pModel->SetPos(m_pos);		// �ʒu
	m_pModel->SetOpacity(m_opacity);// �s�����x
	m_pModel->SetScale(m_scale);	// �g�嗦
	m_pModel->Update();				// �����蔻��̍X�V
}

// �`���[�W��Ԃ̍X�V
void NormalLaser::UpdateCharge()
{
	// �`���[�W�t���[���̎c��t���[��������̃t���[�����ȉ��ɂȂ�����
	if (m_chargeEffectFrame <= laser_charge_sound_start_fade_out_frame)
	{
		// ���[�U�[�̃`���[�W�T�E���h�̃t�F�[�h�A�E�g�̐ݒ�
		const auto& soundManager = Sound::Manager::GetInstance();
		soundManager->SetFadeSound("LaserCharge", laser_charge_sound_fade_out_frame, soundManager->GetSoundVolume("LaserCharge"), 0);
	}

	// �`���[�W���Ԃ��I�������
	if(m_chargeEffectFrame-- <= 0)
	{
		// ���f���̊g�嗦��ݒ�
		m_scale.x = -1.0f;

		// �X�e�[�g�̕ύX
		if (m_isPlayerFollowing)
		{
			// �v���C���[��Ǐ]����ꍇ
			m_stateMachine.SetState(State::FIRE_PLYER_FOLLOWING);

			// ���[�U�[�̌������W��ݒ�
			m_directionPos = m_pPlayer->GetPosLogTable().back();
		}
		else
		{
			// �v���C���[��Ǐ]���Ȃ��ꍇ
			m_stateMachine.SetState(State::NORMAL_FIRE);
		}
	}
}

// �ʏ픭�ˏ�Ԃ̍X�V
void NormalLaser::UpdateNormalFire()
{
	// �ړI�n�ɓ��B�������ǂ����̔���
	if (m_directionPos.Distance(m_normalFireGoalPos) < distance_threshold)
	{
		// �ړI�n�ɓ��B�����玟�̖ړI�n��ݒ�
		m_normalFireMovePointIndex++;

		// ���̖ړI�n�����X�g�̍Ō�܂œ��B������
		if (m_normalFireMovePointIndex >= m_normalFireMovePointList.size())
		{
			// �C���f�b�N�X��������
			m_normalFireMovePointIndex = 0;

			// �n�_�̈ړ������̓���ւ�(�z��̒��g���V���b�t��)
			std::random_device seed;
			std::mt19937 engine(seed());
			std::shuffle(m_normalFireMovePointList.begin(), m_normalFireMovePointList.end(), engine);
		}

		// ���̖ړI�n��ݒ�
		Math::Vector2 screenPos = m_normalFireMovePointList[m_normalFireMovePointIndex];
		float z = (fabs(GetCameraPosition().z - m_pPlayer->GetPos().z)) / GetCameraFar();
		m_normalFireGoalPos = Math::Vector3::FromDxLibVector3(ConvScreenPosToWorldPos_ZLinear({ screenPos.x,screenPos.y, z }));
	}
	else
	{
		// �S�[���̍��W��ݒ�
		float z = (fabs(GetCameraPosition().z - m_pPlayer->GetPos().z)) / GetCameraFar();
		m_normalFireGoalPos = Math::Vector3::FromDxLibVector3(
			ConvScreenPosToWorldPos_ZLinear(
				{ m_normalFireMovePointList[m_normalFireMovePointIndex].x,
				  m_normalFireMovePointList[m_normalFireMovePointIndex].y,
				  z }));

		// �x�N�g����ݒ�
		m_directionVec = (m_normalFireGoalPos - m_directionPos).Normalized() * m_speed;

		// ���W���X�V
		m_directionPos += m_directionVec;
	}
}

// �v���C���[��Ǐ]���Ĕ��ˏ�Ԃ̍X�V
void NormalLaser::UpdateFirePlayerFollowing()
{
	// �x�N�g����ݒ�
	m_directionVec = (m_pPlayer->GetPos() - m_directionPos).Normalized() * m_speed;

	// ���W�̍X�V
	m_directionPos.z += m_pPlayer->GetMoveVec().z;
	m_directionPos   += m_directionVec;
}

// �`��
void NormalLaser::Draw()
{
#ifdef _DEBUG
	// ���f���̕`��
	SetUseLighting(false);
//	m_pModel->Draw();
	SetUseLighting(true);

	Debug::Text::AddLog("LaserDirectionPos", { m_directionPos.x, m_directionPos.y, m_directionPos.z });
#endif 
}

// ���[�U�[���~�߂�
void NormalLaser::Stop(const Math::Vector3& pos)
{
	// ���[�U�[�̔��˒n�_����V�[���h�܂ł̋������Z�o
	Math::Vector3 vec = pos - m_pos;
	float length = vec.Length();

	// ���[�U�[�̃G�t�F�N�g���V�[���h�Ŏ~�܂��Ă�悤������悤�ɁA�G�t�F�N�g�̊g�嗦��ݒ�
	Effect::Effekseer3DManager::GetInstance()->SetEffectScale(
		m_laserEffectHandle, { laser_effect_scale.x, laser_effect_scale.y, length / laser_effect_z_length });

	// ���[�U�[�̓����蔻��p���f���𔽑Α��Ɍ�����
	m_pModel->SetScale(m_scale);
	m_pModel->Update();
}

// ����
void NormalLaser::Reflect(const Math::Vector3& pos)
{
	// ���[�U�[�̔��˃t���O�𗧂Ă�
	m_isReflect = true;

	// �V�[���h�ɓ��������̂ŏ�����
	m_reflectFrame = 0;

	// ���[�U�[���~�߂�
	Stop(pos);
}

// ���˂��ꂽ��Ԃ��猳�ɖ߂�
void NormalLaser::UndoReflect()
{
	// ������
	m_isReflect = false;
	m_reflectFrame = 0;
}

// �����x�N�g���̎擾
Math::Vector3 NormalLaser::GetDirection() const
{
	return (m_directionPos - m_pos);
}

// �`���[�W��Ԃɓ���
void NormalLaser::EnterCharge()
{
	Sound::Manager::GetInstance()->PlaySE("LaserCharge");
}