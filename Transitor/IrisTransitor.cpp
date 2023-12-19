#include "IrisTransitor.h"
#include "../Application.h"
#include "DxLib.h"
#include <cmath>

IrisTransitor::IrisTransitor(bool irisOut, int interval, bool isTiled, int gHandle) :
	TransitorBase(interval),
	m_irisOut(irisOut),
	m_isTiled(isTiled),
	m_gHandle(gHandle)
{
	// ウィンドウサイズの取得
	const auto& size = Application::GetInstance().GetWindowSize();

	// マスクスクリーンの作成
	m_handleForMaskScreen = MakeScreen(size.width, size.height, true);
	m_maskH = CreateMaskScreen();

	// 対角線の長さを計算
	m_diagonalLength = std::hypotf(size.width, size.height) / 2.0f;
}

IrisTransitor::~IrisTransitor()
{
	// マスクスクリーンの削除
	DeleteMaskScreen();
}

void IrisTransitor::Update()
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

void IrisTransitor::Draw()
{
	if (IsEnd()) return;

	auto rate = static_cast<float>(m_frame) / static_cast<float>(m_interval);
	int backRT = m_oldScene;
	int maskedRT = m_nextScene;
	if (m_irisOut) 
	{
		backRT = m_nextScene;
		maskedRT = m_oldScene;
		rate = 1.0f - rate;
	}
	float radius = (m_diagonalLength)*rate;
	SetDrawScreen(m_handleForMaskScreen);
	ClearDrawScreen();

	if (m_gHandle == -1)
	{
		if (m_isTiled) 
		{
			constexpr int xdiv = (640 / 50) + 1;
			constexpr int ydiv = (640 / 50) + 1;
			radius = (50 * m_diagonalLength / 320) * rate;
			for (int y = 0; y < xdiv; y++) 
			{
				for (int x = 0; x < xdiv; x++) 
				{
					DrawCircleAA(50 + x * 100, 50 + y * 100, radius, 32, 0xffffff, true);
				}
			}
		}
		else 
		{
			DrawCircleAA(320, 240, radius, 32, 0xffffff, true);
		}
	}
	else 
	{
		DrawRotaGraphFast(320, 240, rate * 3.5f, rate * 2.0f, m_gHandle, true);
	}

	//隠し関数(現在のグラフィックハンドルをマスクスクリーンに転送)
	SetMaskScreenGraph(m_handleForMaskScreen);

	//描画領域を反転する
	SetMaskReverseEffectFlag(true);

	SetDrawScreen(DX_SCREEN_BACK);
	SetUseMaskScreenFlag(false);
	DrawGraph(0, 0, backRT, true);
	SetUseMaskScreenFlag(true);
	DrawGraph(0, 0, maskedRT, true);
	SetUseMaskScreenFlag(false);
}