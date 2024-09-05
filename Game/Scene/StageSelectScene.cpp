#include "StageSelectScene.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "OptionScene.h"
#include "DebugScene.h"
#include "TitleScene.h"
#include "../Transitor/FadeTransitor.h"
#include "../Transitor/Fade.h"
#include "../String/MessageManager.h"
#include "../MyDebug/DebugText.h"
#include "../Input/InputManager.h"
#include "../Util/Easing.h"
#include "../Util/DrawFunctions.h"
#include "../Game/Camera.h"
#include "../Game/PlanetManager.h"
#include "../Game/SkyDome.h"
#include "../Editor/DataReaderFromUnity.h"
#include "../Math/Vector3.h"
#include "../Application.h"
#include "../ModelHandleManager.h"
#include "../Sound/SoundManager.h"
#include <DxLib.h>

namespace
{
	// �I�u�W�F�N�g�̃t�@�C���p�X
	const std::string object_file_name  = "StageSelect";

	// �摜�t�@�C���p�X
	const std::string xbox_rb_file_path = "Data/Image/xbox_rb.png";
	const std::string xbox_lb_file_path = "Data/Image/xbox_lb.png";
	const std::string xbox_b_file_path  = "Data/Image/xbox_button_b.png";
	const std::string xbox_a_file_path  = "Data/Image/xbox_button_a.png";

	// �X�R�A�����L���O
	constexpr int score_ranking_alpha_add_speed = 15;	// �A���t�@�l
	constexpr int score_ranking_space           = 20;	// �����Ԋu

	// �����E�B���h�E
	const Math::Vector2 explanation_window_size = { 400, 425 };	// �E�B���h�E�̃T�C�Y
	const Math::Vector2 window_scale_frame      = { 15, 20 };		// �E�B���h�E�̊g��̃t���[��
	constexpr int window_max_alpha        = 180;			// �E�B���h�E�̍ő�A���t�@�l

	// �X�e�[�W�I�����ɃJ�����̈ړ��ɂ�����t���[��
	constexpr int camera_move_frame = 40;

	// �X�e�[�W���莞
	constexpr int camera_move_frame_to_planet = 150;	// �f���������܂ł̃t���[��
	constexpr int camera_move_frame_to_stage  = 300;	// �J�����̈ړ��ɂ�����t���[��
}

// �R���X�g���N�^
StageSelectScene::StageSelectScene(SceneManager& manager) :
	SceneBase(manager),
	m_currentSelectItem(0),
	m_easeTime(0),
	m_isInput(false),
	m_line3DAlpha(0),
	m_textAlpha(0),
	m_rbButtonImgHandle(-1),
	m_lbButtonImgHandle(-1),
	m_bButtonImgHandle(-1),
	m_lightHandle(-1),
	m_uiAlpha(255),
	m_easeTime2(0),
	m_explanationWindowSize(0, 0),
	m_windowAlpha(window_max_alpha),
	m_isStartAnimSE(false),
	m_aButtonImgHandle(-1),
	m_screenHandle(-1),
	m_selectStageCameraEasingTime(0),
	m_decisionStageTargetEasingTime(0),
	m_decisionStageCameraEasingTime(0)
{
}

// �f�X�g���N�^
StageSelectScene::~StageSelectScene()
{
}

