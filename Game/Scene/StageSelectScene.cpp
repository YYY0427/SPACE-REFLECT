#include "StageSelectScene.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "../Util/InputState.h"
#include <DxLib.h>

// コンストラクタ
StageSelectScene::StageSelectScene(SceneManager& manager) :
	SceneBase(manager),
	m_currentSelectItem(0)
{
}

// デストラクタ
StageSelectScene::~StageSelectScene()
{
}

// 更新
void StageSelectScene::Update()
{
	// 選択肢を回す処理
	int sceneItemTotalValue = static_cast<int>(Stage::NUM);
	if (InputState::IsTriggered(InputType::UP))
	{
		m_currentSelectItem = ((m_currentSelectItem - 1) + sceneItemTotalValue) % sceneItemTotalValue;
	}
	else if (InputState::IsTriggered(InputType::DOWN))
	{
		m_currentSelectItem = (m_currentSelectItem + 1) % sceneItemTotalValue;
	}

	// 決定ボタンが押されたらシーン遷移
	if (InputState::IsTriggered(InputType::DECISION))
	{
		// 前のシーンに戻る
		if (m_currentSelectItem == static_cast<int>(Stage::EXIT))
		{
			// 戻る
			m_manager.ChangeScene(std::make_shared<DebugScene>(m_manager));
			return;
		}

		// ゲームシーンに遷移
		m_manager.ChangeScene(
			std::make_shared<GameScene>(m_manager, static_cast<Stage>(m_currentSelectItem)));
		return;
	}
}

// 描画
void StageSelectScene::Draw()
{
	// 画面をクリア
	ClearDrawScreen();

	// 飛べるシーンの項目のテキスト表示
	constexpr int draw_text_pos_x = 500;
	constexpr int draw_text_pos_y = 200;
	constexpr int text_space_y = 32;
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(Stage::TUTORIAL), "TUTORIAL", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(Stage::STAGE_1), "STAGE_1", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(Stage::EXIT), "EXIT", 0xffffff, true);

	// 現在選択中の項目の横に→を表示
	DrawString(draw_text_pos_x - text_space_y, draw_text_pos_y + text_space_y * m_currentSelectItem, "→", 0xff0000);
}