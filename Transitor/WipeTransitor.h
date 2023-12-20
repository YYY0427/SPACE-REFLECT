#pragma once
#include "TransitorBase.h"

/// <summary>
/// 画面切り替え演出
/// ワイプ
/// </summary>
class WipeTransitor final : public TransitorBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="dir">ワイプする方向</param>
	/// <param name="interval">切り替わる時間</param>
	WipeTransitor(TransitDirection dir = TransitDirection::LEFT, int interval = 60);

	// デストラクタ
	~WipeTransitor();

	// 更新
	void Update() override final;

	// 描画
	void Draw() override final;

private:
	// グラデーション用ハンドル
	int m_gradationH;
};