// ������
void StageSelectScene::Init()
{
	// �X�e�[�g�}�V���̐ݒ�
	m_stateMachine.AddState(State::STAGE_SELECT, {}, [this]() {UpdateSelectStage(); }, {});
	m_stateMachine.AddState(State::START_ANIMATION, [this]() {EnterStartAnimation(); }, [this]() {UpdateStartAnimation(); }, {});
	m_stateMachine.SetState(State::STAGE_SELECT);

	// ���C�g�̐ݒ�
	m_lightHandle = CreateDirLightHandle({ -1, -1, 1 });
	SetLightDifColorHandle(m_lightHandle, GetColorF(1.0f, 1.0f, 1.0f, 0.0f));

	// �t�F�[�h�C���̉��o
	m_pFade = std::make_unique<Fade>();
	m_pFade->StartFadeIn(0);

	// �I�u�W�F�N�g�̃f�[�^��ǂݍ���
	DataReaderFromUnity::GetInstance().LoadUnityGameObjectData(object_file_name);

	// �C���X�^���X�쐬
	m_pPlanetManager = std::make_unique<PlanetManager>(object_file_name);

	// �X�e�[�W�̐ݒ�
	m_stageData[Stage::TUTORIAL].stageNameId = "Tutorial";
	m_stageData[Stage::TUTORIAL].missionNameId = "TutorialMissionName";
	m_stageData[Stage::TUTORIAL].difficultyId = "TutorialDifficulty";
	m_stageData[Stage::TUTORIAL].conditionsId = "TutorialConditions";
	m_stageData[Stage::TUTORIAL].cameraPos = DataReaderFromUnity::GetInstance().GetData(object_file_name, "MoonCamera")[0].pos;
	m_stageData[Stage::TUTORIAL].pPlanet = m_pPlanetManager->GetPlanet(PlanetType::MOON);

	m_stageData[Stage::STAGE_1].stageNameId = "Stage1";
	m_stageData[Stage::STAGE_1].missionNameId = "EarthMissionName";
	m_stageData[Stage::STAGE_1].conditionsId = "EarthConditions";
	m_stageData[Stage::STAGE_1].difficultyId = "EarthDifficulty";
	m_stageData[Stage::STAGE_1].cameraPos = DataReaderFromUnity::GetInstance().GetData(object_file_name, "EarthCamera")[0].pos;
	m_stageData[Stage::STAGE_1].pPlanet = m_pPlanetManager->GetPlanet(PlanetType::EARTH);

	// �C���X�^���X�쐬
	Math::Vector3 pos = m_stageData[static_cast<Stage>(m_currentSelectItem)].cameraPos;
	m_pCamera = std::make_unique<Camera>();
	m_pCamera->SetCamera(pos, Math::Vector3{ pos.x, pos.y, pos.z + 10.0f });
	m_pSkyDome = std::make_unique<SkyDome>(m_pCamera->GetPos());

	// �X�N���[���̍쐬
	m_screenHandle = MakeScreen(
		Application::GetInstance()->GetWindowSize().width,
		Application::GetInstance()->GetWindowSize().height, true);

	// �摜�̓ǂݍ���
	m_rbButtonImgHandle = my::MyLoadGraph(xbox_rb_file_path.c_str());
	m_lbButtonImgHandle = my::MyLoadGraph(xbox_lb_file_path.c_str());
	m_bButtonImgHandle = my::MyLoadGraph(xbox_b_file_path.c_str());
	m_aButtonImgHandle = my::MyLoadGraph(xbox_a_file_path.c_str());

	// ������
	m_cameraStartPos = m_pCamera->GetPos();
	m_cameraGoalPos = m_stageData[static_cast<Stage>(m_currentSelectItem)].cameraPos;

	// �X�R�A�����L���O�̏�����
	if (Score::Ranking::GetInstance()->GetScoreData(m_stageData[static_cast<Stage>(m_currentSelectItem)].stageNameId).empty())
	{
		Score::Ranking::GetInstance()->CreateNewScoreData(m_stageData[static_cast<Stage>(m_currentSelectItem)].stageNameId);
	}
	// �A���t�@�l�̏�����
	for (int i = 0; i < Score::Ranking::GetInstance()->GetScoreData(m_stageData[static_cast<Stage>(m_currentSelectItem)].stageNameId).size(); i++)
	{
		m_rankingAlpha.push_back(0);
	}

	// BGM�̍Đ�
	Sound::Manager::GetInstance()->PlayBGM("StageSelectBgm");
	Sound::Manager::GetInstance()->SetFadeSound("StageSelectBgm", 60, 0, 255);
}

