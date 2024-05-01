#pragma once
#include "UIBase.h"

/// <summary>
/// ダメージを受けたときのフラッシュ
/// </summary>
class DamageFlash : public UIBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DamageFlash();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DamageFlash();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override final;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override final;

	/// <summary>
	/// 演出開始
	/// </summary>
	/// <param name="frame">フラッシュのフレーム数</param>
	/// <param name="alpha">フラッシュの透明度</param>
	/// <param name="color">フラッシュの色</param>
	void Start(const int frame, const int alpha, const unsigned int color);

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

