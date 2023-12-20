#include "FadeTransitor.h"
#include "DxLib.h"

// コンストラクタ
FadeTransitor::FadeTransitor(int interval) :
	TransitorBase(interval)
{
}

// デストラクタ
FadeTransitor::~FadeTransitor()
{
}

// 更新
void FadeTransitor::Update()
{
	// 演出時間内なら
	if (m_frame < m_interval)
	{
		// フレームを進める
		m_frame++;

		// 描画先を切り替え先に変更
		SetDrawScreen(m_nextScene);
	}
	else if(m_frame == m_interval)
	{
		// 描画先を裏画面に変更
		SetDrawScreen(DX_SCREEN_BACK);
	}
}

void FadeTransitor::Draw()
{
	// 演出が終了していたら描画しない
	if (IsEnd()) return;

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// 割合を計算
	auto rate = static_cast<float>(m_frame) / static_cast<float>(m_interval);

	// 切り替え前の画面を描画
	DrawGraph(0, 0, m_oldScene, true);

	// 切り替え先の画面をアルファブレンドで描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(rate * 255));
	DrawGraph(0, 0, m_nextScene, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