// �I������
void StageSelectScene::End()
{
	// ���C�g�̃n���h�����폜
	DeleteLightHandle(m_lightHandle);

	// BGM�̒�~
	Sound::Manager::GetInstance()->StopSound("StageSelectBgm");

	// �摜�̍폜
	DeleteGraph(m_rbButtonImgHandle);
	DeleteGraph(m_lbButtonImgHandle);
	DeleteGraph(m_bButtonImgHandle);
	DeleteGraph(m_aButtonImgHandle);
}

// �X�e�[�W�I�����̏�����
void StageSelectScene::SelectStageProcess()
{
	// ������
	m_isInput = true;
	m_line3DAlpha = 0;
	m_textAlpha = 0;
	m_explanationWindowEasingTime = { 0, 0 };
	m_explanationWindowSize = { 0, 0 };
	for (auto& alpha : m_rankingAlpha)
	{
		alpha = 0;
	}
}

// �X�^�[�g���o�̊J�n
void StageSelectScene::EnterStartAnimation()
{
	m_cameraStartTargetPos = m_pCamera->GetTarget();
	m_cameraStartPos = m_pCamera->GetPos();
}

// �X�V
void StageSelectScene::Update()
{
	// �f�o�b�O�e�L�X�g
	Debug::Text::AddLog("cameraPos", { m_pCamera->GetPos().x, m_pCamera->GetPos().y ,m_pCamera->GetPos().z });

	// �t�F�[�h�̍X�V
	m_pFade->Update();

	// �X�e�[�g�}�V���̍X�V
	m_stateMachine.Update();
}

