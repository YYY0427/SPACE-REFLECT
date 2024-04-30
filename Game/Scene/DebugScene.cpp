#include "DebugScene.h"
#include "TitleScene.h"
#include "TestScene.h"
#include "OptionScene.h"
#include "StageSelectScene.h"
#include "SceneManager.h"
#include "../Util/InputState.h"
#include "../Application.h"
#include "DxLib.h"

namespace
{
	// 表示するテキストの全体の位置
	const int draw_text_pos_x = Application::GetInstance().GetWindowSize().width / 2 - 100;
	const int draw_text_pos_y = Application::GetInstance().GetWindowSize().height / 2 - 100;

	// テキストの文字間
	constexpr int text_space_y = 32;
}

// コンストラクタ
DebugScene::DebugScene(SceneManager& manager) :
	SceneBase(manager),
	m_currentSelectSceneItem(0)
{
}

// デストラクタ
DebugScene::~DebugScene()
{
}

// 初期化
void DebugScene::Init()
{
}

// 終了処理
void DebugScene::End()
{
}

// 更新
void DebugScene::Update()
{
	// 選択肢を回す処理
	int sceneItemTotalValue = static_cast<int>(SceneItem::NUM);
	if (InputState::IsTriggered(InputType::UP))
	{
		m_currentSelectSceneItem = ((m_currentSelectSceneItem - 1) + sceneItemTotalValue) % sceneItemTotalValue;
	}
	else if (InputState::IsTriggered(InputType::DOWN))
	{
		m_currentSelectSceneItem = (m_currentSelectSceneItem + 1) % sceneItemTotalValue;
	}

	// 決定ボタンが押されたらシーン遷移
	if (InputState::IsTriggered(InputType::DECISION))
	{
		switch(static_cast<SceneItem>(m_currentSelectSceneItem))
		{
		case SceneItem::TITLE:
			m_manager.ChangeScene(std::make_shared<TitleScene>(m_manager));
			return;
		case SceneItem::STAGE_SELECT:
			m_manager.ChangeScene(std::make_shared<StageSelectScene>(m_manager));
			return;
		case SceneItem::PAUSE:
			return;
		case SceneItem::OPTION:
			m_manager.PushScene(std::make_shared<OptionScene>(m_manager, OptionScene::State::PAUSE));
			return;
		case SceneItem::TEST:
			m_manager.ChangeScene(std::make_shared<TestScene>(m_manager));
			return;
		case SceneItem::EXIT:
			Application::GetInstance().Exit();
			return;
		}
	}
}

// 描画
void DebugScene::Draw()
{
	// 画面をクリア
	ClearDrawScreen();

	// 背景の描画
	DrawBox(0, 0, Application::GetInstance().GetWindowSize().width, Application::GetInstance().GetWindowSize().height, 0x000000, true);

	// デバッグシーンから飛べるシーンの項目のテキスト表示
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::TITLE), "TITLE", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::STAGE_SELECT), "STAGE_SELECT", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::PAUSE), "PAUSE", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::OPTION), "OPTION", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::TEST), "TEST", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::EXIT), "EXIT", 0xffffff, true);

	// 現在選択中の項目の横に→を表示
	DrawString(draw_text_pos_x - text_space_y, draw_text_pos_y + text_space_y * m_currentSelectSceneItem, "→", 0xff0000);
}