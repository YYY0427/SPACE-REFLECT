#include "Flash.h"
#include <DxLib.h>
#include <algorithm>

namespace
{
	// 拡大速度
	constexpr float scale_speed = 100.0f;

	// 透明度の減速速度
	constexpr int alpha_speed = 10;
}

// コンストラクタ
Flash::Flash(int enableFrame) :
	m_enableFrame(enableFrame),
	m_pos({ 0, 0 }),
	m_color(0xffffff),
	m_radius(0.0f),
	m_alpha(255)
{
}

// デストラクタ
Flash::~Flash()
{
}

// 更新
void Flash::Update(unsigned int color)
{
	// 色を設定
	m_color = color;

	// 丸を拡大
	m_radius += scale_speed;

	// 一定フレーム経過したら透明度を下げる
	if (m_enableFrame-- <= 0)
	{
		// 透明度を下げる
		m_alpha -= alpha_speed;
		m_alpha = (std::max)(m_alpha, 0);
	}
}

// 描画
void Flash::Draw()
{
	// 透明度を適用して丸を描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
	DrawCircle(m_pos.x, m_pos.y, m_radius, m_color, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// 終了したか
bool Flash::IsEnd() const
{
	return m_alpha <= 0;
}
