#include "StageSelectScene.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "OptionScene.h"
#include "DebugScene.h"
#include "TitleScene.h"
#include "../Transitor/FadeTransitor.h"
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
#include <DxLib.h>

namespace
{
	// オブジェクトのファイルパス
	const std::string object_file_path = "StageSelect";

	// モデルファイルパス
	const std::string earth_model_file_path = "Data/Model/Earth.mv1";

	// 画像ファイルパス
	const std::string xbox_rb_file_path = "Data/Image/xbox_rb.png";
	const std::string xbox_lb_file_path = "Data/Image/xbox_lb.png";
	const std::string xbox_b_file_path = "Data/Image/xbox_button_b.png";
	const std::string xbox_a_file_path = "Data/Image/xbox_button_a.png";

	// カメラの移動にかかるフレーム
	constexpr float camera_move_frame = 70.0f;

	// スコアランキング
	constexpr int score_ranking_space = 20;				// 文字間隔
	constexpr int score_ranking_alpha_add_speed = 10;	// アルファ値

}

// コンストラクタ
StageSelectScene::StageSelectScene(SceneManager& manager) :
	SceneBase(manager),
	m_currentSelectItem(0),
	m_easeTime(0.0f),
	m_isInput(false),
	m_line3DAlpha(0),
	m_windowAlpha(0),
	m_rankingTitleAlpa(0),
	m_rbButtonImgHandle(-1),
	m_lbButtonImgHandle(-1),
	m_bButtonImgHandle(-1)
{
	// 画面切り替え演出の初期化
	m_pTransitor = std::make_unique<FadeTransitor>(10);
	m_pTransitor->Start();

	// オブジェクトのデータを読み込む
	DataReaderFromUnity::GetInstance().LoadUnityGameObjectData(object_file_path);

	// インスタンス作成
	m_pPlanetManager = std::make_unique<PlanetManager>(object_file_path);

	// ステージの設定
	m_stageData[Stage::TUTORIAL].stageName = "Tutorial";
	m_stageData[Stage::TUTORIAL].cameraPos = DataReaderFromUnity::GetInstance().GetData(object_file_path, "MoonCamera")[0].pos;
	m_stageData[Stage::TUTORIAL].pPlanet = m_pPlanetManager->GetPlanet(PlanetType::MOON);
	m_stageData[Stage::STAGE_1].stageName = "Stage1";
	m_stageData[Stage::STAGE_1].cameraPos = DataReaderFromUnity::GetInstance().GetData(object_file_path, "EarthCamera")[0].pos;
	m_stageData[Stage::STAGE_1].pPlanet = m_pPlanetManager->GetPlanet(PlanetType::EARTH);

	// インスタンス作成
	Vector3 pos = m_stageData[static_cast<Stage>(m_currentSelectItem)].cameraPos;
	m_pCamera = std::make_unique<Camera>(pos, Vector3{ pos.x, pos.y, pos.z + 10.0f });
	m_pSkyDome = std::make_unique<SkyDome>(m_pCamera->GetPos());

	// 画像の読み込み
	m_rbButtonImgHandle = my::MyLoadGraph(xbox_rb_file_path.c_str());
	m_lbButtonImgHandle = my::MyLoadGraph(xbox_lb_file_path.c_str());
	m_bButtonImgHandle = my::MyLoadGraph(xbox_b_file_path.c_str());
	m_aButtonImgHandle = my::MyLoadGraph(xbox_a_file_path.c_str());

	// 初期化
	m_cameraStartPos = m_pCamera->GetPos();
	m_cameraGoalPos = m_stageData[static_cast<Stage>(m_currentSelectItem)].cameraPos;

	// スコアランキングの初期化
	if (ScoreRanking::GetInstance().GetScoreData(m_stageData[static_cast<Stage>(m_currentSelectItem)].stageName).empty())
	{
		ScoreRanking::GetInstance().CreateNewScoreData(m_stageData[static_cast<Stage>(m_currentSelectItem)].stageName);
	}
	// アルファ値の初期化
	for (int i = 0; i < ScoreRanking::GetInstance().GetScoreData(m_stageData[static_cast<Stage>(m_currentSelectItem)].stageName).size(); i++)
	{
		m_rankingAlpha.push_back(0);
	}
}

// デストラクタ
StageSelectScene::~StageSelectScene()
{
}

