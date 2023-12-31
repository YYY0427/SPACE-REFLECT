#include "GameScene.h"
#include "../Editor/DataReaderFromUnity.h"
#include "DxLib.h"

// コンストラクタ
GameScene::GameScene(SceneManager& manager, std::string fileName) :
	SceneBase(manager)
{
	// オブジェクト配置データ読み込み
	auto dataReader = std::make_unique<DataReaderFromUnity>();
	dataReader->LoadUnityGameObjectData(fileName);
}

// デストラクタ
GameScene::~GameScene()
{
}

// 更新
void GameScene::Update()
{
}

// 描画
void GameScene::Draw()
{
	// 画面をクリア
	ClearDrawScreen();

	// テスト描画
	DrawString(0, 0, "test", 0xffffff);
}
