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
#include "../Util/InputState.h"
#include "../Util/Easing.h"
#include "../Util/DrawFunctions.h"
#include "../Game/Camera.h"
#include "../Game/PlanetManager.h"
#include "../Game/SkyDome.h"
#include "../Editor/DataReaderFromUnity.h"
#include "../Math/Vector3.h"
#include "../Application.h"
#include "../ModelHandleManager.h"
#include "../SoundManager.h"
#include <DxLib.h>

namespace
{
	// オブジェクトのファイルパス
	const std::string object_file_name  = "StageSelect";

	// 画像ファイルパス
	const std::string xbox_rb_file_path = "Data/Image/xbox_rb.png";
	const std::string xbox_lb_file_path = "Data/Image/xbox_lb.png";
	const std::string xbox_b_file_path  = "Data/Image/xbox_button_b.png";
	const std::string xbox_a_file_path  = "Data/Image/xbox_button_a.png";

	// スコアランキング
	constexpr int score_ranking_alpha_add_speed = 15;	// アルファ値
	constexpr int score_ranking_space           = 20;	// 文字間隔

	// 説明ウィンドウ
	const Vector2 explanation_window_size = { 400, 425 };	// ウィンドウのサイズ
	const Vector2 window_scale_frame      = { 15, 20 };		// ウィンドウの拡大のフレーム
	constexpr int window_max_alpha        = 180;			// ウィンドウの最大アルファ値

	// ステージ選択時にカメラの移動にかかるフレーム
	constexpr int camera_move_frame = 40;

	// ステージ決定時
	constexpr int camera_move_frame_to_planet = 150;	// 惑星を向くまでのフレーム
	constexpr int camera_move_frame_to_stage  = 300;	// カメラの移動にかかるフレーム
}

// コンストラクタ
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

// デストラクタ
StageSelectScene::~StageSelectScene()
{
}

// 初期化
void StageSelectScene::Init()
{
	// ステートマシンの設定
	m_stateMachine.AddState(State::STAGE_SELECT, {}, [this]() {UpdateSelectStage(); }, {});
	m_stateMachine.AddState(State::START_ANIMATION, [this]() {EnterStartAnimation(); }, [this]() {UpdateStartAnimation(); }, {});
	m_stateMachine.SetState(State::STAGE_SELECT);

	// ライトの設定
	m_lightHandle = CreateDirLightHandle({ -1, -1, 1 });
	SetLightDifColorHandle(m_lightHandle, GetColorF(1.0f, 1.0f, 1.0f, 0.0f));

	// フェードインの演出
	m_pFade = std::make_unique<Fade>();
	m_pFade->StartFadeIn(0);

	// オブジェクトのデータを読み込む
	DataReaderFromUnity::GetInstance().LoadUnityGameObjectData(object_file_name);

	// インスタンス作成
	m_pPlanetManager = std::make_unique<PlanetManager>(object_file_name);

	// ステージの設定
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

	// インスタンス作成
	Vector3 pos = m_stageData[static_cast<Stage>(m_currentSelectItem)].cameraPos;
	m_pCamera = std::make_unique<Camera>();
	m_pCamera->SetCamera(pos, Vector3{ pos.x, pos.y, pos.z + 10.0f });
	m_pSkyDome = std::make_unique<SkyDome>(m_pCamera->GetPos());

	// スクリーンの作成
	m_screenHandle = MakeScreen(
		Application::GetInstance().GetWindowSize().width,
		Application::GetInstance().GetWindowSize().height, true);

	// 画像の読み込み
	m_rbButtonImgHandle = my::MyLoadGraph(xbox_rb_file_path.c_str());
	m_lbButtonImgHandle = my::MyLoadGraph(xbox_lb_file_path.c_str());
	m_bButtonImgHandle = my::MyLoadGraph(xbox_b_file_path.c_str());
	m_aButtonImgHandle = my::MyLoadGraph(xbox_a_file_path.c_str());

	// 初期化
	m_cameraStartPos = m_pCamera->GetPos();
	m_cameraGoalPos = m_stageData[static_cast<Stage>(m_currentSelectItem)].cameraPos;

	// スコアランキングの初期化
	if (ScoreRanking::GetInstance().GetScoreData(m_stageData[static_cast<Stage>(m_currentSelectItem)].stageNameId).empty())
	{
		ScoreRanking::GetInstance().CreateNewScoreData(m_stageData[static_cast<Stage>(m_currentSelectItem)].stageNameId);
	}
	// アルファ値の初期化
	for (int i = 0; i < ScoreRanking::GetInstance().GetScoreData(m_stageData[static_cast<Stage>(m_currentSelectItem)].stageNameId).size(); i++)
	{
		m_rankingAlpha.push_back(0);
	}

	// BGMの再生
	SoundManager::GetInstance().PlayBGM("StageSelectBgm");
	SoundManager::GetInstance().SetFadeSound("StageSelectBgm", 60, 0, 255);
}