// 更新
void StageSelectScene::Update()
{
	// 初期化
	m_isInput = false;

	// イージングが終了してたら入力を受け付ける
	if (m_easeTime >= camera_move_frame)
	{
		// 選択肢を回す処理
		int sceneItemTotalValue = static_cast<int>(Stage::NUM);
		if (InputState::IsTriggered(InputType::RIGHT))
		{
			m_currentSelectItem = ((m_currentSelectItem - 1) + sceneItemTotalValue) % sceneItemTotalValue;
			m_isInput = true;
			m_line3DAlpha = 0;
			m_windowAlpha = 0;
			m_rankingTitleAlpa = 0;
			for (auto& alpha : m_rankingAlpha)
			{
				alpha = 0;
			}
		}
		else if (InputState::IsTriggered(InputType::LEFT))
		{
			m_currentSelectItem = (m_currentSelectItem + 1) % sceneItemTotalValue;
			m_isInput = true;
			m_line3DAlpha = 0;
			m_windowAlpha = 0;
			m_rankingTitleAlpa = 0;
			for (auto& alpha : m_rankingAlpha)
			{
				alpha = 0;
			}
		}
	}

	// 更新
	UpdateCamera();
	m_pPlanetManager->UpdateStageSelect();
	m_pSkyDome->Update(m_pCamera->GetPos());

	// 3Dの線のアルファ値の更新
	m_line3DAlpha = (std::min)(m_line3DAlpha += 3, 255);

	// 3Dの線のアルファ値が特定の値を超えたか
	if (m_line3DAlpha >= 255)
	{
		// ウィンドウのアルファ値の更新
		m_windowAlpha = (std::min)(m_windowAlpha += 3, 100);
		m_rankingTitleAlpa = (std::min)(m_rankingTitleAlpa += 3, 255);

		// ウィンドウのアルファ値が特定の値を超えたか
		if (m_windowAlpha >= 100)
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
	}
	
	// 画面切り替え演出の更新
	m_pTransitor->Update();

	// オプション画面に遷移
	if (InputState::IsTriggered(InputType::RIGTH_SHERDER))
	{
		m_pTransitor->SetFrame(0);
		m_manager.ChangeScene(std::make_shared<OptionScene>(m_manager, State::STAGE_SELECT));
		return;
	}

	// 決定ボタンが押されたらシーン遷移
	if (InputState::IsTriggered(InputType::DECISION))
	{
		// ゲームシーンに遷移
		m_manager.ChangeScene(
			std::make_shared<GameScene>(m_manager, static_cast<Stage>(m_currentSelectItem)));
		return;
	}

	// キャンセルボタンが押されたらシーン遷移
	if (InputState::IsTriggered(InputType::BACK))
	{
		// 戻る
#ifdef _DEBUG
		m_manager.ChangeScene(std::make_shared<DebugScene>(m_manager));
#else
		m_manager.ChangeScene(std::make_shared<TitleScene>(m_manager));
#endif
		return;
	}

	DebugText::Log("easeTime", { m_easeTime });
	DebugText::Log("cameraPos", { m_pCamera->GetPos().x, m_pCamera->GetPos().y ,m_pCamera->GetPos().z });
}

// カメラの更新
void StageSelectScene::UpdateCamera()
{
	// 入力があった
	// 現在のイージングが終了していたら
	if (m_isInput && m_easeTime >= camera_move_frame)
	{
		// カメラの目的地を設定
		m_cameraGoalPos = m_stageData[static_cast<Stage>(m_currentSelectItem)].cameraPos;

		// 初期化
		m_cameraStartPos = m_pCamera->GetPos();
		m_easeTime = 0.0f;
	}

	// イージングでカメラを移動
	m_easeTime++;
	m_easeTime = (std::min)(m_easeTime, camera_move_frame);
	float x = Easing::EaseOutCubic(m_easeTime, camera_move_frame, m_cameraGoalPos.x, m_cameraStartPos.x);
	float y = Easing::EaseOutCubic(m_easeTime, camera_move_frame, m_cameraGoalPos.y, m_cameraStartPos.y);
	float z = Easing::EaseOutCubic(m_easeTime, camera_move_frame, m_cameraGoalPos.z, m_cameraStartPos.z);
	m_pCamera->Update({ x, y, z }, { x, y, z + 10.0f });
}

// 描画
void StageSelectScene::Draw()
{
	// 画面をクリア
	ClearDrawScreen();

	// モデル描画
	m_pCamera->SetCamera();
	m_pSkyDome->Draw();
	SetUseLighting(FALSE);
	m_pPlanetManager->Draw();
	SetUseLighting(TRUE);

	// ステージセレクトタイトルの描画
	auto& screenSize = Application::GetInstance().GetWindowSize();
	DrawRoundRectAA((screenSize.width / 2.0f) - 325, 50, (screenSize.width / 2.0f) - 50, 110, 5, 5, 8, 0xffffff, true);
	MessageManager::GetInstance().DrawStringCenter("MissionTitle", (screenSize.width / 2.0f) - 187, 80, 0x000000);

	// RBボタンの描画
	DrawRotaGraph((screenSize.width / 2.0f) + 375, 95, 1.0f, 0.0f, m_rbButtonImgHandle, true);

	// オプションタイトルの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
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
	DrawCone3D(m_stageData[static_cast<Stage>(m_currentSelectItem)].pPlanet->GetPos().ToDxLibVector3(),
		ConvScreenPosToWorldPos_ZLinear({ screenSize.width / 2.0f + 49, 153, /*m_stageData[static_cast<Stage>(m_currentSelectItem)].cameraPos.z / GetCameraFar()*/0.02f }), 3.0f, 8, 0xffffff, 0xffffff, true);
	SetUseLighting(TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 説明ウィンドウの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_windowAlpha);
	DrawRoundRectAA(screenSize.width / 2.0f + 50, 150, screenSize.width / 2.0f + 450, 575, 5, 5, 4, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_line3DAlpha);
	DrawRoundRectAA(screenSize.width / 2.0f + 50, 150, screenSize.width / 2.0f + 450, 575, 5, 5, 4, 0xffffff, false, 5.0f);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// スコアランキングの描画
	DrawScoreRanking();

	// 画面切り替え演出の描画
	m_pTransitor->Draw();
}

// スコアランキングの描画
void StageSelectScene::DrawScoreRanking()
{
	// スコアランキングの取得
	m_scoreRanking = ScoreRanking::GetInstance().GetScoreData(m_stageData[static_cast<Stage>(m_currentSelectItem)].stageName);

	// スコアランキングが空だったら新規作成
	if (m_scoreRanking.empty())
	{
		ScoreRanking::GetInstance().CreateNewScoreData(m_stageData[static_cast<Stage>(m_currentSelectItem)].stageName);
	}

	// ランキングタイトルの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_rankingTitleAlpa);
	MessageManager::GetInstance().DrawStringCenter("RankingTitle", 900, 425, 0xffffff);
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