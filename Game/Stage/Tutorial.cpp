#include "Tutorial.h"
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
#include "../Score/ScoreManager.h"
#include "../String/MessageManager.h"
#include "../MyDebug/DebugText.h"
#include "../UI/TutorialUI.h"
#include "../Input/InputManager.h"
#include "../Transitor/Fade.h"
#include "../Sound/SoundManager.h"
#include <DxLib.h>

namespace
{
	// �I�u�W�F�N�g�z�u�f�[�^�̃t�@�C����
	const std::string object_data_file_name = "Tutorial";

	// �G�̔z�u�f�[�^�̃t�@�C����
	const std::string enemy_data_file_name = "Tutorial";

	// �E�F�[�u�̑ҋ@�t���[����
	constexpr int wave_wait_frame = 200;

	// �ړ��`���[�g���A���ɑJ�ڂ���܂ł̃t���[����
	constexpr int move_tutorial_frame = 200;

	// �_���[�W
	constexpr int meteor_damage = 2;			 // 覐΂ɓ������Ă���ԂɃv���C���[�ɗ^����_���[�W
	constexpr int laser_damage = 1;				 // ���[�U�[�ɓ������Ă���ԂɃv���C���[�ɗ^����_���[�W
	constexpr int enemy_damage = 1;				 // �G�ɓ������Ă���ԂɃv���C���[�ɗ^����_���[�W
	constexpr int normal_enemy_reflect_laser_damage = 1000;  // ���˃��[�U�[�ɓ������Ă���Ԃɒʏ�̓G�ɗ^����_���[�W
	constexpr int boss_reflect_laser_damage = 2;			 // ���˃��[�U�[�ɓ������Ă���ԂɃ{�X�ɗ^����_���[�W
}

// �R���X�g���N�^
Tutorial::Tutorial(SceneManager& manager) :
	StageBase(manager)
{
	// �_���[�W�̐ݒ�
	m_meteorDamage= meteor_damage;
	m_laserDamage = laser_damage;
	m_enemyDamage = enemy_damage;
	m_playerToEnemyDamage = normal_enemy_reflect_laser_damage;
	m_playerToBossDamage = boss_reflect_laser_damage;

	// �X�e�[�g�}�V���̐ݒ�
	m_stateMachine.AddState(State::START_ANIMATION, {}, [this]() { UpdateStartAnimation(); }, {});
	m_stateMachine.AddState(State::MOVE_TUTORIAL, [this]() { EnterStartAnimation(); }, [this]() { UpdateMoveTutorial(); }, {});
	m_stateMachine.AddState(State::SHIELD_TUTORIAL, {}, [this]() { UpdateShieldTutorial(); }, {});
	m_stateMachine.AddState(State::REFLECT_TUTORIAL, {}, [this]() { UpdateReflectTutorial(); }, {});
	m_stateMachine.AddState(State::CUBE_TUTORIAL, {}, [this]() { UpdateCubeTutorial(); }, {});
	m_stateMachine.AddState(State::PLAY, {}, [this]() { UpdatePlay(); }, {});
	m_stateMachine.AddState(State::GAME_CLEAR, [this]() {EnterGameClear(); }, [this]() { UpdateGameClear(); }, {});
	m_stateMachine.AddState(State::GAME_OVER, {}, [this]() { UpdateGameOver(); }, {});
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
	m_pScreenShaker= std::make_shared<Effect::ScreenShaker>(m_pCamera);
	m_pEnemyManager = std::make_shared<EnemyManager>(m_pPlayer, m_pLaserManager, m_pScreenShaker);
	m_pTutorialUI = std::make_shared<TutorialUI>();

	// �|�C���^�̐ݒ�
	m_pPlayer->SetCameraPointer(m_pCamera);

	// UI�̃C���X�^���X�̍쐬
	m_pDamageFlash = std::make_shared<DamageFlash>();
	UI::Manager::GetInstance()->AddUI("DamageFlash", m_pDamageFlash, 3, { 0, 0 });

	// �E�F�[�u�f�[�^�̓ǂݍ���
	m_pEnemyManager->LoadEnemyStageFileData(enemy_data_file_name);
}

