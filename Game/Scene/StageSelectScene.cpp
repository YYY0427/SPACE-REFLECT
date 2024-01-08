#include "StageSelectScene.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "../Util/InputState.h"
#include "DxLib.h"
#include <string>

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
	int sceneItemTotalValue = static_cast<int>(StageItem::NUM);
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
		// ステージファイル名
		std::string fileName;

		// 選択されている項目によってステージファイル名を設定
		switch (static_cast<StageItem>(m_currentSelectItem))
		{
		case StageItem::TUTORIAL:
			fileName = "Tutorial";
			break;
		case StageItem::STAGE_1:
			fileName = "Stage1";
			break;
		case StageItem::EXIT:
			return;
		}

		// ゲームシーンに遷移
		m_manager.ChangeScene(std::make_shared<GameScene>(m_manager, fileName));
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
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(StageItem::TUTORIAL), "TUTORIAL", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(StageItem::STAGE_1), "STAGE_1", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(StageItem::EXIT), "EXIT", 0xffffff, true);

	// 現在選択中の項目の横に→を表示
	DrawString(draw_text_pos_x - text_space_y, draw_text_pos_y + text_space_y * m_currentSelectItem, "→", 0xff0000);
}