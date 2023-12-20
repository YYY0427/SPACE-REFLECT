#include "IrisTransitor.h"
#include "../Application.h"
#include "DxLib.h"
#include <cmath>

// コンストラクタ
IrisTransitor::IrisTransitor(bool irisOut, int interval, bool isTiled, int gHandle) :
	TransitorBase(interval),
	m_irisOut(irisOut),
	m_isTiled(isTiled),
	m_handle(gHandle)
{
	// ウィンドウサイズの取得
	const auto& size = Application::GetInstance().GetWindowSize();

	// マスクスクリーンの作成
	m_handleForMaskScreen = MakeScreen(size.width, size.height, true);
	m_maskH = CreateMaskScreen();

	// 対角線の長さを計算
	m_diagonalLength = std::hypotf(size.width, size.height) / 2.0f;
}

// デストラクタ
IrisTransitor::~IrisTransitor()
{
	// マスクスクリーンの削除
	DeleteMaskScreen();
}

// 更新
void IrisTransitor::Update()
{
	// 演出時間内なら
	if (m_frame < m_interval) 
	{
		// フレームを進める
		m_frame++;

		// 描画先を切り替え先に変更
		SetDrawScreen(m_nextScene);
	}
	else if (m_frame == m_interval) 
	{
		// 描画先を裏画面に変更
		SetDrawScreen(DX_SCREEN_BACK);
	}
}

// 描画
void IrisTransitor::Draw()
{
	// 演出が終了していたら描画しない
	if (IsEnd()) return;

	// ウィンドウサイズの取得
	const auto& size = Application::GetInstance().GetWindowSize();

	// 割合を計算
	auto rate = static_cast<float>(m_frame) / static_cast<float>(m_interval);

	// ハンドルの保存
	int backScene = m_oldScene;
	int maskedScene = m_nextScene;

	// アイリスアウトの場合は逆にする
	if (m_irisOut) 
	{
		backScene = m_nextScene;
		maskedScene = m_oldScene;
		rate = 1.0f - rate;
	}

	// 円の半径を計算
	float radius = (m_diagonalLength) * rate;

	// マスクスクリーンを描画先にする
	SetDrawScreen(m_handleForMaskScreen);

	// 画面のクリア
	ClearDrawScreen();

	// 画像ハンドルがない場合は円を描画
	if (m_handle == -1)
	{
		// タイル状に描画する場合
		if (m_isTiled) 
		{
			// タイルの数を計算
			int xDiv = (size.width / 50) + 1;
			int yDiv = (size.width / 50) + 1;

			// 円の半径を計算
			radius = (50 * m_diagonalLength / 320) * rate;

			// タイル状に円を描画
			for (int y = 0; y < xDiv; y++) 
			{
				for (int x = 0; x < xDiv; x++) 
				{
					DrawCircleAA(50 + x * 100, 50 + y * 100, radius, 32, 0xffffff, true);
				}
			}
		}
		else 
		{
			// 円を描画
			DrawCircleAA(size.width / 2.0f, size.height / 2.0f, radius, 32, 0xffffff, true);
		}
	}
	else 
	{
		// 画像ハンドルがある場合は画像を描画
		DrawRotaGraphFast(size.width / 2.0f, size.height / 2.0f, rate * 3.5f, rate * 2.0f, m_handle, true);
	}

	// 隠し関数(現在のグラフィックハンドルをマスクスクリーンに転送)
	SetMaskScreenGraph(m_handleForMaskScreen);

	// 描画領域を反転する
	SetMaskReverseEffectFlag(true);

	// 描画先を裏画面に切り替え 
	SetDrawScreen(DX_SCREEN_BACK);

	// マスクを適用
	SetUseMaskScreenFlag(false);

	// 切り替え前画面を描画
	DrawGraph(0, 0, backScene, true);

	// マスクを適用
	SetUseMaskScreenFlag(true);

	// 切り替え後画面を描画
	DrawGraph(0, 0, maskedScene, true);

	// マスクを適用しない
	SetUseMaskScreenFlag(false);
}