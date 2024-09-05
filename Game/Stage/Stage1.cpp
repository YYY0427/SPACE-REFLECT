#include "Stage1.h"
#include "../Scene/StageSelectScene.h"
#include "../Scene/SceneManager.h"
#include "../Editor/DataReaderFromUnity.h"
#include "../UI/UIManager.h"
#include "../UI/DamageFlash.h"
#include "../UI/ResultWindow.h"
#include "../Effect/ScreenShaker.h"
#include "../Effect/Effekseer3DEffectManager.h"
#include "../Game/Player.h"
#include "../Game/Camera.h"
#include "../Game/SkyDome.h"
#include "../Game/Shield.h"
#include "../Game/MeteorManager.h"
#include "../Game/PlanetManager.h"
#include "../Game/Laser/LaserManager.h"
#include "../Game/Laser/LaserBase.h"
#include "../Game/Enemy/EnemyManager.h"
#include "../Game/Enemy/EnemyBase.h"
#include "../Score/ScoreRanking.h"
#include "../Score/Score.h"
#include "../String/MessageManager.h"
#include "../MyDebug/DebugText.h"
#include "../UI/TutorialUI.h"
#include "../Util/InputState.h"
#include "../Transitor/Fade.h"
#include "../Sound/SoundManager.h"
#include <DxLib.h>

namespace
{
	// �I�u�W�F�N�g�z�u�f�[�^�̃t�@�C����
	const std::string object_data_file_name = "Stage1";

	// �G�̔z�u�f�[�^�̃t�@�C����
	const std::string enemy_data_file_name = "Stage1";

	// �E�F�[�u�̑ҋ@�t���[����
	constexpr int wave_wait_frame = 200;

	// �_���[�W
	constexpr int meteor_damage = 2;			 // 覐΂ɓ������Ă���ԂɃv���C���[�ɗ^����_���[�W
	constexpr int laser_damage = 1;				 // ���[�U�[�ɓ������Ă���ԂɃv���C���[�ɗ^����_���[�W
	constexpr int enemy_damage = 1;				 // �G�ɓ������Ă���ԂɃv���C���[�ɗ^����_���[�W
	constexpr int normal_enemy_reflect_laser_damage = 1000; // ���˃��[�U�[�ɓ������Ă���ԂɎG���G�ɗ^����_���[�W
	constexpr int boss_reflect_laser_damage = 2;			// ���˃��[�U�[�ɓ������Ă���ԂɃ{�X�ɗ^����_���[�W
}

// �R���X�g���N�^
Stage1::Stage1(SceneManager& manager) :
	StageBase(manager)
{
	// �_���[�W�̐ݒ�
	m_meteorDamage = meteor_damage;
	m_laserDamage = laser_damage;
	m_enemyDamage = enemy_damage;
	m_playerToEnemyDamage = normal_enemy_reflect_laser_damage;
	m_playerToBossDamage = boss_reflect_laser_damage;

	// �X�e�[�g�}�V���̐ݒ�
	m_stateMachine.AddState(State::START_ANIMATION, [this](){ EnterPlay(); }, [this]() { UpdateStartAnimation(); }, {});
	m_stateMachine.AddState(State::PLAY, {}, [this]() { UpdatePlay(); }, {});
	m_stateMachine.AddState(State::GAME_CLEAR, [this]() {EnterGameClear(); }, [this]() { UpdateGameClear(); }, {});
	m_stateMachine.AddState(State::GAME_OVER, [this]() {EnterGameOver(); }, [this]() { UpdateGameOver(); }, {});
	m_stateMachine.AddState(State::RESULT, [this]() { EnterResult(); }, [this]() { UpdateResult(); }, {});
	m_stateMachine.SetState(State::START_ANIMATION);

	// �I�u�W�F�N�g�z�u�f�[�^�ǂݍ���
	auto& dataReader = DataReaderFromUnity::GetInstance();
	dataReader.LoadUnityGameObjectData(object_data_file_name.c_str());

	// �C���X�^���X�̍쐬
	m_pPlayer = std::make_shared<Player>(object_data_file_name);
	m_pCamera = std::make_shared<Camera>(m_pPlayer->GetPos());
	m_pLaserManager = std::make_shared<LaserManager>(m_pPlayer, m_pCamera);
	m_pPlanetManager = std::make_shared<PlanetManager>(object_data_file_name);
	m_pMeteorManager = std::make_shared<MeteorManager>(object_data_file_name, m_pPlayer);
	m_pSkyDome = std::make_shared<SkyDome>(m_pCamera->GetPos());
	m_pScreenShaker = std::make_shared<Effect::ScreenShaker>(m_pCamera);
	m_pEnemyManager = std::make_shared<EnemyManager>(m_pPlayer, m_pLaserManager, m_pScreenShaker);

	// �|�C���^�̐ݒ�
	m_pPlayer->SetCameraPointer(m_pCamera);

	// UI�̃C���X�^���X�̍쐬
	m_pDamageFlash = std::make_shared<DamageFlash>();
	UIManager::GetInstance().AddUI("DamageFlash", m_pDamageFlash, 3, { 0, 0 });

	// �E�F�[�u�f�[�^�̓ǂݍ���
	m_pEnemyManager->LoadEnemyStageFileData(enemy_data_file_name);
}

