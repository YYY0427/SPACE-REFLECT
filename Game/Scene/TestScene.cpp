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
#include "DxLib.h"

namespace
{
	const std::string file_extension = ".bmp";
}

// コンストラクタ
TestScene::TestScene(SceneManager& manager) :
	SceneBase(manager)
{
	// 画面切り替え演出の設定
	m_pTransitor = std::make_unique<StripTransitor>();
	m_pTransitor->Start();

	m_pResultWindow = std::make_unique<ResultWindow>();

	// 警告UIの設定
//	m_pWarning = std::make_unique<Warning>(200);

	// ファイル名の一括読み込み
//	m_fileNames = FileUtil::LoadFileNames("Data/Test", file_extension);
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

	m_pResultWindow->Update();

	// 警告UIの更新
//	m_pWarning->Update();
}

// 描画
void TestScene::Draw()
{
	// 画面をクリア
	ClearDrawScreen();

	// 背景の描画
	const auto& size = Application::GetInstance().GetWindowSize();
	DrawBox(0, 0, size.width, size.height, 0x000000, true);

	m_pResultWindow->Draw();

	// 警告UIの描画
//	m_pWarning->Draw();

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
