#pragma once
#include "TransitorBase.h"

/// <summary>
/// 画面切り替え演出
/// 短冊状切り替え
/// </summary>
class StripTransitor final : public TransitorBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="width">短冊の幅</param>
	/// <param name="interval">切り替わる時間</param>
	StripTransitor(int width = 100, int interval = 60);

	// デストラクタ
	~StripTransitor();

	// 更新
	void Update() override final;

	// 描画
	void Draw() override final;

private:
	// 短冊の幅
	int m_width;
};