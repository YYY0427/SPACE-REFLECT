#include "StageSelectScene.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "../Util/InputState.h"
#include "../Game/Camera.h"
#include "../Game/PlanetManager.h"
#include "../Game/SkyDome.h"
#include "../Editor/DataReaderFromUnity.h"
#include <DxLib.h>

namespace
{
	// オブジェクトのファイルパス
	const std::string object_file_path = "StageSelect";

	// モデルファイルパス
	const std::string earth_model_file_path = "Data/Model/Earth.mv1";
}

// コンストラクタ
StageSelectScene::StageSelectScene(SceneManager& manager) :
	SceneBase(manager),
	m_currentSelectItem(0)
{
	// オブジェクトのデータを読み込む
	DataReaderFromUnity::GetInstance().LoadUnityGameObjectData(object_file_path);

	// インスタンスの作成
	m_pCamera = std::make_unique<Camera>( Vector3{0, 0, 0}, Vector3{0, 0, 1});
	m_pPlanetManager = std::make_unique<PlanetManager>(object_file_path);
	m_pSkyDome = std::make_unique<SkyDome>(m_pCamera->GetPos());

	// ステージ名の設定
	m_stageData[Stage::TUTORIAL].stageName =  "Tutorial";
	m_stageData[Stage::STAGE_1].stageName = "Stage1";

	// 惑星の設定
	m_stageData[Stage::TUTORIAL].pPlanet = m_pPlanetManager->GetPlanet(PlanetType::MOON);
	m_stageData[Stage::STAGE_1].pPlanet = m_pPlanetManager->GetPlanet(PlanetType::EARTH);
}

// デストラクタ
StageSelectScene::~StageSelectScene()
{
}

// 更新
void StageSelectScene::Update()
{
	// 選択肢を回す処理
	int sceneItemTotalValue = static_cast<int>(Stage::NUM);
	if (InputState::IsTriggered(InputType::UP))
	{
		m_currentSelectItem = ((m_currentSelectItem - 1) + sceneItemTotalValue) % sceneItemTotalValue;
	}
	else if (InputState::IsTriggered(InputType::DOWN))
	{
		m_currentSelectItem = (m_currentSelectItem + 1) % sceneItemTotalValue;
	}

	// 更新
	m_pPlanetManager->Update();
	m_pSkyDome->Update(m_pCamera->GetPos());
	UpdateCamera();
	

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
}

// カメラの更新
void StageSelectScene::UpdateCamera()
{
	auto stageData = m_stageData[static_cast<Stage>(m_currentSelectItem)].pPlanet->GetPos();
	m_pCamera->Update({ stageData.x, stageData.y, stageData.z - 5000.0f }, stageData);
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

	// スコアランキングの描画
	for (int i = 0; i < m_scoreRanking.size(); i++)
	{
		// 右下に描画
		DrawString(700, 350 + i * 32, m_scoreRanking[i].playerName, 0xffffff);
		DrawFormatString(900, 350 + i * 32, 0xffffff, "%d", m_scoreRanking[i].score);
	}
}