#include "GameScene.h"
#include "../Editor/DataReaderFromUnity.h"
#include "../UI/UIManager.h"
#include "../Game/Player.h"
#include "../Game/Camera.h"
#include "../Game/SkyDome.h"
#include "DxLib.h"

// コンストラクタ
GameScene::GameScene(SceneManager& manager, std::string fileName) :
	SceneBase(manager)
{
	// オブジェクト配置データ読み込み
	auto dataReader = std::make_unique<DataReaderFromUnity>();
	dataReader->LoadUnityGameObjectData(fileName);

	// オブジェクトの生成
	auto& playerData = dataReader->GetData("Player");
	if (!playerData.empty())
	{
		m_pPlayer = std::make_shared<Player>(playerData.front());
	}

	// カメラの生成
	m_pCamera = std::make_shared<Camera>(m_pPlayer->GetPos());

	// スカイドームの生成
	m_pSkyDome = std::make_shared<SkyDome>(m_pPlayer->GetPos());
}

// デストラクタ
GameScene::~GameScene()
{
}

// 更新
void GameScene::Update()
{
	// プレイヤーの更新
	m_pPlayer->Update(m_pCamera->GetCameraHorizon());

	// カメラの更新
	m_pCamera->Update(m_pPlayer->GetPos());

	// スカイドームの更新
	m_pSkyDome->Update(m_pPlayer->GetPos());

	// UIの更新
	UIManager::GetInstance().Update();
}

// 描画
void GameScene::Draw()
{
	// 画面をクリア
	ClearDrawScreen();

	// スカイドームの描画
	m_pSkyDome->Draw();

	// プレイヤーの描画
	m_pPlayer->Draw();

	// UIの描画
	UIManager::GetInstance().Draw();
}
