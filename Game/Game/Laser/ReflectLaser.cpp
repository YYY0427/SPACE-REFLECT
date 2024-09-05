#include "ReflectLaser.h"
#include "../Shield.h"
#include "../../Math/Matrix.h"
#include "../../Math/MathUtil.h"
#include "../../Effect/Effekseer3DEffectManager.h"
#include "../../Game/Enemy/EnemyManager.h"
#include "../../Game/Enemy/EnemyBase.h"
#include "../../Model.h"
#include "../../MyDebug/DebugText.h"
#include "../../ModelHandleManager.h"
#include "../../Sound/SoundManager.h"

namespace
{
	// ��������
	const Vector3 init_model_direction        = { 1.0f, 0.0f, 0.0f };	// ���f��
	const Vector3 init_laser_effect_direction = { 0.0f, 0.0f, -1.0f };	// �G�t�F�N�g

	// �g�嗦
	const Vector3 model_scale  = { 1.0f, 0.2f, 0.2f };		// ���f��
//	const Vector3 model_scale  = { 1.0f, 0.1f, 0.1f };		// ���f��
	const Vector3 effect_scale = { 70.0f, 70.0f, 40.0f };	// �G�t�F�N�g
//	const Vector3 effect_scale = { 40.0f, 40.0f, 40.0f };	// �G�t�F�N�g

	// ���[�U�[�̈ړ����x
	constexpr float move_speed = 35.0f;			

	// ���[�U�[�G�t�F�N�g�̍Đ����x
	constexpr float effect_play_speed = 1.0f;

	// �G�C���A�V�X�g
	constexpr float aim_assist_range = 3000.0f;	// �G�C���A�V�X�g�͈̔�
	constexpr float aim_assist_power = 0.45;	// �G�C���A�V�X�g�̋���(0.0�`1.0)
}

// �R���X�g���N�^
ReflectLaser::ReflectLaser(const std::shared_ptr<EnemyManager>& pEnemyManager, 
						   const std::shared_ptr<Shield>& pShield, 
						   const std::shared_ptr<LaserBase>& pLaser, 
						   const Vector3& firePos)
{
	// ������
	m_pos = firePos;
	m_pShield = pShield;
	m_pLaser = pLaser;
	m_pEnemyManager = pEnemyManager;
	m_isReflect = true;
	m_isEnabled = true;

	// ���˃x�N�g�����쐬
	m_directionPos = Vector3::Reflect(m_pLaser->GetDirection(), Vector3::FromDxLibVector3(m_pShield->GetVertex().front().norm));

	// �w�肵���ʒu�����Ɍ�����s��̍쐬
	auto rotEffectMtx = Math::Matrix::GetRotationMatrix(init_laser_effect_direction, m_directionPos);	// �G�t�F�N�g
	auto rotModelMtx  = Math::Matrix::GetRotationMatrix(init_model_direction, -m_directionPos);			// ���f��

	// ���[�U�[�G�t�F�N�g�̍Đ�
	Effect::Effekseer3DManager::GetInstance()->PlayEffectLoopAndFollow(
		m_laserEffectHandle, "ReflectLaser", &m_pos, effect_scale, effect_play_speed, rotEffectMtx.ToEulerAngle());

	// ���X�̃��[�U�[�����~
	const auto& soundManager = Sound::Manager::GetInstance();
	soundManager->StopSound("Laser");
	
	// ���[�U�[�̉��̍Đ�
	soundManager->PlaySE("ReflectLaser");

	// ���f���̐ݒ�
	m_pModel = std::make_shared<Model>(ModelHandleManager::GetInstance().GetHandle("Laser"));	// �C���X�^���X����
	m_pModel->SetUseCollision(true);	// �����蔻��ݒ�
	m_pModel->SetScale(model_scale);	// �g�嗦
	m_pModel->SetRotMtx(rotModelMtx);	// ��]�s��
	m_pModel->SetPos(m_pos);			// �ʒu
	m_pModel->Update();					// �����蔻��̍X�V
}											