// �f�X�g���N�^
Tutorial::~Tutorial()
{
}

// �X�V
void Tutorial::Update()
{
	// �v���C���[�����񂾂�
	if (!m_pPlayer->IsEnabled())
	{
		// �Q�[���I�[�o�[�ɑJ��
		m_stateMachine.SetState(State::GAME_OVER);
	}

	// ������覐΂̐���
	m_pMeteorManager->CreateSmallMeteor(5000.0f);

	// �X�V
	m_pTutorialUI->Update();								// �`���[�g���A��UI
	m_pSkyDome->Update({ 0, 0, m_pCamera->GetPos().z });	// �X�J�C�h�[��
	m_pPlanetManager->UpdatePlay(m_pPlayer->GetMoveVec());	// �f��
	m_pLaserManager->Update();								// ���[�U�[
	m_pEnemyManager->Update();								// �G
	m_pDamageFlash->Update();								// �_���[�W�t���b�V��
	m_pScreenShaker->Update();								// ��ʗh��
	Effect::Effekseer3DManager::GetInstance()->Update();		// �G�t�F�N�g
	UI::Manager::GetInstance()->Update();						// UI

	Collision();										// �����蔻��
	m_pFade->Update();									// �t�F�[�h
	m_stateMachine.Update();							// �X�e�[�g�}�V��
}

// �X�^�[�g���o�̍X�V
void Tutorial::UpdateStartAnimation()
{
	// �X�V
	m_pPlayer->UpdateStart(m_pCamera->GetPos());	// �v���C���[
	m_pCamera->UpdateStart(m_pPlayer->GetPos());	// �J����
	m_pMeteorManager->UpdateStart();				// 覐�

	// �X�^�[�g���o���I�������ړ��`���[�g���A���ɑJ��
	if (m_pPlayer->IsStartAnimation() &&
		m_pCamera->IsStartAnimation())
	{
		m_stateMachine.SetState(State::MOVE_TUTORIAL);
	}
}

// �ړ��`���[�g���A���̍X�V
void Tutorial::UpdateMoveTutorial()
{
	// �X�V
	m_pPlayer->UpdatePlay(m_pCamera->GetCameraHorizon());					// �v���C���[
	m_pCamera->UpdatePlay(m_pPlayer->GetPos(), m_pPlayer->GetMoveVec());	// �J����
	m_pMeteorManager->Update(m_pCamera->GetPos());							// 覐�

	// ����̃t���[����������
	m_currentFrame++;
	if (m_currentFrame > wave_wait_frame + move_tutorial_frame)
	{
		// �ړ��`���[�g���A��UI�̊J�n
		m_pTutorialUI->StartState(TutorialState::MOVE);

		// �ړ��`���[�g���A��UI���I��������
		if (m_pTutorialUI->IsEndState(TutorialState::MOVE))
		{
			// �V�[���h�`���[�g���A���ɑJ��
			m_stateMachine.SetState(State::SHIELD_TUTORIAL);
			m_currentFrame = 0;
		}
	}
}

// �V�[���h�`���[�g���A���̍X�V
void Tutorial::UpdateShieldTutorial()
{
	// �X�V
	m_pPlayer->UpdatePlay(m_pCamera->GetCameraHorizon());					// �v���C���[	
	m_pCamera->UpdatePlay(m_pPlayer->GetPos(), m_pPlayer->GetMoveVec());	// �J����
	m_pMeteorManager->Update(m_pCamera->GetPos());							// 覐�

	// ����̃t���[����������
	m_currentFrame++;
	if (m_currentFrame > wave_wait_frame)
	{
		// �ړ��`���[�g���A��UI�̊J�n
		m_pTutorialUI->StartState(TutorialState::SHIELD);

		// �ړ��`���[�g���A��UI���I��������
		if (m_pTutorialUI->IsEndState(TutorialState::SHIELD))
		{
			// �V�[���h�`���[�g���A���ɑJ��
			m_stateMachine.SetState(State::REFLECT_TUTORIAL);
			m_currentFrame = 0;
		}
	}

	// �f�o�b�O�e�L�X�g�̒ǉ�
	Debug::Text::AddLog("ShieldTutorial");
}

