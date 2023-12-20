#pragma once
#include "TransitorBase.h"

/// <summary>
/// 画面切り替え演出
/// 押し出し
/// </summary>
class PushTransitor final : public TransitorBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="dir">押し出す方向</param>
	/// <param name="interval">切り替わる時間</param>
	PushTransitor(TransitDirection dir = TransitDirection::UP, int interval = 60);

	// デストラクタ
	~PushTransitor();

	// 更新
	void Update() override final;

	// 描画
	void Draw() override final;

private:
	// 方向
	TransitDirection m_direction;
};

