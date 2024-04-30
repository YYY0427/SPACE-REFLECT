#include "TestScene.h"
#include "../Application.h"
#include "../Transitor/TileTransitor.h"
#include "../Transitor/FadeTransitor.h"
#include "../Transitor/IrisTransitor.h"
#include "../Transitor/WipeTransitor.h"
#include "../Transitor/PushTransitor.h"
#include "../Transitor/StripTransitor.h"
#include "../UI/Warning.h"
#include "../Util/FileUtil.h"
#include "../UI/ResultWindow.h"
#include "../Math/MathUtil.h"
#include "../MyDebug/DebugText.h"
#include "DxLib.h"

namespace
{
	const std::string file_extension = ".bmp";
}

// コンストラクタ
TestScene::TestScene(SceneManager& manager) :
	SceneBase(manager)
{
}

// デストラクタ
TestScene::~TestScene()
{
}

// 初期化
void TestScene::Init()
{
	// 画面切り替え演出の設定
	m_pTransitor = std::make_unique<StripTransitor>();
	m_pTransitor->Start();
}

// 終了処理
void TestScene::End()
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
	DrawBox(0, 0, size.width, size.height, 0x000000, true);
	
	static int count = 0;
	count++;
	if (count % 10 == 0)
	{
		DebugText::AddLog("TEST", { MathUtil::GetRandFloat(0.1f, 1.0) });
	}

	//// 読み込んだファイルの描画
	//for (int i = 0; i < m_fileNames.size(); i++)
	//{
	//	for (int j = 0; j < m_fileNames[i].size(); j++)
	//	{
	//		DrawString(20 * j, 50 + (i * 20), m_fileNames[i][j].c_str(), 0xffffff);
	//	}
	//}

	//int i = 0;
	//for (int i = 0; i < m_fileNames.size(); i++)
	//{
	//	// 拡張子を削除
	//	m_fileNames[i] = m_fileNames[i].substr(0, m_fileNames[i].size() - file_extension.size());

	//	// ファイル名の描画
	//	DrawString(0, 50 + (i * 20), m_fileNames[i].c_str(), 0xffffff);
	//}

	// 画面切り替え演出の描画
	m_pTransitor->Draw();
}
