#pragma once
#include "TransitorBase.h"

/// <summary>
/// 画面切り替え演出
/// クロスフェード
/// </summary>
class FadeTransitor final : public TransitorBase
{
public:
	// コンストラクタ
	FadeTransitor(int interval = 60);

	// デストラクタ
	~FadeTransitor();

	// 更新
	void Update() override final;

	// 描画
	void Draw() override final;
};