// �X�e�[�W�I���̍X�V
void StageSelectScene::UpdateSelectStage()
{
	// ������
	m_isInput = false;

	// �C�[�W���O���I�����Ă�����͂��󂯕t����
	if (m_selectStageCameraEasingTime >= camera_move_frame)
	{
		// �I�������񂷏���
		int sceneItemTotalValue = static_cast<int>(Stage::NUM);
		if (Input::Manager::IsTriggered(Input::Type::RIGHT))
		{
			m_currentSelectItem = ((m_currentSelectItem - 1) + sceneItemTotalValue) % sceneItemTotalValue;
			SelectStageProcess();
		}
		else if (Input::Manager::IsTriggered(Input::Type::LEFT))
		{
			m_currentSelectItem = (m_currentSelectItem + 1) % sceneItemTotalValue;
			SelectStageProcess();
		}
	}

	// �X�V
	UpdateCamera();
	m_pPlanetManager->UpdateStageSelect();
	m_pSkyDome      ->Update(m_pCamera->GetPos());

	// �����E�B���h�E�̕������X�ɍL����
	m_explanationWindowEasingTime.x = (std::min)(++m_explanationWindowEasingTime.x, window_scale_frame.x);
	m_explanationWindowSize.x = Easing::EaseOutCubic(m_explanationWindowEasingTime.x, window_scale_frame.x, explanation_window_size.x, 0);
	if (m_explanationWindowSize.x < explanation_window_size.x)
	{
		Sound::Manager::GetInstance()->PlaySE("StageSelectOpenMenu");
	}

	// 3D�̐��̃A���t�@�l�̍X�V
	m_line3DAlpha = (std::min)(m_line3DAlpha += 6, 255);

	// �ő�T�C�Y�ɒB�����獂�����L����
	if (m_explanationWindowSize.x >= explanation_window_size.x)
	{
		// �����E�B���h�E�̍��������X�ɍL����
		m_explanationWindowEasingTime.y = (std::min)(++m_explanationWindowEasingTime.y, window_scale_frame.y);
		m_explanationWindowSize.y = Easing::EaseOutCubic(m_explanationWindowEasingTime.y, window_scale_frame.y, explanation_window_size.y, 0);

		// �ő�T�C�Y�ɒB������A���t�@�l���グ��
		if (m_explanationWindowSize.y >= explanation_window_size.y)
		{
			// �E�B���h�E�̃A���t�@�l�̍X�V
			m_textAlpha = (std::min)(m_textAlpha += 6, 255);

			// �X�R�A�����L���O�̃A���t�@�l�̍X�V
			UpdateRankingAlpha();
		}
	}

	// �I�v�V������ʂɑJ��
	if (Input::Manager::IsTriggered(Input::Type::RIGHT_SHOULDER))
	{
		// SE�̍Đ�
		Sound::Manager::GetInstance()->PlaySE("Select");

		// �V�[���̑J��
		m_manager.PushScene(std::make_shared<OptionScene>(m_manager, OptionScene::State::STAGE_SELECT));
		return;
	}

	// ����{�^���������ꂽ��X�^�[�g���o�ɑJ��
	if (Input::Manager::IsTriggered(Input::Type::DECISION))
	{
		// �X�e�[�g�}�V���̐ݒ�
		m_stateMachine.SetState(State::START_ANIMATION);

		// SE�̍Đ�
		const auto& soundManager = Sound::Manager::GetInstance();
		soundManager->PlaySE("Enter");

		// BGM�̃t�F�[�h�A�E�g�̐ݒ�
		soundManager->SetFadeSound("StageSelectBgm", 60, soundManager->GetSoundVolume("StageSelectBgm"), 0);
		return;
	}

	// �L�����Z���{�^���������ꂽ��
	if (Input::Manager::IsTriggered(Input::Type::BACK))
	{
		// �t�F�[�h�A�E�g�̉��o�̊J�n
		m_pFade->StartFadeOut(255, 10);

		// BGM�̃t�F�[�h�A�E�g
		const auto& soundManager = Sound::Manager::GetInstance();
		soundManager->SetFadeSound("StageSelectBgm", 60, soundManager->GetSoundVolume("StageSelectBgm"), 0);
	}

	// �t�F�[�h�A�E�g���I��������V�[���J��
	if (m_pFade->IsFadeOutEnd())
	{
		// �^�C�g����ʂɖ߂�
		m_manager.ChangeScene(std::make_shared<TitleScene>(m_manager));
		return;
	}
}

