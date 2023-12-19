#pragma once
#include "TransitorBase.h"

class StripTransitor final : public TransitorBase
{
public:
	StripTransitor(int width = 100, int interval = 60);
	~StripTransitor();

	void Update() override final;
	void Draw() override final;

private:
	int m_width;
};