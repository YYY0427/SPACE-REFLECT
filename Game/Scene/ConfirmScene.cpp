#include "ConfirmScene.h"
#include "SceneManager.h"
#include "StageSelectScene.h"
#include "TitleScene.h"
#include "../Sound/SoundManager.h"
#include "../Input/InputManager.h"
#include "../Application.h"
#include "../MyDebug/DebugText.h"
#include "../String/MessageManager.h"

namespace
{
	// 項目の選択されていないときの描画色
	constexpr unsigned int normal_color = 0x444444;

	// 項目の選択されているときの描画色
	constexpr unsigned int choose_color = 0xffffff;
}

namespace Scene
{
	// コンストラクタ
	Confirm::Confirm(const std::shared_ptr<Scene::Manager>& pSceneManager, Puse::State state) :
		Base(pSceneManager),
		m_currentSelectItem(0),
		m_gaussScreen(-1),
		m_state(state)
	{
		// ロードシーンを使用しない
		m_isUseLoadScene = false;
	}

	// デストラクタ
	Confirm::~Confirm()
	{
	}

	// 初期化
	void Confirm::Init()
	{
		// モザイク処理用のグラフィックの作成
		auto& screenSize = Application::GetInstance()->GetWindowSize();
		m_gaussScreen = MakeScreen(screenSize.width, screenSize.height);

		// 項目の描画色を選択されていないときの色に初期化
		for (int i = 0; i < static_cast<int>(State::NUM); i++)
		{
			m_itemColorTable.push_back(normal_color);
		}
	}

	// 終了処理
	void Confirm::End()
	{
		// モザイク処理用のグラフィックの削除
		DeleteGraph(m_gaussScreen);
	}

	// 更新
	void Confirm::Update()
	{
		// カラーの初期化
		for (auto& itemColor : m_itemColorTable)
		{
			itemColor = normal_color;
		}

		// 選択肢を回す処理
		int itemTotalValue = static_cast<int>(Confirm::State::NUM);
		if (Input::Manager::IsTriggered(Input::Type::LEFT))
		{
			m_currentSelectItem = ((m_currentSelectItem - 1) + itemTotalValue) % itemTotalValue;
			Sound::Manager::GetInstance()->PlaySE("Select");
		}
		else if (Input::Manager::IsTriggered(Input::Type::RIGHT))
		{
			m_currentSelectItem = (m_currentSelectItem + 1) % itemTotalValue;
			Sound::Manager::GetInstance()->PlaySE("Select");
		}

		// 選択されている項目の色を変える
		m_itemColorTable[m_currentSelectItem] = choose_color;

		// 決定
		if (Input::Manager::IsTriggered(Input::Type::DECISION))
		{
			// どこのシーンに戻るか
			switch (static_cast<Confirm::State>(m_currentSelectItem))
			{
			case State::YES:
				switch (m_state)
				{
				case Puse::State::STAGE_SELECT:
					m_pSceneManager->ChangeAndClearScene(std::make_shared<StageSelect>(m_pSceneManager));
					break;

				case Puse::State::TITLE:
					m_pSceneManager->ChangeAndClearScene(std::make_shared<Title>(m_pSceneManager));
					break;
				}
				break;

			case State::NO:
				m_pSceneManager->PopScene();
				break;
			}
		}

		// 戻る
		if (Input::Manager::IsTriggered(Input::Type::BACK))
		{
			m_pSceneManager->PopScene();
			return;
		}
	}

	// 描画
	void Confirm::Draw()
	{
		Debug::Text::AddLog("currentSelectItem", { m_currentSelectItem });

		// モザイク処理
		const auto& size = Application::GetInstance()->GetWindowSize();
		GetDrawScreenGraph(0, 0, size.width, size.height, m_gaussScreen);
		GraphFilter(m_gaussScreen, DX_GRAPH_FILTER_GAUSS, 8, 600);
		DrawGraph(0, 0, m_gaussScreen, true);

		// 画面を暗くする処理
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawBox(0, 0, size.width, size.height, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// ウィンドウの描画
		DrawBox(0, size.height / 2.0f - 200, size.width, size.height / 2.0f + 200, 0x555555, true);

		// メッセージの描画
		String::MessageManager::GetInstance()->DrawStringCenter("ConfirmBack", size.width / 2.0f, size.height / 2.0f - 100, 0xffffff);
		String::MessageManager::GetInstance()->DrawStringCenter("ConfirmYes", size.width / 2.0f - 100, size.height / 2.0f + 50, m_itemColorTable[static_cast<int>(Confirm::State::YES)]);
		String::MessageManager::GetInstance()->DrawStringCenter("ConfirmNo", size.width / 2.0f + 100, size.height / 2.0f + 50, m_itemColorTable[static_cast<int>(Confirm::State::NO)]);
	}

}