#include "StageSelectScene.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "../String/MessageManager.h"
#include "../MyDebug/DebugText.h"
#include "../Util/InputState.h"
#include "../Util/Easing.h"
#include "../Game/Camera.h"
#include "../Game/PlanetManager.h"
#include "../Game/SkyDome.h"
#include "../Editor/DataReaderFromUnity.h"
#include "../Math/Vector3.h"
#include <DxLib.h>

namespace
{
	// オブジェクトのファイルパス
	const std::string object_file_path = "StageSelect";

	// モデルファイルパス
	const std::string earth_model_file_path = "Data/Model/Earth.mv1";

	// カメラの移動にかかるフレーム
	constexpr float camera_move_frame = 70.0f;

	// スコアランキング
	constexpr int score_ranking_space = 40;				// 文字間隔
	constexpr int score_ranking_alpha_add_speed = 10;	// アルファ値

}

// コンストラクタ
StageSelectScene::StageSelectScene(SceneManager& manager) :
	SceneBase(manager),
	m_currentSelectItem(0),
	m_easeTime(0.0f),
	m_isInput(false)
{
	// オブジェクトのデータを読み込む
	DataReaderFromUnity::GetInstance().LoadUnityGameObjectData(object_file_path);

	// インスタンスの作成
	m_pCamera = std::make_unique<Camera>( Vector3{0, 0, 0}, Vector3{0, 0, 1});
	m_pPlanetManager = std::make_unique<PlanetManager>(object_file_path);
	m_pSkyDome = std::make_unique<SkyDome>(m_pCamera->GetPos());

	// ステージの設定
	m_stageData[Stage::TUTORIAL].stageName =  "Tutorial";
	m_stageData[Stage::TUTORIAL].cameraPos = DataReaderFromUnity::GetInstance().GetData(object_file_path, "MoonCamera")[0].pos;
	m_stageData[Stage::STAGE_1].stageName = "Stage1";
	m_stageData[Stage::STAGE_1].cameraPos = DataReaderFromUnity::GetInstance().GetData(object_file_path, "EarthCamera")[0].pos;

	// 初期化
	m_cameraStartPos = m_pCamera->GetPos();
	m_cameraGoalPos = m_stageData[static_cast<Stage>(m_currentSelectItem)].cameraPos;
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
		if (InputState::IsTriggered(InputType::UP))
		{
			m_currentSelectItem = ((m_currentSelectItem - 1) + sceneItemTotalValue) % sceneItemTotalValue;
			m_isInput = true;
			for (auto& alpha : m_rankingAlpha)
			{
				alpha = 0;
			}
		}
		else if (InputState::IsTriggered(InputType::DOWN))
		{
			m_currentSelectItem = (m_currentSelectItem + 1) % sceneItemTotalValue;
			m_isInput = true;
			for (auto& alpha : m_rankingAlpha)
			{
				alpha = 0;
			}
		}
	}

	// 更新
	UpdateCamera();
	m_pPlanetManager->Update();
	m_pSkyDome->SetPos(m_pCamera->GetPos());
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
		m_manager.ChangeScene(std::make_shared<DebugScene>(m_manager));
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
	m_pPlanetManager->Draw();
	m_pSkyDome->Draw();

	// 飛べるシーンの項目のテキスト表示
	constexpr int draw_text_pos_x = 500;
	constexpr int draw_text_pos_y = 200;
	constexpr int text_space_y = 32;
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(Stage::TUTORIAL), "TUTORIAL", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(Stage::STAGE_1), "STAGE_1", 0xffffff, true);

	// 現在選択中の項目の横に→を表示
	DrawString(draw_text_pos_x - text_space_y, draw_text_pos_y + text_space_y * m_currentSelectItem, "→", 0xff0000);

	// スコアランキングの描画
	DrawScoreRanking();
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
	MessageManager::GetInstance().DrawStringCenter("RankingTitle", 900, 310, 0xffffff);

	// スコアランキングの描画
	// 逆から描画
	for(int i = m_scoreRanking.size() - 1; i >= 0; i--)
	{
		// フォントハンドルの取得
		auto fontHandle = MessageManager::GetInstance().GetMessageData("RankingFont").fontHandle;

		// 徐々に描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_rankingAlpha[i]);

	//	m_rankingAlpha[i] += (score_ranking_alpha_add_speed) * i + 1;

		// 順位の文字列の作成
		std::string rank = std::to_string(i + 1);
		DrawStringToHandle(750, 350 + i * score_ranking_space, rank.c_str(), 0xffffff, MessageManager::GetInstance().GetMessageData("RankingFont").fontHandle);

		// プレイヤー名の描画
		DrawStringToHandle(800, 350 + i * score_ranking_space, m_scoreRanking[i].playerName, 0xffffff, fontHandle);

		// スコアの文字列の作成
		std::string str = std::to_string(m_scoreRanking[i].score);
		while (str.size() < 4)
		{
			str = "0" + str;
		}
		// スコアの描画
		DrawStringToHandle(1000, 350 + i * score_ranking_space, str.c_str(), 0xffffff, fontHandle);

		// ブレンドモードの解除
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}