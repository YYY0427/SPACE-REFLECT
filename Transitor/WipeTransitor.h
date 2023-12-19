#pragma once
#include "TransitorBase.h"

class WipeTransitor final : public TransitorBase
{
public:
	WipeTransitor(TransitDirection dir = TransitDirection::LEFT, int interval = 60);
	~WipeTransitor();

	void Update() override final;
	void Draw() override final;

private:
	// グラデーション用ハンドル
	int m_gradationH;
};