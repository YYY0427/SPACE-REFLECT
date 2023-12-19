#include "FadeTransitor.h"
#include "DxLib.h"

FadeTransitor::FadeTransitor(int interval) :
	TransitorBase(interval)
{
}

FadeTransitor::~FadeTransitor()
{
}

void FadeTransitor::Update()
{
	if (m_frame < m_interval)
	{
		m_frame++;
		SetDrawScreen(m_nextScene);
	}
	else if(m_frame == m_interval)
	{
		SetDrawScreen(DX_SCREEN_BACK);
	}
}

void FadeTransitor::Draw()
{
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
