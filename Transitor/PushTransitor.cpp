#include "PushTransitor.h"
#include "../Application.h"
#include "DxLib.h"
#include <cassert>

// コンストラクタ
PushTransitor::PushTransitor(TransitDirection dir, int interval) :
	TransitorBase(interval),
	m_direction(dir)
{
}

// デストラクタ
PushTransitor::~PushTransitor()
{
}

// 更新
void PushTransitor::Update()
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

// 描画
void PushTransitor::Draw()
{
	// 演出が終了していたら描画しない
	if(IsEnd()) return;

	// ウィンドウサイズを取得
	const auto& size = Application::GetInstance().GetWindowSize();

	// 描画先を裏画面に変更
	SetDrawScreen(DX_SCREEN_BACK);

	// 割合を計算
	auto rate = static_cast<float>(m_frame) / static_cast<float>(m_interval);

	// 最終的に切り替え先画面が0に来るようにminusoneを用意
	auto minusone = rate - 1.0f;

	// 押し出す方向によって座標を計算
	int endX = 0, endY = 0;
	switch (m_direction)
	{
	case TransitDirection::UP:
		endY = -size.height;
		break;
	case TransitDirection::DOWN:
		endY = size.height;
		break;
	case TransitDirection::LEFT:
		endX = -size.width;
		break;
	case TransitDirection::RIGHT:
		endX = size.width;
		break;
	default:
		assert(false);
	}

	// 切り替え前の画面の描画
	DrawGraph(endX * rate, endY * rate, m_oldScene, true);

	// 切り替え後の画面の描画
	DrawGraph(endX * minusone, endY * minusone, m_nextScene, true);
}