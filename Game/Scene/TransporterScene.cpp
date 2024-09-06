#include "TransporterScene.h"
#include "TitleScene.h"
#include "TestScene.h"
#include "OptionScene.h"
#include "StageSelectScene.h"
#include "SceneManager.h"
#include "../Input/InputManager.h"
#include "../Application.h"
#include "DxLib.h"

namespace
{
	// 表示するテキストの全体の位置
	const int draw_text_pos_x = Application::GetInstance()->GetWindowSize().width / 2 - 100;
	const int draw_text_pos_y = Application::GetInstance()->GetWindowSize().height / 2 - 100;

	// テキストの文字間
	constexpr int text_space_y = 32;
}

namespace Scene
{
	// コンストラクタ
	Transporter::Transporter(const std::shared_ptr<Scene::Manager>& pSceneManager) :
		Base(pSceneManager),
		m_selectIndex(0)
	{
	}

	// デストラクタ
	Transporter::~Transporter()
	{
	}

	// 初期化
	void Transporter::Init()
	{
		// シーン名を設定
		m_pSceneTable[SceneItem::TITLE] = "TitleScene";
		m_pSceneTable[SceneItem::STAGE_SELECT] = "StageSelectScene";
		m_pSceneTable[SceneItem::PAUSE] = "PauseScene";
		m_pSceneTable[SceneItem::OPTION] = "OptionScene";
		m_pSceneTable[SceneItem::TEST] = "TestScene";
		m_pSceneTable[SceneItem::EXIT] = "Exit";
	}

	// 終了処理
	void Transporter::End()
	{
	}

	// 更新
	void Transporter::Update()
	{
		// 選択肢を回す処理
		int sceneItemTotalValue = static_cast<int>(SceneItem::NUM);
		if (Input::Manager::IsTriggered(Input::Type::UP))
		{
			m_selectIndex = ((m_selectIndex - 1) + sceneItemTotalValue) % sceneItemTotalValue;
		}
		else if (Input::Manager::IsTriggered(Input::Type::DOWN))
		{
			m_selectIndex = (m_selectIndex + 1) % sceneItemTotalValue;
		}

		// 決定ボタンが押されたらシーン遷移
		if (Input::Manager::IsTriggered(Input::Type::DECISION))
		{
			switch (static_cast<SceneItem>(m_selectIndex))
			{
			case SceneItem::TITLE:
				m_pSceneManager->ChangeScene(std::make_shared<Title>(m_pSceneManager));
				return;
			case SceneItem::STAGE_SELECT:
				m_pSceneManager->ChangeScene(std::make_shared<StageSelect>(m_pSceneManager));
				return;
			case SceneItem::PAUSE:
				return;
			case SceneItem::OPTION:
				m_pSceneManager->PushScene(std::make_shared<Option>(m_pSceneManager, Option::State::PAUSE));
				return;
			case SceneItem::TEST:
				m_pSceneManager->ChangeScene(std::make_shared<Test>(m_pSceneManager));
				return;
			case SceneItem::EXIT:
				Application::GetInstance()->Exit();
				return;
			}
		}
	}

	// 描画
	void Transporter::Draw()
	{
		// 画面をクリア
		ClearDrawScreen();

		// 背景の描画
		DrawBox(0, 0, Application::GetInstance()->GetWindowSize().width, Application::GetInstance()->GetWindowSize().height, 0x000000, true);

		// デバッグシーンから飛べるシーンの項目のテキスト表示
		for (auto& scene : m_pSceneTable)
		{
			// シーン名を表示
			DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(scene.first), scene.second.c_str(), 0xffffff, true);

			// 選択中のシーンに矢印を表示
			if (scene.first == static_cast<SceneItem>(m_selectIndex))
			{
				DrawString(draw_text_pos_x - 50, draw_text_pos_y + text_space_y * static_cast<int>(scene.first), "→", 0xffffff, true);
			}
		}

		//// デバッグシーンから飛べるシーンの項目のテキスト表示
		//DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::TITLE), "TITLE", 0xffffff, true);
		//DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::STAGE_SELECT), "STAGE_SELECT", 0xffffff, true);
		//DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::PAUSE), "PAUSE", 0xffffff, true);
		//DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::OPTION), "OPTION", 0xffffff, true);
		//DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::TEST), "TEST", 0xffffff, true);
		//DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::EXIT), "EXIT", 0xffffff, true);

		//// 現在選択中の項目の横に→を表示
		//DrawString(draw_text_pos_x - text_space_y, draw_text_pos_y + text_space_y * m_selectIndex, "→", 0xff0000);
	}
}