// �f�X�g���N�^
Stage1::~Stage1()
{
}

// �v���C�X�e�[�g�̊J�n
void Stage1::EnterPlay()
{
	// BGM�̍Đ�
	Sound::Manager::GetInstance()->PlayBGM("TutorialBgm");
	Sound::Manager::GetInstance()->SetFadeSound("TutorialBgm", 120, 0, 255);
}

// ���U���g�X�e�[�g�̊J�n
void Stage1::EnterResult()
{
	// ���U���g��ʂ̃C���X�^���X����
	m_pResultWindow = std::make_shared<ResultWindow>();
}

// �Q�[���N���A�X�e�[�g�̊J�n
void Stage1::EnterGameClear()
{
	Sound::Manager::GetInstance()->PlaySE("GameClear");
}

// �Q�[���I�[�o�[�X�e�[�g�̊J�n
void Stage1::EnterGameOver()
{
	// BGM���Đ����Ȃ�
	const auto& soundManager = Sound::Manager::GetInstance();
	if (soundManager->IsPlayBGM())
	{
		// BGM�̃t�F�[�h�A�E�g
		auto fileName = soundManager->GetPlayBGMFileName();
		soundManager->SetFadeSound(fileName, 60, soundManager->GetSoundVolume(fileName), 0);
	}
}

// �ʏ�̍X�V
void Stage1::Update()
{
	// �v���C���[�����񂾂�
	if (!m_pPlayer->IsEnabled())
	{
		// �Q�[���I�[�o�[�ɑJ��
		m_stateMachine.SetState(State::GAME_OVER);
	}

	// �{�X�����S���o���Ȃ�覐΂̍폜
	if (m_pEnemyManager->IsDeadBossAnim())
	{
		m_pMeteorManager->DeleteAllMeteor();
	}	

	// �X�V
	m_pSkyDome->Update({ 0, 0, m_pCamera->GetPos().z });	// �X�J�C�h�[��
	m_pPlanetManager->UpdatePlay(m_pPlayer->GetMoveVec());	// �f��
	m_pLaserManager->Update();								// ���[�U�[
	m_pEnemyManager->Update();								// ���[�U�[
	m_pDamageFlash->Update();								// �_���[�W�t���b�V��
	m_pScreenShaker->Update();								// ��ʗh��
	Effect::Effekseer3DManager::GetInstance()->Update();		// �G�t�F�N�g
	UIManager::GetInstance().Update();						// UI

	Collision();										// �����蔻��
	m_pFade->Update();									// �t�F�[�h
	m_stateMachine.Update();							// �X�e�[�g�}�V��
}

// �X�^�[�g���o�X�e�[�g�̍X�V
void Stage1::UpdateStartAnimation()
{
	// �X�V
	m_pPlayer->UpdateStart(m_pCamera->GetPos());	// �v���C���[
	m_pCamera->UpdateStart(m_pPlayer->GetPos());	// �J����
	m_pMeteorManager->UpdateStart();				// 覐�

	// ������覐΂̐���
	m_pMeteorManager->CreateSmallMeteor(5000.0f);	

	// �X�^�[�g���o���I�������v���C���ɑJ��
	if (m_pPlayer->IsStartAnimation() &&
		m_pCamera->IsStartAnimation())
	{
		m_stateMachine.SetState(State::PLAY);
	}
}

