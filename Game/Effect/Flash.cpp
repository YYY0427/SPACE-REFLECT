#include "Flash.h"
#include "../Application.h"
#include <DxLib.h>
#include <algorithm>

namespace
{
	// 拡大速度
	constexpr float scale_speed = 100.0f;

	// 透明度の減速速度
	constexpr int alpha_speed = 10;

	// フラッシュの初期色
	constexpr unsigned int default_color = 0xffffff;

	//フラッシュの初期透明度(0～255)
	constexpr int default_alpha = 255;
}

// コンストラクタ
Flash::Flash(const int enableFrame) :
	m_enableFrame(enableFrame),
	m_color(default_color),
	m_alpha(default_alpha)
{
}

// デストラクタ
Flash::~Flash()
{
}

// 更新
void Flash::Update(const unsigned int color)
{
	// 色を設定
	m_color = color;

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
	auto& screenSize = Application::GetInstance().GetWindowSize();
	DrawBox(0, 0, screenSize.width, screenSize.height, m_color,true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// 終了したか
bool Flash::IsEnd() const
{
	return m_alpha <= 0;
}