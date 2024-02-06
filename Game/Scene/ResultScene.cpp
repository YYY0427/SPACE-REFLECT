#include "SceneManager.h"
#include "ResultScene.h"
#include "StageSelectScene.h"
#include "../Application.h"
#include "../Util/InputState.h"
#include "../Score/Score.h"
#include "../Score/ScoreRanking.h"
#include <DxLib.h>

namespace
{
	// ウィンドウサイズ
	const Vector2 window_size = { 800, 500 };

	// ウィンドウの移動速度
	constexpr float window_move_speed = 20.0f;
}

// コンストラクタ
ResultScene::ResultScene(SceneManager& manager, std::string stageName) :
	SceneBase(manager),
	m_stageName(stageName)
{
	auto& screenSize = Application::GetInstance().GetWindowSize();
	m_windowPos = { screenSize.width * 2.0f, screenSize.height / 2.0f };
}

// デストラクタ
ResultScene::~ResultScene()
{
}

// 更新
void ResultScene::Update()
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
		// スコアをランキングに追加
		ScoreRanking::GetInstance().AddScore(m_stageName, "No Name", Score::GetInstance().GetTotalScore());

		// スコアを初期化
		Score::GetInstance().Reset();

		// シーン遷移
		m_manager.ChangeAndClearScene(std::make_shared<StageSelectScene>(m_manager));
		return;
	}
}

// 描画
void ResultScene::Draw()
{
	auto& screenSize = Application::GetInstance().GetWindowSize();
//	SetDrawBlendMode(DX_BLENDMODE_MULA, 180);
	DrawBox(m_windowPos.x - (window_size.x / 2), m_windowPos.y - (window_size.y / 2),
			m_windowPos.x + (window_size.x / 2), m_windowPos.y + (window_size.y / 2), 0xffffff, TRUE);
//	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}