// ���˃`���[�g���A���̍X�V
void Tutorial::UpdateReflectTutorial()
{
	// �X�V
	m_pPlayer->UpdatePlay(m_pCamera->GetCameraHorizon());					// �v���C���[	
	m_pCamera->UpdatePlay(m_pPlayer->GetPos(), m_pPlayer->GetMoveVec());	// �J����
	m_pMeteorManager->Update(m_pCamera->GetPos());							// 覐�

	// ����̃t���[����������
	m_currentFrame++;
	if (m_currentFrame > wave_wait_frame)
	{
		// ���˃`���[�g���A��UI�̊J�n
		m_pTutorialUI->StartState(TutorialState::REFLECT);

		// ���˃`���[�g���A��UI���I��������
		if (m_pTutorialUI->IsEndState(TutorialState::REFLECT))
		{
			// ���˃`���[�g���A��UI�̊J�n
			m_pTutorialUI->StartState(TutorialState::REFLECT2);

			// �G�̃E�F�[�u�J�n
			m_pEnemyManager->StartWave();
		}
	}
	// ���݂̓G�̃E�F�[�u���I��������
	if (m_pEnemyManager->IsEndWave() && m_pTutorialUI->IsEndState(TutorialState::REFLECT))
	{
		// ���˃`���[�g���A�����I��
		m_pTutorialUI->EndState();
		if (m_pTutorialUI->IsEndState(TutorialState::REFLECT2))
		{
			// �V�[���h�`���[�g���A���ɑJ��
			m_stateMachine.SetState(State::CUBE_TUTORIAL);
			m_currentFrame = 0;
		}
	}
	// �f�o�b�O�e�L�X�g�̕`��
	Debug::Text::AddLog("ReflectTutorial");
}

// �L���[�u�`���[�g���A���̍X�V
void Tutorial::UpdateCubeTutorial()
{
	// �X�V
	m_pPlayer->UpdatePlay(m_pCamera->GetCameraHorizon());					// �v���C���[
	m_pCamera->UpdatePlay(m_pPlayer->GetPos(), m_pPlayer->GetMoveVec());	// �J����
	m_pMeteorManager->Update(m_pCamera->GetPos());							// 覐�

	// ����̃t���[����������
	m_currentFrame++;
	if (m_currentFrame > wave_wait_frame)
	{
		// �L���[�u�`���[�g���A��UI�̊J�n
		m_pTutorialUI->StartState(TutorialState::CUBE);

		if (m_pTutorialUI->IsEndState(TutorialState::CUBE))
		{
			// �L���[�u�`���[�g���A��UI�̊J�n
			m_pTutorialUI->StartState(TutorialState::CUBE2);

			if (!m_isWaveStart)
			{
				m_pEnemyManager->NextWave();
				m_isWaveStart = true;
			}
		}
	}
	if (m_pEnemyManager->IsEndWave() && m_pTutorialUI->IsEndState(TutorialState::CUBE))
	{
		// �L���[�u�`���[�g���A�����I��
		m_pTutorialUI->EndState();
		if (m_pTutorialUI->IsEndState(TutorialState::CUBE2))
		{
			// �V�[���h�`���[�g���A���ɑJ��
			m_stateMachine.SetState(State::PLAY);
			m_currentFrame = 0;
			m_isWaveStart = false;
		}
	}

	// �f�o�b�O�e�L�X�g�̒ǉ�
	Debug::Text::AddLog("CubeTutorial");
}

