#include "StripTransitor.h"
#include "../Application.h"
#include "DxLib.h"

StripTransitor::StripTransitor(int width, int interval) :
	TransitorBase(interval),
	m_width(width)
{
}

StripTransitor::~StripTransitor()
{
}

void StripTransitor::Update()
{
	if (m_frame < m_interval) 
	{
		m_frame++;
		SetDrawScreen(m_nextScene);
	}
	else if (m_frame == m_interval) 
	{
		SetDrawScreen(DX_SCREEN_BACK);
	}
}

void StripTransitor::Draw()
{
	if (IsEnd()) return;
	SetDrawScreen(DX_SCREEN_BACK);
	const auto& size = Application::GetInstance().GetWindowSize();
	auto rate = static_cast<float>(m_frame) / static_cast<float>(m_interval);
	int lp = (size.width / m_width) + 1;
	DrawRectGraph(0, 0, 0, 0, size.width, size.height, m_oldScene, true);
	for (int i = 0; i < lp; ++i) {
		if (i % 2 == 0) {
			DrawRectGraph(i * m_width, size.height * (rate - 1.0f), i * m_width, 0, m_width, size.height, m_nextScene, true);
		}
		else {
			DrawRectGraph(i * m_width, size.height * (1.0f - rate), i * m_width, 0, m_width, size.height, m_nextScene, true);
		}
	}
}