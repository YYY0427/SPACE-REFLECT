#pragma once
#include "TransitorBase.h"

/// <summary>
/// 画面切り替え演出
/// 押し出し
/// </summary>
class PushTransitor final : public TransitorBase
{
public:
	PushTransitor(TransitDirection dir = TransitDirection::UP, int interval = 60);
	~PushTransitor();

	void Update() override final;
	void Draw() override final;

private:
	// 方向
	TransitDirection m_direction;
};