// �v���C���̍X�V
void Tutorial::UpdatePlay()
{
	// �X�V
	m_pPlayer->UpdatePlay(m_pCamera->GetCameraHorizon());					// �v���C���[
	m_pCamera->UpdatePlay(m_pPlayer->GetPos(), m_pPlayer->GetMoveVec());	// �J����
	m_pMeteorManager->Update(m_pCamera->GetPos());							// 覐�

	// �E�F�[�u�̑ҋ@�t���[�������߂�����
	m_currentFrame++;
	if (m_currentFrame > wave_wait_frame)
	{
		// ���H�`���[�g���A��UI�̊J�n
		m_pTutorialUI->StartState(TutorialState::PLAY);

		// �E�F�[�u���J�n����Ă��Ȃ�������
		if (!m_isWaveStart)
		{
			// �E�F�[�u�J�n
			m_pEnemyManager->NextWave();

			// �E�F�[�u���J�n���ꂽ�t���O�𗧂Ă�
			m_isWaveStart = true;
		}
	}
	// ���݂̓G�̃E�F�[�u���I��������
	if (m_pEnemyManager->IsEndWave())
	{
		// �L���[�u�`���[�g���A�����I��
		m_pTutorialUI->EndState();

		// ���H�`���[�g���A��UI���I��������
		if (m_pTutorialUI->IsEndState(TutorialState::PLAY))
		{
			// ���ݗ���Ă���BGM�̃t�F�[�h�A�E�g
			const auto& soundManager = Sound::Manager::GetInstance();
			auto fileName = soundManager->GetPlayBGMFileName();
			soundManager->SetFadeSound(fileName, 60, soundManager->GetSoundVolume(fileName), 0);

			// �V�[���h�`���[�g���A���ɑJ��
			m_stateMachine.SetState(State::GAME_CLEAR);
		}
	}
}

// �Q�[���N���A�̍X�V
void Tutorial::UpdateGameClear()
{
	// UI�̊i�[
	UI::Manager::GetInstance()->Store();

	// �S�Ẵ��[�U�[�̍폜
	m_pLaserManager->DeleteAllLaser();

	// �X�V
	m_pPlayer->UpdateGameClear();
	m_pMeteorManager->Update(m_pCamera->GetPos());

	// �J�����̍X�V���I��������
	if (m_pCamera->UpdateGameClear(m_pPlayer->GetPos()))
	{
		// ���U���g��ʂɑJ��
		m_stateMachine.SetState(State::RESULT);
	}
}

// �Q�[���I�[�o�[�̍X�V
void Tutorial::UpdateGameOver()
{
	// UI�̊i�[
	UI::Manager::GetInstance()->Store();

	// �S�Ẵ��[�U�[�̍폜
	m_pLaserManager->DeleteAllLaser();

	// �Q�[���I�[�o�[���̍X�V
	m_pCamera->UpdateGameOver(m_pPlayer->GetPos());
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
		return;
	}

	// �f�o�b�O�e�L�X�g�̒ǉ�
	Debug::Text::AddLog("GameOver");
}

// �X�^�[�g�A�j���[�V�����̊J�n
void Tutorial::EnterStartAnimation()
{
	// BGM�̍Đ�
	Sound::Manager::GetInstance()->PlayBGM("TutorialBgm");
	Sound::Manager::GetInstance()->SetFadeSound("TutorialBgm", 120, 0, 255);
}

void Tutorial::EnterGameClear()
{
	Sound::Manager::GetInstance()->PlaySE("GameClear");
}

// �Q�[���I�[�o�[�̊J�n
void Tutorial::EnterGameOver()
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

// ���U���g�̊J�n
void Tutorial::EnterResult()
{
	// ���U���g��ʂ̃C���X�^���X����
	m_pResultWindow = std::make_shared<ResultWindow>();
}

// ���U���g�̍X�V
void Tutorial::UpdateResult()
{
	StageBase::UpdateResult("Tutorial");
}

// �`��
void Tutorial::Draw()
{
	// ��ʗh��̑O����
	m_pScreenShaker->PreDraw();

	// �`��
	m_pSkyDome->Draw();			// �X�J�C�h�[��
	m_pPlanetManager->Draw();	// �f��
	m_pMeteorManager->Draw();	// 覐�
	m_pEnemyManager->Draw();	// �G
	m_pLaserManager->Draw();	// ���[�U�[
	m_pPlayer->Draw();									// �v���C���[
	m_pPlayer->DrawShield();							// �V�[���h
	Effect::Effekseer3DManager::GetInstance()->Draw();	// �G�t�F�N�g
	UI::Manager::GetInstance()->Draw();					// UI
	m_pTutorialUI->Draw();								// �`���[�g���A��UI
	Score::Manager::GetInstance()->DrawScore();			// �X�R�A

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