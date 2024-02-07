#include "TitleScene.h"
#include "DebugScene.h"
#include "OptionScene.h"
#include "SceneManager.h"
#include "../Util/InputState.h"
#include "../Application.h"
#include "../String/MessageManager.h"
#include "StageSelectScene.h"
#include "../Transitor/WipeTransitor.h"
#include "../Transitor/TileTransitor.h"
#include "../Transitor/FadeTransitor.h"
#include "../Transitor/IrisTransitor.h"
#include "../Transitor/PushTransitor.h"
#include "../Transitor/StripTransitor.h"
#include "DxLib.h"

namespace
{
	// 表示するテキストの全体の位置
	const int draw_text_pos_y = Application::GetInstance().GetWindowSize().height / 2 + 100;

	// テキストの文字間
	constexpr int text_space_y = 50;
}

// コンストラクタ
TitleScene::TitleScene(SceneManager& manager) :
	SceneBase(manager),
	m_currentSelectSceneItem(0)
{
	// ゲームのプレイ動画を再生

	// 画面切り替え演出の設定
	m_pTransitor = std::make_unique<FadeTransitor>();
	m_pTransitor->Start();
}

// デストラクタ
TitleScene::~TitleScene()
{
}

// 更新
void TitleScene::Update()
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
		switch (static_cast<SceneItem>(m_currentSelectSceneItem))
		{
		case SceneItem::GAME:
			m_manager.ChangeScene(std::make_shared<StageSelectScene>(m_manager));
			return;
		case SceneItem::EXIT:
			Application::GetInstance().Exit();
			return;
		}
	}

	// 画面切り替え演出の更新
	m_pTransitor->Update();
}

void TitleScene::Draw()
{
	// 画面をクリア
	ClearDrawScreen();

	// 背景色の描画
	auto& size = Application::GetInstance().GetWindowSize();	
	DrawBox(0, 0, size.width, size.height, 0xffffff, true);

	// タイトルの描画
	auto& messageManager = MessageManager::GetInstance();
	messageManager.DrawStringCenter("TitleLogo", size.width / 2, 300, 0x000000);

	// 項目の描画
	messageManager.DrawStringCenter("TitleItemStart", size.width / 2, 
		draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::GAME), 0x000000);
	messageManager.DrawStringCenter("TitleItemExit", size.width / 2, 
		draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::EXIT), 0x000000);

	// 選択中の項目にバーを描画
	messageManager.DrawStringCenter("TitleItemSelectBarRight", size.width / 2 - 100, 
		draw_text_pos_y + text_space_y * m_currentSelectSceneItem, 0x000000);
	messageManager.DrawStringCenter("TitleItemSelectBarLeft", size.width / 2 + 100, 
		draw_text_pos_y + text_space_y * m_currentSelectSceneItem, 0x000000);

	// 画面切り替え演出の描画
	m_pTransitor->Draw();
}