// �X�^�[�g���o�̍X�V			
void StageSelectScene::UpdateStartAnimation()
{
	// UI�S�̂̃A���t�@�l��������
	m_uiAlpha -= 10;

	// �f���̍X�V
	m_pPlanetManager->UpdateStageSelect();

	// �C�[�W���O�ŃJ�����̒����_���ړ�
	m_decisionStageTargetEasingTime++;
	m_decisionStageTargetEasingTime = (std::min)(m_decisionStageTargetEasingTime, camera_move_frame_to_planet);
	Math::Vector3 goalPos = m_stageData[static_cast<Stage>(m_currentSelectItem)].pPlanet->GetPos();
	float x = Easing::EaseOutCubic(m_decisionStageTargetEasingTime, camera_move_frame_to_planet, goalPos.x, m_cameraStartTargetPos.x);
	float y = Easing::EaseOutCubic(m_decisionStageTargetEasingTime, camera_move_frame_to_planet, goalPos.y, m_cameraStartTargetPos.y);
	m_pCamera->SetCamera(m_pCamera->GetPos(), { x, y, goalPos.z });

	// �J�����̒����_���f���ɓ��B������
	if (m_decisionStageTargetEasingTime >= camera_move_frame_to_planet)
	{
		// �C���X�^���X�̎擾
		const auto& soundManager = Sound::Manager::GetInstance();

		// �X�^�[�g�A�j���[�V������SE���Đ����Ă��Ȃ��ꍇ
		if (!m_isStartAnimSE)
		{
			// �X�^�[�g�A�j���[�V������SE���Đ�
			soundManager->PlaySE("StartAnimSe");

			// �X�^�[�g�A�j���[�V������SE�̃t�F�[�h�C���̐ݒ�
			soundManager->SetFadeSound("StartAnimSe", 30, soundManager->GetSoundVolume("StartAnimSe"), 255);

			// �Đ������t���O�𗧂Ă�
			m_isStartAnimSE = true;
		}
		
		// �C�[�W���O�ŃJ�������ړ�
		m_decisionStageCameraEasingTime++;
		m_decisionStageCameraEasingTime = (std::min)(m_decisionStageCameraEasingTime, camera_move_frame_to_stage);
		Math::Vector3 goalPos = m_stageData[static_cast<Stage>(m_currentSelectItem)].pPlanet->GetPos();
		float posX = Easing::EaseOutCubic(m_decisionStageCameraEasingTime, camera_move_frame_to_stage, goalPos.x, m_cameraStartPos.x);
		float posY = Easing::EaseOutCubic(m_decisionStageCameraEasingTime, camera_move_frame_to_stage, goalPos.y, m_cameraStartPos.y);
		float posZ = Easing::EaseOutCubic(m_decisionStageCameraEasingTime, camera_move_frame_to_stage, goalPos.z, m_cameraStartPos.z);
		m_pCamera->SetCamera({ posX, posY, posZ }, { m_pCamera->GetTarget().x, m_pCamera->GetTarget().y, goalPos.z });

		if (m_decisionStageCameraEasingTime >= 60)
		{
			// �t�F�[�h�A�E�g�̉��o�̊J�n
			m_pFade->StartFadeOut(255, 10);

			// �X�^�[�g�A�j���[�V������SE�̃t�F�[�h�A�E�g�̐ݒ�
			soundManager->SetFadeSound("StartAnimSe", 60, soundManager->GetSoundVolume("StartAnimSe"), 0);
		}
	}

	// �t�F�[�h�A�E�g���I�������玟�̃V�[���ɑJ��
	if (m_pFade->IsFadeOutEnd())
	{
		// �Q�[���V�[���ɑJ��
		m_manager.ChangeScene(
			std::make_shared<GameScene>(m_manager, static_cast<Stage>(m_currentSelectItem)));
		return;
	}
}

// �J�����̍X�V
void StageSelectScene::UpdateCamera()
{
	// ���͂�������
	// ���݂̃C�[�W���O���I�����Ă�����
	if (m_isInput && m_selectStageCameraEasingTime >= camera_move_frame)
	{
		// �J�����̖ړI�n��ݒ�
		m_cameraGoalPos = m_stageData[static_cast<Stage>(m_currentSelectItem)].cameraPos;

		// ������
		m_cameraStartPos = m_pCamera->GetPos();
		m_selectStageCameraEasingTime = 0;
	}

	// �C�[�W���O�ŃJ�������ړ�
	m_selectStageCameraEasingTime++;
	m_selectStageCameraEasingTime = (std::min)(m_selectStageCameraEasingTime, camera_move_frame);
	float x = Easing::EaseOutCubic(m_selectStageCameraEasingTime, camera_move_frame, m_cameraGoalPos.x, m_cameraStartPos.x);
	float y = Easing::EaseOutCubic(m_selectStageCameraEasingTime, camera_move_frame, m_cameraGoalPos.y, m_cameraStartPos.y);
	float z = Easing::EaseOutCubic(m_selectStageCameraEasingTime, camera_move_frame, m_cameraGoalPos.z, m_cameraStartPos.z);
	m_pCamera->SetCamera({ x, y, z }, { x, y, z + 10.0f });
}