// �v���C�X�e�[�g�̍X�V
void Stage1::UpdatePlay()
{
	// �{�X�����񂾂�Q�[���N���A�ɑJ��
	if (m_pEnemyManager->IsDeadBoosEndAnim())
	{
		m_stateMachine.SetState(State::GAME_CLEAR);
		return;
	}

	// ������覐΂̐���
	m_pMeteorManager->CreateSmallMeteor(5000.0f);

	// ���݂̓G�̃E�F�[�u���I��������
	if (m_pEnemyManager->IsEndWave())
	{
		if (m_currentFrame++ > wave_wait_frame)
		{
			// �E�F�[�u�J�n
	    	m_pEnemyManager->StartWave();

			// ���̃E�F�[�u�ɐi��
			m_pEnemyManager->NextWave();

			// �E�F�[�u�̑ҋ@�t���[���������Z�b�g
			m_currentFrame = 0;
		}
	}

	// �X�V
	m_pPlayer->UpdatePlay(m_pCamera->GetCameraHorizon());
	m_pCamera->UpdatePlay(m_pPlayer->GetPos(), m_pPlayer->GetMoveVec());
	m_pMeteorManager->Update(m_pCamera->GetPos());			// 覐�
}

// �Q�[���N���A�X�e�[�g�̍X�V
void Stage1::UpdateGameClear()
{
	// �S�Ẵ��[�U�[�̍폜
	m_pLaserManager->DeleteAllLaser();

	// �Q�[���N���A���̍X�V
	m_pPlayer->UpdateGameClear();
	m_pMeteorManager->Update(m_pCamera->GetPos());

	// �J�����̍X�V���I��������
	if (m_pCamera->UpdateGameClear(m_pPlayer->GetPos()))
	{
		// ���U���g��ʂɑJ��
		m_stateMachine.SetState(State::RESULT);
	}
}

// �Q�[���I�[�o�[�X�e�[�g�̍X�V
void Stage1::UpdateGameOver()
{
	// UI�̊i�[
	UIManager::GetInstance().Store();

	// �S�Ẵ��[�U�[�̍폜
	m_pLaserManager->DeleteAllLaser();

	// �Q�[���I�[�o�[���̍X�V
	m_pCamera->UpdateGameOver(m_pPlayer->GetPos());
	m_pMeteorManager->Update(m_pCamera->GetPos());
	if (m_pPlayer->UpdateGameOver())
	{
		// �t�F�[�h�A�E�g�̉��o�̊J�n
		m_pFade->StartFadeOut(255, 2);
	}
	// �t�F�[�h�A�E�g���I��������
	if (m_pFade->IsFadeOutEnd())
	{
		// �X�e�[�W�Z���N�g�ɑJ��
		m_manager.ChangeScene(std::make_shared<StageSelectScene>(m_manager));
	}

	// �f�o�b�O�e�L�X�g�̒ǉ�
	Debug::Text::AddLog("GameOver");
}

// ���U���g�X�e�[�g�̍X�V
void Stage1::UpdateResult()
{
	StageBase::UpdateResult("Stage1");
}

// �`��
void Stage1::Draw()
{
	// ��ʗh��̑O����
	m_pScreenShaker->Effect::ScreenShaker::PreDraw();

	// �`��
	m_pSkyDome->Draw();			// �X�J�C�h�[��
	m_pPlanetManager->Draw();	// �f��
	m_pMeteorManager->Draw();	// 覐�
	m_pEnemyManager->Draw();	// �G
	m_pLaserManager->Draw();	// ���[�U�[
	m_pPlayer->Draw();								// �v���C���[
	m_pPlayer->DrawShield();						// �V�[���h
	Effect::Effekseer3DManager::GetInstance()->Draw();	// �G�t�F�N�g
	UIManager::GetInstance().Draw();				// UI
	Score::GetInstance().DrawScore();				// �X�R�A

	// ���U���g��ʂ��J�n����Ă�����
	if (m_stateMachine.GetCurrentState() == State::RESULT)
	{
		// ���U���g��ʂ̕`��
		m_pResultWindow->Draw();
	}

	// ��ʗh��`��
	m_pScreenShaker->Draw();

	// �t�F�[�h�̕`��
	m_pFade->DrawFade(true);
}