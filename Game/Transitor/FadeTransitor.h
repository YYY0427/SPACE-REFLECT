#pragma once
#include "TransitorBase.h"

/// <summary>
/// 画面切り替え演出
/// クロスフェード
/// </summary>
class FadeTransitor final : public TransitorBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="interval">切り替え時間</param>
	FadeTransitor(int interval = 60);

	// デストラクタ
	~FadeTransitor();

	// 更新
	void Update() override final;

	// 描画
	void Draw() override final;
};