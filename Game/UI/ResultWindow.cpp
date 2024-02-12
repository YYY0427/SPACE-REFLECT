#include "ResultWindow.h"
#include "../Application.h"
#include "../Util/InputState.h"
#include "../Util/Easing.h"
#include "../String/MessageManager.h"
#include <DxLib.h>

namespace
{
	// ウィンドウサイズ
	const Vector2 window_size = { 1280, 570 };

	// MISSIONの位置
	const Vector2 init_mission_pos = { -300, 200 };
	const Vector2 goal_mission_pos = { 300, 200 };

	// COMPLETEの位置
	const Vector2 init_complete_pos = { 1500, 270 };
	const Vector2 goal_complete_pos = { 440, 270 };
}

// コンストラクタ
ResultWindow::ResultWindow() :
	m_isEnd(false),
	m_isEndWindow(false),
	m_easing({ 0.0f, 0.0f }),
	m_missionPos(init_mission_pos),
	m_completePos(init_complete_pos)
{
	auto& screenSize = Application::GetInstance().GetWindowSize();
	m_windowPos = { screenSize.width / 2.0f, screenSize.height / 2.0f };
	m_windowSize = { 0, 0 };
}

// デストラクタ
ResultWindow::~ResultWindow()
{
}

// 更新
void ResultWindow::Update()
{
	// 徐々にウィンドウを大きくする
	if (!m_isEndWindow)
	{
		m_easing.x++;
		m_windowSize.x = Easing::EaseOutCubic(m_easing.x, 100, window_size.x, m_windowSize.x);
		if (m_windowSize.x >= window_size.x)
		{
			m_easing.y++;
			m_windowSize.y = Easing::EaseOutCubic(m_easing.y, 100, window_size.y, m_windowSize.y);
			if (m_windowSize.y >= window_size.y)
			{
				m_isEndWindow = true;
				m_easing = { 0.0f, 0.0f };
			}
		}
	}

	// ウィンドウが終了したら
	if (m_isEndWindow)
	{
		// MISSIONの位置を移動
		m_easing.x++;
		m_missionPos.x = Easing::EaseOutCubic(m_easing.x, 100, goal_mission_pos.x, m_missionPos.x);
		m_missionPos.y = Easing::EaseOutCubic(m_easing.x, 100, goal_mission_pos.y, m_missionPos.y);

		// COMPLETEの位置を移動
		m_easing.y++;
		m_completePos.x = Easing::EaseOutCubic(m_easing.y, 100, goal_complete_pos.x, m_completePos.x);
		m_completePos.y = Easing::EaseOutCubic(m_easing.y, 100, goal_complete_pos.y, m_completePos.y);
	}

	// 決定ボタンが押されたらシーン遷移
	if (InputState::IsTriggered(InputType::DECISION))
	{
		m_isEnd = true;
	}
}

// 描画
void ResultWindow::Draw()
{
	auto& screenSize = Application::GetInstance().GetWindowSize();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(m_windowPos.x - (m_windowSize.x / 2), m_windowPos.y - (m_windowSize.y / 2),
			m_windowPos.x + (m_windowSize.x / 2), m_windowPos.y + (m_windowSize.y / 2), 0xeeeeee, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// MISSIONの描画
	MessageManager::GetInstance().DrawStringCenter(
		"ResultMission", m_missionPos.x, m_missionPos.y, GetColor(255, 255, 255), 0xffffff);
	// COMPLETEの描画
	MessageManager::GetInstance().DrawStringCenter(
		"ResultComplete", m_completePos.x, m_completePos.y, GetColor(255, 255, 255), 0xffffff);
}

// 終了フラグの取得
bool ResultWindow::IsEnd() const
{
	return m_isEnd;
}
