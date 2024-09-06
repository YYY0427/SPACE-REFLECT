#include "StageBase.h"
#include "../Scene/StageSelectScene.h"
#include "../Scene/SceneManager.h"
#include "../Editor/DataReaderFromUnity.h"
#include "../UI/UIManager.h"
#include "../Effect/Effekseer3DEffectManager.h"
#include "../Transitor/Fade.h"
#include "../Game/Player.h"
#include "../Game/MeteorManager.h"
#include "../Game/Laser/LaserManager.h"
#include "../Game/Laser/LaserBase.h"
#include "../Game/Shield.h"
#include "../Game/Enemy/EnemyManager.h"
#include "../Game/Enemy/EnemyBase.h"
#include "../Effect/ScreenShaker.h"
#include "../UI/DamageFlash.h"
#include "../Score/ScoreManager.h"
#include "../Score/ScoreRanking.h"
#include "../UI/ResultWindow.h"
#include "../Sound/SoundManager.h"
#include <DxLib.h>

// �R���X�g���N�^
StageBase::StageBase(SceneManager& manager) :
	m_manager(manager),
	m_currentFrame(0),
	m_isWaveStart(false),
	m_directionalLightHandle(-1),
	m_meteorDamage(0),
	m_laserDamage(0),
	m_enemyDamage(0),
	m_playerToBossDamage(0),
	m_playerToEnemyDamage(0)
{
	// �t�F�[�h�̃X�^�[�g
	m_pFade = std::make_unique<Fade>();
	m_pFade->StartFadeIn(0);

	// ���C�g�̐ݒ�
	m_directionalLightHandle = CreateDirLightHandle({ 1, 1, 0 });
	SetLightDifColorHandle(m_directionalLightHandle, GetColorF(0.5f, 0.5f, 0.5f, 0.0f));

	// �X�R�A�̏�����
	Score::Manager::GetInstance()->Reset();
}

// �f�X�g���N�^
StageBase::~StageBase()
{
	// ���C�g�̍폜
	DeleteLightHandle(m_directionalLightHandle);

	// ���ׂĂ�UI���폜
	UI::Manager::GetInstance()->DeleteAllUI();

	// �G�t�F�N�g�̑S�폜
	Effect::Effekseer3DManager::GetInstance()->DeleteAllEffect();

	// �I�u�W�F�N�g�̔z�u�f�[�^�̍폜
	DataReaderFromUnity::GetInstance().DeleteAllData();

	// �S�Ẳ��y���~
	Sound::Manager::GetInstance()->StopAllSound();
}

// ���U���g��ʂ̍X�V
void StageBase::UpdateResult(const std::string& stageName)
{
	// �t�F�[�h���I��������
	if (m_pFade->IsFadeOutEnd())
	{
		// �X�e�[�W�Z���N�g�ɑJ��
		m_manager.ChangeScene(std::make_shared<StageSelectScene>(m_manager));
		return;
	}

	// ���U���g��ʂ̍X�V
	m_pResultWindow->Update();

	// ���U���g��ʂ��I��������
	if (m_pResultWindow->IsEnd() && !m_pFade->IsFadingOut())
	{
		// �X�R�A�������L���O�ɒǉ�
		Score::Ranking::GetInstance()->AddScore(stageName, "NO NAME", Score::Manager::GetInstance()->GetTotalScore());

		// �t�F�[�h�A�E�g�J�n
		m_pFade->StartFadeOut(255);
	}
}

// �`��
void StageBase::Draw()
{
}

