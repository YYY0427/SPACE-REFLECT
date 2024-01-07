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
	// UI管理クラスの生成
	m_pUIManager = std::make_shared<UIManager>();

	// オブジェクト配置データ読み込み
	auto dataReader = std::make_unique<DataReaderFromUnity>();
	dataReader->LoadUnityGameObjectData(fileName);

	// カメラの生成
	auto& cameraData = dataReader->GetData("Camera");
	if (!cameraData.empty())
	{
		m_pCamera = std::make_shared<Camera>(cameraData.front());
	}

	// オブジェクトの生成
	auto& playerData = dataReader->GetData("Player");
	if (!playerData.empty())
	{
		m_pPlayer = std::make_shared<Player>(playerData.front());
	}

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
	// カメラの更新
	m_pCamera->Update();

	// プレイヤーの更新
	m_pPlayer->Update(m_pCamera->GetCameraHorizon());

	// スカイドームの更新
	m_pSkyDome->Update(m_pPlayer->GetPos());

	// UIの更新
	m_pUIManager->Update();
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
	m_pUIManager->Draw();
}
