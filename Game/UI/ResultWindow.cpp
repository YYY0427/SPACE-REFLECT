#include "ResultWindow.h"
#include "../Application.h"
#include "../Util/InputState.h"
#include <DxLib.h>

namespace
{
	// ウィンドウサイズ
	const Vector2 window_size = { 800, 500 };

	// ウィンドウの移動速度
	constexpr float window_move_speed = 20.0f;
}

// コンストラクタ
ResultWindow::ResultWindow() :
	m_isEnd(false)
{
	auto& screenSize = Application::GetInstance().GetWindowSize();
	m_windowPos = { screenSize.width * 2.0f, screenSize.height / 2.0f };
}

// デストラクタ
ResultWindow::~ResultWindow()
{
}

// 更新
void ResultWindow::Update()
{
	// ウィンドウを移動
	auto& screenSize = Application::GetInstance().GetWindowSize();
	if (m_windowPos.x > screenSize.width / 2.0f)
	{
		m_windowPos.x -= window_move_speed;
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
	DrawBox(m_windowPos.x - (window_size.x / 2), m_windowPos.y - (window_size.y / 2),
			m_windowPos.x + (window_size.x / 2), m_windowPos.y + (window_size.y / 2), 0xffffff, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// 終了フラグの取得
bool ResultWindow::IsEnd() const
{
	return m_isEnd;
}
