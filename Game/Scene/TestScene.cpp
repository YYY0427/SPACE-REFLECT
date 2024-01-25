#include "TestScene.h"
#include "../Application.h"
#include "../Transitor/TileTransitor.h"
#include "../Transitor/FadeTransitor.h"
#include "../Transitor/IrisTransitor.h"
#include "../Transitor/WipeTransitor.h"
#include "../Transitor/PushTransitor.h"
#include "../Transitor/StripTransitor.h"
#include "../UI/Warning.h"
#include "DxLib.h"

namespace
{
	constexpr int kStretchWindowWidth = 640;
	constexpr int kStretchWindowHeight = 180;
}

// コンストラクタ
TestScene::TestScene(SceneManager& manager) :
	SceneBase(manager)
{
	// 画面切り替え演出の設定
	m_pTransitor = std::make_unique<StripTransitor>();
	m_pTransitor->Start();

	// 警告UIの設定
	m_pWarning = std::make_unique<Warning>(200);

	window = LoadGraph("Data/Image/window.png");
	player = LoadGraph("Data/Image/player.png");

	width = kStretchWindowWidth;
	height = kStretchWindowHeight;
	frameCount = 0;
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

	// 警告UIの更新
	m_pWarning->Update();

	frameCount++;
	if (frameCount > 120 * 3)
	{
		if (frameCount > 120 * 3 + 60)
		{
			width -= 8;
			height++;
		}
		else
		{
			width += 8;
			height--;
		}
	}
	if (frameCount > 120 * 4)
	{
		frameCount = 0;
		width = kStretchWindowWidth;
		height = kStretchWindowHeight;
	}
}

// 描画
void TestScene::Draw()
{
	// 画面をクリア
	ClearDrawScreen();

	// 背景の描画
	const auto& size = Application::GetInstance().GetWindowSize();
	DrawBox(0, 0, size.width, size.height, 0x000000, true);

	// 警告UIの描画
	m_pWarning->Draw();

	DrawString(16, 16, "UI用の素材を使用して可変サイズのウインドウを表示するサンプル", 0xffffff);

	DrawGraph(64, 64, window, true);
	DrawString(64, 64 + 96, "↑96x96の素材を使用してウインドウを表示する", 0xffffff);

	DrawExtendGraph(64, 220, 64 + width, 220 + height, window, true);
	DrawString(64, 220 + kStretchWindowHeight, "↑ダメな例　単純に拡大する　角の部分の丸みやアスペクト比がウインドウサイズによって変化してしまう", 0xffffff);

	DrawGraph(64, 450, window, true);
	DrawLine(64, 450 + 32, 64 + 96, 450 + 32, 0xf0f0f0);
	DrawLine(64, 450 + 64, 64 + 96, 450 + 64, 0xf0f0f0);
	DrawLine(64 + 32, 450, 64 + 32, 450 + 96, 0xf0f0f0);
	DrawLine(64 + 64, 450, 64 + 64, 450 + 96, 0xf0f0f0);
	DrawString(64, 450 + 96, "↑96x96の素材9個に分割して角、各辺、ウインドウ内部として使用する", 0xffffff);

	// 256*96のウインドウ表示
	DrawRectGraph(64, 600, 0, 0, 32, 32, window, true);	// 左上
	DrawRectGraph(64, 600 + height - 32, 0, 64, 32, 32, window, true);	// 左下
	DrawRectGraph(64 + width - 32, 600, 64, 0, 32, 32, window, true);	// 右上
	DrawRectGraph(64 + width - 32, 600 + height - 32, 64, 64, 32, 32, window, true);	// 右下
	DrawString(64, 600 + kStretchWindowHeight, "1.角をそれぞれ等倍で表示", 0xffffff);
	if (frameCount > 120 * 1)
	{
		DrawRectExtendGraph(64 + 32, 600, 64 + width - 32, 600 + 32, 32, 0, 32, 32, window, true);	// 上辺
		DrawRectExtendGraph(64 + 32, 600 + height - 32, 64 + width - 32, 600 + height - 32 + 32, 32, 64, 32, 32, window, true);	// 下辺
		DrawRectExtendGraph(64, 600 + 32, 64 + 32, 600 + height - 32, 0, 32, 32, 32, window, true);// 左辺
		DrawRectExtendGraph(64 + width - 32, 600 + 32, 64 + width, 600 + height - 32, 64, 32, 32, 32, window, true);// 右辺
		DrawString(64, 600 + kStretchWindowHeight + 16, "2.各辺を並べて表示　伸ばして問題なさそうなら伸ばして表示", 0xffffff);
	}

	if (frameCount > 120 * 2)
	{
		DrawRectExtendGraph(64 + 32, 600 + 32, 64 + width - 32, 600 + height - 32, 32, 32, 32, 32, window, true);	// ウインドウ内部
		DrawString(64, 600 + kStretchWindowHeight + 32, "3.ウインドウの中身をを並べて表示　伸ばして問題なさそうなら伸ばして表示", 0xffffff);
	}

	DrawRectRotaGraph(1280, 120, (frameCount / 8) % 2 * 32, 0, 32, 32, 1.0, 0, player, true);
	DrawString(1200, 120 + 32, "元画像", 0xffffff);

	DrawRectRotaGraph(1280, 240, (frameCount / 8) % 2 * 32, 0, 32, 32, 2.0, 0, player, true);
	DrawRectRotaGraph(1400, 240, (frameCount / 8) % 2 * 32, 0, 32, 32, 4.0, 0, player, true);
	DrawString(1200, 240 + 64, "縦横同じ比率で拡大→〇", 0xffffff);

	DrawRectRotaGraph3F(1280, 400, (frameCount / 8) % 2 * 32, 0, 32, 32, 16, 16, 4.0, 1.0, 0.0, player, true);
	DrawRectRotaGraph3F(1400, 400, (frameCount / 8) % 2 * 32, 0, 32, 32, 16, 16, 1.0, 5.0, 0.0, player, true);
	DrawRectRotaGraph3F(1520, 400, (frameCount / 8) % 2 * 32, 0, 32, 32, 16, 16, 2.5, 2.0, 0.0, player, true);
	DrawString(1200, 400 + 64, "アスペクト比(縦横比)がおかしい→×", 0xffffff);
	DrawString(1200, 400 + 64 + 16, "著作権的に改変ととられる恐れあり", 0xffffff);
	DrawString(1200, 400 + 64 + 32, "それ以前に見た目がおかしい、と気付ける感覚を", 0xffffff);
	DrawString(1200, 400 + 64 + 48, "ダメージを受けた時に一瞬変形、等はあり", 0xffffff);
	DrawString(1200, 400 + 64 + 64, "ただし著作権は…", 0xffffff);

	// 画面切り替え演出の描画
	m_pTransitor->Draw();
}