// �f�X�g���N�^
ReflectLaser::~ReflectLaser()
{
	// �G�t�F�N�g�̍폜
	Effect::Effekseer3DManager::GetInstance()->DeleteEffect(m_laserEffectHandle);

	// ���[�U�[�̉��̒�~
	Sound::Manager::GetInstance()->StopSound("ReflectLaser");
}

// �X�V
void ReflectLaser::Update()
{
	// ���ˌ��̃��[�U�[���V�[���h�ɔ��˂��Ă��Ȃ��܂��͖����Ȃ�
	if (!m_pLaser->IsReflect() || !m_pLaser->IsEnabled())
	{
		// �����ɂ���
		m_isEnabled = false;
		return;
	}

	// ���[�U�[�������Ă�������Ƌ߂��G��T��
	Vector3 enemyPos{};	
	if (!m_pEnemyManager->GetEnemyList().empty())
	{
		enemyPos = m_pEnemyManager->GetEnemyList().front()->GetPos();
		float minDistance = enemyPos.Distance(m_directionPos);

		for (auto& enemy : m_pEnemyManager->GetEnemyList())
		{
			// �G���L���Ȃ�
			if (enemy->IsEnabled())
			{
				// �G�ƃ��[�U�[�̌����Ă�������Ƃ̋������r
				float distance = enemy->GetPos().Distance(m_directionPos);

				// ���߂��G�̍��W���擾
				if (distance < minDistance)
				{
					enemyPos = enemy->GetPos();
					minDistance = distance;
				}
			}
		}
	}

	// �{�X���������Ă���Ȃ�
	if (m_pEnemyManager->IsBossAlive())
	{
		// �{�X�̍��W���擾
		enemyPos = m_pEnemyManager->GetBossEnemy()->GetPos();
	}

	// Z���𖳌��ɂ���
	Vector3 directionPos = m_directionPos;
	directionPos.z = 0;
	enemyPos.z     = 0;

	// �G�ƃ��[�U�[�̌����Ă�������Ƃ̋��������͈͓��Ȃ�
	Vector3 aimAssistVec{};
	if (enemyPos.Distance(directionPos) < aim_assist_range)
	{
		// �G�C���A�V�X�g��L���ɂ���
		aimAssistVec = (enemyPos - directionPos).Normalized();
	}

	// ���˃x�N�g�����쐬
	Vector3 goalPos = Vector3::Reflect(
		m_pLaser->GetDirection(), Vector3::FromDxLibVector3(m_pShield->GetVertex().front().norm));
	Vector3 reflectVec = (goalPos - m_directionPos).Normalized();

	// �x�N�g���̎擾
	float reflectVecPower = 1.0f - aim_assist_power;
	Vector3 moveVec = ((reflectVec * reflectVecPower) + (aimAssistVec * aim_assist_power)) * move_speed;

	// �����������X�V
	m_directionPos += moveVec;

	// �w�肵���x�N�g�������Ɍ�����s��̍쐬
	auto rotEffectMtx = Math::Matrix::GetRotationMatrix(init_laser_effect_direction, m_directionPos);	// �G�t�F�N�g
	auto rotModelMtx  = Math::Matrix::GetRotationMatrix(init_model_direction, -m_directionPos);			// ���f��

	// �G�t�F�N�g�̉�]����ݒ�
	Effect::Effekseer3DManager::GetInstance()->SetEffectRot(m_laserEffectHandle, rotEffectMtx.ToEulerAngle());

	// ���f���̐ݒ�
	m_pModel->SetRotMtx(rotModelMtx);	// ��]�s��
	m_pModel->SetPos(m_pos);			// �ʒu
	m_pModel->Update();					// �����蔻��̍X�V
}													

// �`��
void ReflectLaser::Draw()
{
#ifdef _DEBUG
	// ���f���̕`��
//	m_pModel->Draw();
#endif 
}