// �����蔻��
void StageBase::Collision()
{
	// �v���C���[��覐΂̓����蔻��
	for (auto& meteor : m_pMeteorManager->GetMeteor())
	{
		// ������覐΂Ȃ画�肵�Ȃ�
		if(meteor->GetType() == MeteorType::SMALL) continue;

		// ���ƃ��b�V���̓����蔻��
		MV1_COLL_RESULT_POLY_DIM result{};
		result = MV1CollCheck_Sphere(
			meteor->GetModelHandle(),
			-1,
			m_pPlayer->GetPos().ToDxLibVector3(),
			m_pPlayer->GetCollisionRadius());

		// �������Ă�����
		if (result.HitNum > 0)
		{
			// �v���C���[�̃_���[�W����
			m_pPlayer->OnDamage(m_meteorDamage);

			// �_���[�W�t���b�V���̉��o
			m_pDamageFlash->Start(60, 50, 0xff0000);

			// ��ʗh��̉��o
			m_pScreenShaker->StartShake({ m_meteorDamage * 10.0f, m_meteorDamage * 10.0f }, 30);
		}

		// �����蔻����̌�n��
		MV1CollResultPolyDimTerminate(result);
	}

	// �V�[���h�ƓG���[�U�[�̓����蔻��
	for (auto& laser : m_pLaserManager->GetLaserList())
	{
		// ���[�U�[�̎�ނ����˃��[�U�[�Ȃ画�肵�Ȃ�
		if (laser.type == LaserType::REFLECT)	continue;

		// �L���[�u���[�U�[�Ȃ画�肵�Ȃ�
		if (laser.type == LaserType::CUBE) continue;

		// �v���C���[�̃V�[���h���Ȃ���Δ��肵�Ȃ�
		if (!m_pPlayer->GetShield()->IsShield())
		{
			// ���[�U�[�����ɖ߂�
			laser.pLaser->UndoReflect();

			// �V�[���h���Ȃ���Δ��肵�Ȃ�
			continue;
		}

		// �V�[���h�̒��_�̍��W���擾
		Math::Vector3 shieldLeftTopPos = Math::Vector3::FromDxLibVector3(m_pPlayer->GetShield()->GetVertex()[0].pos);
		Math::Vector3 shieldRightTopPos = Math::Vector3::FromDxLibVector3(m_pPlayer->GetShield()->GetVertex()[1].pos);
		Math::Vector3 shieldLeftBottomPos = Math::Vector3::FromDxLibVector3(m_pPlayer->GetShield()->GetVertex()[2].pos);
		Math::Vector3 shieldRightBottomPos = Math::Vector3::FromDxLibVector3(m_pPlayer->GetShield()->GetVertex()[3].pos);

		// �V�[���h��2�̃|���S������ł��Ă�̂�2�̃|���S���Ƃ��`�F�b�N
		HITRESULT_LINE result = HitCheck_Line_Triangle(
			laser.pLaser->GetPos().ToDxLibVector3(), laser.pLaser->GetEndPos().ToDxLibVector3(),
			shieldLeftTopPos.ToDxLibVector3(), shieldRightTopPos.ToDxLibVector3(), shieldLeftBottomPos.ToDxLibVector3());

		HITRESULT_LINE result2 = HitCheck_Line_Triangle(
			laser.pLaser->GetPos().ToDxLibVector3(), laser.pLaser->GetEndPos().ToDxLibVector3(),
			shieldRightBottomPos.ToDxLibVector3(), shieldLeftBottomPos.ToDxLibVector3(), shieldRightTopPos.ToDxLibVector3());

		// �ǂ������̃|���S�����������Ă�����
		if (result.HitFlag || result2.HitFlag)
		{
			// ���˃��[�U�[�̔��ˈʒu���擾
			Math::Vector3 firePos{};
			if (result.HitFlag)	firePos = Math::Vector3::FromDxLibVector3(result.Position);
			else				firePos = Math::Vector3::FromDxLibVector3(result2.Position);

			// �܂����˃��[�U�[���Ȃ���Δ��˃��[�U�[��ǉ�
			if (!laser.pLaser->IsReflect())
			{
				// ���ˉ��̍Đ�
				Sound::Manager::GetInstance()->PlaySE("Reflect");

				// ���˃��[�U�[��ǉ�
				int key = m_pLaserManager->AddReflectLaser(m_pEnemyManager, m_pPlayer->GetShield(), laser.pLaser, firePos);
				laser.pLaser->SetReflectLaserKey(key);
			}
			// ���ɔ��˃��[�U�[������Δ��˃��[�U�[�̈ʒu��ݒ�
			else
			{
				// ���˃��[�U�[�̈ʒu��ݒ�
				m_pLaserManager->SetLaserPos(laser.pLaser->GetReflectLaserKey(), firePos);
			}

			// �G�̃��[�U�[���~�߂�
			laser.pLaser->Reflect(m_pPlayer->GetShield()->GetPos());
		}
	}

	// �v���C���[�ƓG�̃��[�U�[�̓����蔻��
	for (auto& laser : m_pLaserManager->GetLaserList())
	{
		// ���˒����[�U�[�Ȃ画�肵�Ȃ�
		if (laser.pLaser->IsReflect()) continue;

		// ���[�U�[�̎�ނ����˃��[�U�[�Ȃ画�肵�Ȃ�
		if (laser.type == LaserType::REFLECT)	continue;

		// ���ƃ��b�V���̓����蔻��
		MV1_COLL_RESULT_POLY_DIM result{};
		result = MV1CollCheck_Sphere(
			laser.pLaser->GetModelHandle(),
			-1,
			m_pPlayer->GetPos().ToDxLibVector3(),
			m_pPlayer->GetCollisionRadius());

		// �������Ă�����
		if (result.HitNum > 0)
		{
			// �v���C���[�̃_���[�W����
			m_pPlayer->OnDamage(m_laserDamage);

			// �_���[�W�t���b�V���̉��o
			m_pDamageFlash->Start(60, 50, 0xff0000);

			// ��ʗh��̉��o
			m_pScreenShaker->StartShake({ m_laserDamage * 10.0f, m_laserDamage * 10.0f }, 30);

			// �������Ă������[�U�[���ʏ�̃��[�U�[�Ȃ�
			if (laser.type == LaserType::NORMAL)
			{
				// ���[�U�[���~�߂�
				laser.pLaser->Stop(m_pPlayer->GetPos());
			}
		}
		// �����蔻����̌�n��
		MV1CollResultPolyDimTerminate(result);
	}

	// ���˂������[�U�[�ƓG�̓����蔻��
	for (auto& laser : m_pLaserManager->GetLaserList())
	{
		// ���˒����[�U�[�łȂ���Δ��肵�Ȃ�
		if (laser.type != LaserType::REFLECT) continue;

		// �G���G
		for (auto& enemy : m_pEnemyManager->GetEnemyList())
		{
			// ���ƃ��b�V���̓����蔻��
			MV1_COLL_RESULT_POLY_DIM result{};
			result = MV1CollCheck_Sphere(
				laser.pLaser->GetModelHandle(),
				-1,
				enemy->GetPos().ToDxLibVector3(),
				enemy->GetCollisionRadius());

			// �������Ă�����
			if (result.HitNum > 0)
			{
				// �G�Ƀ_���[�W����
				enemy->OnDamage(m_playerToEnemyDamage, Math::Vector3::FromDxLibVector3(result.Dim->HitPosition));
			}
			// �����蔻����̌�n��
			MV1CollResultPolyDimTerminate(result);
		}
		// �{�X
		if (m_pEnemyManager->GetBossEnemy())
		{
			MV1_COLL_RESULT_POLY_DIM result{};
			result = MV1CollCheck_Sphere(
				laser.pLaser->GetModelHandle(),
				-1,
				m_pEnemyManager->GetBossEnemy()->GetPos().ToDxLibVector3(),
				m_pEnemyManager->GetBossEnemy()->GetCollisionRadius());

			// �������Ă�����
			if (result.HitNum > 0)
			{
				// �{�X�Ƀ_���[�W����
				m_pEnemyManager->GetBossEnemy()->OnDamage(
					m_playerToBossDamage, Math::Vector3::FromDxLibVector3(result.Dim->HitPosition));
			}
			// �����蔻����̌�n��
			MV1CollResultPolyDimTerminate(result);
		}
	}

	// �v���C���[�ƓG�̓����蔻��
	for (auto& enemy : m_pEnemyManager->GetEnemyList())
	{
		// ���Ƌ��̓����蔻��
		float distance = (enemy->GetPos() - m_pPlayer->GetPos()).Length();
		if (distance < enemy->GetCollisionRadius() + m_pPlayer->GetCollisionRadius())
		{
			// �v���C���[�̃_���[�W����
			m_pPlayer->OnDamage(m_enemyDamage);

			// �_���[�W�t���b�V���̉��o
			m_pDamageFlash->Start(60, 50, 0xff0000);

			// ��ʗh��̉��o
			m_pScreenShaker->StartShake({ m_enemyDamage * 10.0f, m_enemyDamage * 10.0f }, 30);
		}
	}
}