// 終了処理
void StageSelectScene::End()
{
	// ライトのハンドルを削除
	DeleteLightHandle(m_lightHandle);

	// BGMの停止
	SoundManager::GetInstance().StopSound("StageSelectBgm");

	// 画像の削除
	DeleteGraph(m_rbButtonImgHandle);
	DeleteGraph(m_lbButtonImgHandle);
	DeleteGraph(m_bButtonImgHandle);
	DeleteGraph(m_aButtonImgHandle);
}

// ステージ選択時の初期化
void StageSelectScene::SelectStageProcess()
{
	// 初期化
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

// スタート演出の開始
void StageSelectScene::EnterStartAnimation()
{
	m_cameraStartTargetPos = m_pCamera->GetTarget();
	m_cameraStartPos = m_pCamera->GetPos();
}

// 更新
void StageSelectScene::Update()
{
	// デバッグテキスト
	DebugText::AddLog("cameraPos", { m_pCamera->GetPos().x, m_pCamera->GetPos().y ,m_pCamera->GetPos().z });

	// フェードの更新
	m_pFade->Update();

	// ステートマシンの更新
	m_stateMachine.Update();
}

// ステージ選択の更新
void StageSelectScene::UpdateSelectStage()
{
	// 初期化
	m_isInput = false;

	// イージングが終了してたら入力を受け付ける
	if (m_selectStageCameraEasingTime >= camera_move_frame)
	{
		// 選択肢を回す処理
		int sceneItemTotalValue = static_cast<int>(Stage::NUM);
		if (InputState::IsTriggered(InputType::RIGHT))
		{
			m_currentSelectItem = ((m_currentSelectItem - 1) + sceneItemTotalValue) % sceneItemTotalValue;
			SelectStageProcess();
		}
		else if (InputState::IsTriggered(InputType::LEFT))
		{
			m_currentSelectItem = (m_currentSelectItem + 1) % sceneItemTotalValue;
			SelectStageProcess();
		}
	}

	// 更新
	UpdateCamera();
	m_pPlanetManager->UpdateStageSelect();
	m_pSkyDome      ->Update(m_pCamera->GetPos());

	// 説明ウィンドウの幅を徐々に広げる
	m_explanationWindowEasingTime.x = (std::min)(++m_explanationWindowEasingTime.x, window_scale_frame.x);
	m_explanationWindowSize.x = Easing::EaseOutCubic(m_explanationWindowEasingTime.x, window_scale_frame.x, explanation_window_size.x, 0);
	if (m_explanationWindowSize.x < explanation_window_size.x)
	{
		SoundManager::GetInstance().PlaySE("StageSelectOpenMenu");
	}

	// 3Dの線のアルファ値の更新
	m_line3DAlpha = (std::min)(m_line3DAlpha += 6, 255);

	// 最大サイズに達したら高さを広げる
	if (m_explanationWindowSize.x >= explanation_window_size.x)
	{
		// 説明ウィンドウの高さを徐々に広げる
		m_explanationWindowEasingTime.y = (std::min)(++m_explanationWindowEasingTime.y, window_scale_frame.y);
		m_explanationWindowSize.y = Easing::EaseOutCubic(m_explanationWindowEasingTime.y, window_scale_frame.y, explanation_window_size.y, 0);

		// 最大サイズに達したらアルファ値を上げる
		if (m_explanationWindowSize.y >= explanation_window_size.y)
		{
			// ウィンドウのアルファ値の更新
			m_textAlpha = (std::min)(m_textAlpha += 6, 255);

			// スコアランキングのアルファ値の更新
			UpdateRankingAlpha();
		}
	}

	// オプション画面に遷移
	if (InputState::IsTriggered(InputType::RIGHT_SHOULDER))
	{
		// SEの再生
		SoundManager::GetInstance().PlaySE("Select");

		// シーンの遷移
		m_manager.PushScene(std::make_shared<OptionScene>(m_manager, OptionScene::State::STAGE_SELECT));
		return;
	}

	// 決定ボタンが押されたらスタート演出に遷移
	if (InputState::IsTriggered(InputType::DECISION))
	{
		// ステートマシンの設定
		m_stateMachine.SetState(State::START_ANIMATION);

		// SEの再生
		auto& soundManager = SoundManager::GetInstance();
		soundManager.PlaySE("Enter");

		// BGMのフェードアウトの設定
		soundManager.SetFadeSound("StageSelectBgm", 60, soundManager.GetSoundVolume("StageSelectBgm"), 0);
		return;
	}

	// キャンセルボタンが押されたら
	if (InputState::IsTriggered(InputType::BACK))
	{
		// フェードアウトの演出の開始
		m_pFade->StartFadeOut(255, 10);

		// BGMのフェードアウト
		auto& soundManager = SoundManager::GetInstance();
		soundManager.SetFadeSound("StageSelectBgm", 60, soundManager.GetSoundVolume("StageSelectBgm"), 0);
	}

	// フェードアウトが終了したらシーン遷移
	if (m_pFade->IsFadeOutEnd())
	{
		// タイトル画面に戻る
		m_manager.ChangeScene(std::make_shared<TitleScene>(m_manager));
		return;
	}
}

// スタート演出の更新			
void StageSelectScene::UpdateStartAnimation()
{
	// UI全体のアルファ値を下げる
	m_uiAlpha -= 10;

	// 惑星の更新
	m_pPlanetManager->UpdateStageSelect();

	// イージングでカメラの注視点を移動
	m_decisionStageTargetEasingTime++;
	m_decisionStageTargetEasingTime = (std::min)(m_decisionStageTargetEasingTime, camera_move_frame_to_planet);
	Vector3 goalPos = m_stageData[static_cast<Stage>(m_currentSelectItem)].pPlanet->GetPos();
	float x = Easing::EaseOutCubic(m_decisionStageTargetEasingTime, camera_move_frame_to_planet, goalPos.x, m_cameraStartTargetPos.x);
	float y = Easing::EaseOutCubic(m_decisionStageTargetEasingTime, camera_move_frame_to_planet, goalPos.y, m_cameraStartTargetPos.y);
	m_pCamera->SetCamera(m_pCamera->GetPos(), { x, y, goalPos.z });

	// カメラの注視点が惑星に到達したら
	if (m_decisionStageTargetEasingTime >= camera_move_frame_to_planet)
	{
		// インスタンスの取得
		auto& soundManager = SoundManager::GetInstance();

		// スタートアニメーションのSEを再生していない場合
		if (!m_isStartAnimSE)
		{
			// スタートアニメーションのSEを再生
			soundManager.PlaySE("StartAnimSe");

			// スタートアニメーションのSEのフェードインの設定
			soundManager.SetFadeSound("StartAnimSe", 30, soundManager.GetSoundVolume("StartAnimSe"), 255);

			// 再生したフラグを立てる
			m_isStartAnimSE = true;
		}
		
		// イージングでカメラを移動
		m_decisionStageCameraEasingTime++;
		m_decisionStageCameraEasingTime = (std::min)(m_decisionStageCameraEasingTime, camera_move_frame_to_stage);
		Vector3 goalPos = m_stageData[static_cast<Stage>(m_currentSelectItem)].pPlanet->GetPos();
		float posX = Easing::EaseOutCubic(m_decisionStageCameraEasingTime, camera_move_frame_to_stage, goalPos.x, m_cameraStartPos.x);
		float posY = Easing::EaseOutCubic(m_decisionStageCameraEasingTime, camera_move_frame_to_stage, goalPos.y, m_cameraStartPos.y);
		float posZ = Easing::EaseOutCubic(m_decisionStageCameraEasingTime, camera_move_frame_to_stage, goalPos.z, m_cameraStartPos.z);
		m_pCamera->SetCamera({ posX, posY, posZ }, { m_pCamera->GetTarget().x, m_pCamera->GetTarget().y, goalPos.z });

		if (m_decisionStageCameraEasingTime >= 60)
		{
			// フェードアウトの演出の開始
			m_pFade->StartFadeOut(255, 10);

			// スタートアニメーションのSEのフェードアウトの設定
			soundManager.SetFadeSound("StartAnimSe", 60, soundManager.GetSoundVolume("StartAnimSe"), 0);
		}
	}

	// フェードアウトが終了したら次のシーンに遷移
	if (m_pFade->IsFadeOutEnd())
	{
		// ゲームシーンに遷移
		m_manager.ChangeScene(
			std::make_shared<GameScene>(m_manager, static_cast<Stage>(m_currentSelectItem)));
		return;
	}
}

// カメラの更新
void StageSelectScene::UpdateCamera()
{
	// 入力があった
	// 現在のイージングが終了していたら
	if (m_isInput && m_selectStageCameraEasingTime >= camera_move_frame)
	{
		// カメラの目的地を設定
		m_cameraGoalPos = m_stageData[static_cast<Stage>(m_currentSelectItem)].cameraPos;

		// 初期化
		m_cameraStartPos = m_pCamera->GetPos();
		m_selectStageCameraEasingTime = 0;
	}

	// イージングでカメラを移動
	m_selectStageCameraEasingTime++;
	m_selectStageCameraEasingTime = (std::min)(m_selectStageCameraEasingTime, camera_move_frame);
	float x = Easing::EaseOutCubic(m_selectStageCameraEasingTime, camera_move_frame, m_cameraGoalPos.x, m_cameraStartPos.x);
	float y = Easing::EaseOutCubic(m_selectStageCameraEasingTime, camera_move_frame, m_cameraGoalPos.y, m_cameraStartPos.y);
	float z = Easing::EaseOutCubic(m_selectStageCameraEasingTime, camera_move_frame, m_cameraGoalPos.z, m_cameraStartPos.z);
	m_pCamera->SetCamera({ x, y, z }, { x, y, z + 10.0f });
}

// ランキングのアルファ値の更新
void StageSelectScene::UpdateRankingAlpha()
{
	// スコアランキングのアルファ値の更新
	// 一番下の順位が255になったら次の順位を更新
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

// 描画
void StageSelectScene::Draw()
{
	// 画面をクリア
	ClearDrawScreen();

	// 描画先を変更
	SetDrawScreen(m_screenHandle);

	// 画面をクリア
	ClearDrawScreen();

	// ステージセレクトタイトルの描画
	auto& screenSize = Application::GetInstance().GetWindowSize();
	DrawRoundRectAA((screenSize.width / 2.0f) - 325, 50, (screenSize.width / 2.0f) - 50, 110, 5, 5, 8, 0xffffff, true);
	MessageManager::GetInstance().DrawStringCenter("MissionTitle", (screenSize.width / 2.0f) - 187, 80, 0x000000);

	// RBボタンの描画
	DrawRotaGraph((screenSize.width / 2.0f) + 375, 95, 1.0f, 0.0f, m_rbButtonImgHandle, true);

	// オプションタイトルの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawRoundRectAA((screenSize.width / 2.0f) + 325, 50, (screenSize.width / 2.0f) + 50, 110, 5, 5, 8, 0xffffff, true);
	MessageManager::GetInstance().DrawStringCenter("OptionTitle", (screenSize.width / 2.0f) + 187, 80, 0x000000);
	// LBボタンの描画
	DrawRotaGraph((screenSize.width / 2.0f) - 375, 95, 1.0f, 0.0f, m_lbButtonImgHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
	// Bボタンの描画
	DrawRotaGraph(screenSize.width - 180, screenSize.height - 50, 1.0, 0.0f, m_bButtonImgHandle, true);
	MessageManager::GetInstance().DrawStringCenter("StageSelectBack", screenSize.width - 100, screenSize.height - 50, 0xffffff);

	// Aボタンの描画
	DrawRotaGraph(screenSize.width / 2 - 120, screenSize.height - 75, 1.0, 0.0f, m_aButtonImgHandle, true);
	MessageManager::GetInstance().DrawStringCenter("StageSelectStart", screenSize.width / 2, screenSize.height - 75, 0xffffff);

	// 三角形の描画
	DrawTriangleAA(screenSize.width - 50, screenSize.height / 2.0f, screenSize.width - 100, screenSize.height / 2.0f - 75, screenSize.width - 100, screenSize.height / 2.0f + 75, 0xffffff, true);
	DrawTriangleAA(0 + 50, screenSize.height / 2.0f, 0 + 100, screenSize.height / 2.0f - 75, 0 + 100, screenSize.height / 2.0f + 75, 0xffffff, true);

	// メニューとメイン画面を区切る線の描画
	DrawLineAA(0 + 100, 120, screenSize.width - 100, 120, 0xffffff, 3.0f);

	// 惑星から説明ウィンドウまでの線の描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_line3DAlpha);
	SetUseLighting(FALSE);
	SetUseZBuffer3D(FALSE);
	DrawCone3D(m_stageData[static_cast<Stage>(m_currentSelectItem)].pPlanet->GetPos().ToDxLibVector3(),
		ConvScreenPosToWorldPos_ZLinear({ screenSize.width / 2.0f + 49, 153, 0.02f }), 3.0f, 8, 0xffffff, 0xffffff, true);
	SetUseZBuffer3D(TRUE);
	SetUseLighting(TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 説明ウィンドウの描画
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

	// ステージタイトルの描画
	MessageManager::GetInstance().DrawStringCenter("StageSelectMission", 890, 200, 0xffffff);
	MessageManager::GetInstance().DrawStringCenter(m_stageData[static_cast<Stage>(m_currentSelectItem)].missionNameId, 890, 250, 0xffffff);

	// 線の描画
	DrawLine(screenSize.width / 2.0f + 100, 280,  screenSize.width / 2.0f + 400, 280, 0xffffff, 2.0f);

	// 難易度の描画
	MessageManager::GetInstance().DrawString(m_stageData[static_cast<Stage>(m_currentSelectItem)].difficultyId, 790, 310, 0xffffff);
	MessageManager::GetInstance().DrawString(m_stageData[static_cast<Stage>(m_currentSelectItem)].conditionsId, 790, 350, 0xffffff);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// スコアランキングの描画
	DrawScoreRanking();
	SetDrawScreen(DX_SCREEN_BACK);

	// モデル描画
	m_pCamera->SetCamera();
	m_pSkyDome->Draw();
	m_pPlanetManager->Draw();

	// UIの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_uiAlpha);
	DrawGraph(0, 0, m_screenHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// フェードの描画
	m_pFade->DrawFade(true);
}

// スコアランキングの描画
void StageSelectScene::DrawScoreRanking()
{
	// スコアランキングの取得
	m_scoreRanking = ScoreRanking::GetInstance().GetScoreData(m_stageData[static_cast<Stage>(m_currentSelectItem)].stageNameId);

	// スコアランキングが空だったら新規作成
	if (m_scoreRanking.empty())
	{
		ScoreRanking::GetInstance().CreateNewScoreData(m_stageData[static_cast<Stage>(m_currentSelectItem)].stageNameId);
	}

	// ランキングタイトルの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_textAlpha);
	MessageManager::GetInstance().DrawStringCenter("RankingTitle", 805, 425, 0xffffff);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// スコアランキングの描画
	// 逆から描画
	for(int i = m_scoreRanking.size() - 1; i >= 0; i--)
	{
		// フォントハンドルの取得
		auto fontHandle = MessageManager::GetInstance().GetMessageData("RankingFont").fontHandle;

		// 徐々に描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_rankingAlpha[i]);

		// 順位の文字列の作成
		std::string rank = std::to_string(i + 1);
		DrawStringToHandle(750, 450 + i * score_ranking_space, rank.c_str(), 0xffffff, MessageManager::GetInstance().GetMessageData("RankingFont").fontHandle);

		// プレイヤー名の描画
		DrawStringToHandle(800, 450 + i * score_ranking_space, m_scoreRanking[i].playerName, 0xffffff, fontHandle);

		// スコアの文字列の作成
		std::string str = std::to_string(m_scoreRanking[i].score);
		while (str.size() < 4)
		{
			str = "0" + str;
		}

		// スコアの描画
		DrawStringToHandle(1000, 450 + i * score_ranking_space, str.c_str(), 0xffffff, fontHandle);

		// ブレンドモードの解除
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}