// �����L���O�̃A���t�@�l�̍X�V
void StageSelectScene::UpdateRankingAlpha()
{
	// �X�R�A�����L���O�̃A���t�@�l�̍X�V
	// ��ԉ��̏��ʂ�255�ɂȂ����玟�̏��ʂ��X�V
	for (int i = m_rankingAlpha.size() - 1; i >= 0; i--)
	{
		if (i == m_rankingAlpha.size() - 1)
		{
			m_rankingAlpha[i] += score_ranking_alpha_add_speed;
		}
		else
		{
			if (m_rankingAlpha[i + 1] >= 255)
			{
				m_rankingAlpha[i] += score_ranking_alpha_add_speed;
			}
		}
	}
}

// �`��
void StageSelectScene::Draw()
{
	// ��ʂ��N���A
	ClearDrawScreen();

	// �`����ύX
	SetDrawScreen(m_screenHandle);

	// ��ʂ��N���A
	ClearDrawScreen();

	const auto& messageManager = String::MessageManager::GetInstance();

	// �X�e�[�W�Z���N�g�^�C�g���̕`��
	auto& screenSize = Application::GetInstance()->GetWindowSize();
	DrawRoundRectAA((screenSize.width / 2.0f) - 325, 50, (screenSize.width / 2.0f) - 50, 110, 5, 5, 8, 0xffffff, true);
	messageManager->DrawStringCenter("MissionTitle", (screenSize.width / 2.0f) - 187, 80, 0x000000);

	// RB�{�^���̕`��
	DrawRotaGraph((screenSize.width / 2.0f) + 375, 95, 1.0f, 0.0f, m_rbButtonImgHandle, true);

	// �I�v�V�����^�C�g���̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawRoundRectAA((screenSize.width / 2.0f) + 325, 50, (screenSize.width / 2.0f) + 50, 110, 5, 5, 8, 0xffffff, true);
	messageManager->DrawStringCenter("OptionTitle", (screenSize.width / 2.0f) + 187, 80, 0x000000);
	// LB�{�^���̕`��
	DrawRotaGraph((screenSize.width / 2.0f) - 375, 95, 1.0f, 0.0f, m_lbButtonImgHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
	// B�{�^���̕`��
	DrawRotaGraph(screenSize.width - 180, screenSize.height - 50, 1.0, 0.0f, m_bButtonImgHandle, true);
	messageManager->DrawStringCenter("StageSelectBack", screenSize.width - 100, screenSize.height - 50, 0xffffff);

	// A�{�^���̕`��
	DrawRotaGraph(screenSize.width / 2 - 120, screenSize.height - 75, 1.0, 0.0f, m_aButtonImgHandle, true);
	messageManager->DrawStringCenter("StageSelectStart", screenSize.width / 2, screenSize.height - 75, 0xffffff);

	// �O�p�`�̕`��
	DrawTriangleAA(screenSize.width - 50, screenSize.height / 2.0f, screenSize.width - 100, screenSize.height / 2.0f - 75, screenSize.width - 100, screenSize.height / 2.0f + 75, 0xffffff, true);
	DrawTriangleAA(0 + 50, screenSize.height / 2.0f, 0 + 100, screenSize.height / 2.0f - 75, 0 + 100, screenSize.height / 2.0f + 75, 0xffffff, true);

	// ���j���[�ƃ��C����ʂ���؂���̕`��
	DrawLineAA(0 + 100, 120, screenSize.width - 100, 120, 0xffffff, 3.0f);

	// �f����������E�B���h�E�܂ł̐��̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_line3DAlpha);
	SetUseLighting(FALSE);
	SetUseZBuffer3D(FALSE);
	DrawCone3D(m_stageData[static_cast<Stage>(m_currentSelectItem)].pPlanet->GetPos().ToDxLibVector3(),
		ConvScreenPosToWorldPos_ZLinear({ screenSize.width / 2.0f + 49, 153, 0.02f }), 3.0f, 8, 0xffffff, 0xffffff, true);
	SetUseZBuffer3D(TRUE);
	SetUseLighting(TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �����E�B���h�E�̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_windowAlpha);
	DrawRoundRectAA(890 - (m_explanationWindowSize.x / 2), 
					362 - (m_explanationWindowSize.y / 2), 
					890 + (m_explanationWindowSize.x / 2), 
					362 + (m_explanationWindowSize.y / 2), 
					5, 5, 4, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_line3DAlpha);
	DrawRoundRectAA(890 - (m_explanationWindowSize.x / 2),
					362 - (m_explanationWindowSize.y / 2),
					890 + (m_explanationWindowSize.x / 2),
					362 + (m_explanationWindowSize.y / 2),
					5, 5, 4, 0xffffff, false, 5.0f);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_textAlpha);

	// �X�e�[�W�^�C�g���̕`��
	messageManager->DrawStringCenter("StageSelectMission", 890, 200, 0xffffff);
	messageManager->DrawStringCenter(m_stageData[static_cast<Stage>(m_currentSelectItem)].missionNameId, 890, 250, 0xffffff);

	// ���̕`��
	DrawLine(screenSize.width / 2.0f + 100, 280,  screenSize.width / 2.0f + 400, 280, 0xffffff, 2.0f);

	// ��Փx�̕`��
	messageManager->DrawString(m_stageData[static_cast<Stage>(m_currentSelectItem)].difficultyId, 790, 310, 0xffffff);
	messageManager->DrawString(m_stageData[static_cast<Stage>(m_currentSelectItem)].conditionsId, 790, 350, 0xffffff);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �X�R�A�����L���O�̕`��
	DrawScoreRanking();
	SetDrawScreen(DX_SCREEN_BACK);

	// ���f���`��
	m_pCamera->SetCamera();
	m_pSkyDome->Draw();
	m_pPlanetManager->Draw();

	// UI�̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_uiAlpha);
	DrawGraph(0, 0, m_screenHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �t�F�[�h�̕`��
	m_pFade->DrawFade(true);
}

// �X�R�A�����L���O�̕`��
void StageSelectScene::DrawScoreRanking()
{
	// �X�R�A�����L���O�̎擾
	m_scoreRanking = Score::Ranking::GetInstance()->GetScoreData(m_stageData[static_cast<Stage>(m_currentSelectItem)].stageNameId);

	// �X�R�A�����L���O���󂾂�����V�K�쐬
	if (m_scoreRanking.empty())
	{
		Score::Ranking::GetInstance()->CreateNewScoreData(m_stageData[static_cast<Stage>(m_currentSelectItem)].stageNameId);
	}

	// ���b�Z�[�W�}�l�[�W���̎擾
	const auto& messageManager = String::MessageManager::GetInstance();

	// �����L���O�^�C�g���̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_textAlpha);
	messageManager->DrawStringCenter("RankingTitle", 805, 425, 0xffffff);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �X�R�A�����L���O�̕`��
	// �t����`��
	for(int i = m_scoreRanking.size() - 1; i >= 0; i--)
	{
		// �t�H���g�n���h���̎擾
		auto fontHandle = messageManager->GetMessageData("RankingFont").fontHandle;

		// ���X�ɕ`��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_rankingAlpha[i]);

		// ���ʂ̕�����̍쐬
		std::string rank = std::to_string(i + 1);
		DrawStringToHandle(750, 450 + i * score_ranking_space, rank.c_str(), 0xffffff, messageManager->GetMessageData("RankingFont").fontHandle);

		// �v���C���[���̕`��
		DrawStringToHandle(800, 450 + i * score_ranking_space, m_scoreRanking[i].playerName, 0xffffff, fontHandle);

		// �X�R�A�̕�����̍쐬
		std::string str = std::to_string(m_scoreRanking[i].score);
		while (str.size() < 4)
		{
			str = "0" + str;
		}

		// �X�R�A�̕`��
		DrawStringToHandle(1000, 450 + i * score_ranking_space, str.c_str(), 0xffffff, fontHandle);

		// �u�����h���[�h�̉���
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}