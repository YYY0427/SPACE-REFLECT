#include "TestScene.h"
#include "../Application.h"
#include "../Transitor/TileTransitor.h"
#include "../Transitor/FadeTransitor.h"
#include "../Transitor/IrisTransitor.h"
#include "../Transitor/WipeTransitor.h"
#include "../Transitor/PushTransitor.h"
#include "../Transitor/StripTransitor.h"
#include "DxLib.h"

// コンストラクタ
TestScene::TestScene(SceneManager& manager) :
	SceneBase(manager)
{
	// 画面切り替え演出の設定
	m_pTransitor = std::make_unique<StripTransitor>();
	m_pTransitor->Start();
}

// デストラクタ
TestScene::~TestScene()
{
}

// 更新
void TestScene::Update()
{
	// 画面切り替え演出の更新
	m_pTransitor->Update();
}

// 描画
void TestScene::Draw()
{
	// 画面をクリア
	ClearDrawScreen();

	// 背景の描画
	const auto& size = Application::GetInstance().GetWindowSize();
	DrawBox(0, 0, size.width, size.height, 0xffffff, true);

	// 画面切り替え演出の描画
	m_pTransitor->Draw();
}
