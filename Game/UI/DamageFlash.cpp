#include "DamageFlash.h"
#include "../Application.h"
#include <DxLib.h>

// コンストラクタ
DamageFlash::DamageFlash() :
	m_frame(0),
	m_color(0xffffff),
	m_alpha(0),
	m_speed(0.0f)
{

}

// デストラクタ
DamageFlash::~DamageFlash()
{
}

// 更新
void DamageFlash::Update()
{
	// フレームが0以下なら処理を行わない
	if (m_frame <= 0) return;

	// アルファ値を減らす
	m_alpha -= m_speed;

	// アルファ値が0以下になったら0にする
	if (m_alpha <= 0)
	{
		m_alpha = 0;
		m_frame = 0;
	}
}

// 描画
void DamageFlash::Draw()
{
	// 画面のサイズを取得
	auto& screenSize = Application::GetInstance().GetWindowSize();

	// 色の描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
	DrawBox(0, 0, screenSize.width, screenSize.height, m_color, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// フラッシュの開始
void DamageFlash::Start(const int frame, const int alpha, const unsigned int color)
{
	// 値の設定
	m_frame = frame;
	m_alpha = alpha;
	m_color = color;

	// 速度の計算
	m_speed = static_cast<float>(alpha) / static_cast<float>(frame);
}
