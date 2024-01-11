#pragma once
#include "UIBase.h"

/// <summary>
/// ダメージを受けたときのフラッシュ
/// </summary>
class DamageFlash : public UIBase
{
public:
	// コンストラクタ
	DamageFlash();

	// デストラクタ
	~DamageFlash();

	// 更新
	void Update() override final;

	// 描画
	void Draw() override final;

	// 演出開始
	void Start(int frame, int alpha, unsigned int color);

private:
	// フレーム数
	int m_frame;

	// アルファ値
	int m_alpha;

	// 色
	unsigned int m_color;

	// 透明度の変化量
	float m_speed;
};

