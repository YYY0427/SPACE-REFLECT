#include "StripTransitor.h"
#include "../Application.h"
#include "DxLib.h"

// コンストラクタ
StripTransitor::StripTransitor(int width, int interval) :
	TransitorBase(interval),
	m_width(width)
{
}

// デストラクタ
StripTransitor::~StripTransitor()
{
}

// 更新
void StripTransitor::Update()
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
void StripTransitor::Draw()
{
	// 演出が終了していたら描画しない
	if (IsEnd()) return;

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// ウィンドウサイズの取得
	const auto& size = Application::GetInstance().GetWindowSize();

	// 割合を計算
	auto rate = static_cast<float>(m_frame) / static_cast<float>(m_interval);

	// 短冊の数を計算
	int lp = (size.width / m_width) + 1;

	// 切り替え前の画面を描画
	DrawRectGraph(0, 0, 0, 0, size.width, size.height, m_oldScene, true);

	// 切り替え先の画面を短冊状に描画
	for (int i = 0; i < lp; i++)
	{
		if (i % 2 == 0) 
		{
			DrawRectGraph(i * m_width, size.height * (rate - 1.0f), i * m_width, 0, m_width, size.height, m_nextScene, true);
		}
		else 
		{
			DrawRectGraph(i * m_width, size.height * (1.0f - rate), i * m_width, 0, m_width, size.height, m_nextScene, true);
		}
	}
}