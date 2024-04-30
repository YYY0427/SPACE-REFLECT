#include "ConfirmScene.h"
#include "SceneManager.h"
#include "StageSelectScene.h"
#include "TitleScene.h"
#include "../SoundManager.h"
#include "../Util/InputState.h"
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

// コンストラクタ
ConfirmScene::ConfirmScene(SceneManager& manager, PuseScene::State state) :
	SceneBase(manager),
	m_currentSelectItem(0),
	m_gaussScreen(-1),
	m_state(state)
{
}

// デストラクタ
ConfirmScene::~ConfirmScene()
{
}

// 初期化
void ConfirmScene::Init()
{
	// モザイク処理用のグラフィックの作成
	auto& screenSize = Application::GetInstance().GetWindowSize();
	m_gaussScreen = MakeScreen(screenSize.width, screenSize.height);

	// 項目の描画色を選択されていないときの色に初期化
	for (int i = 0; i < static_cast<int>(State::NUM); i++)
	{
		m_itemColorTable.push_back(normal_color);
	}
}

// 終了処理
void ConfirmScene::End()
{
	// モザイク処理用のグラフィックの削除
	DeleteGraph(m_gaussScreen);
}

// 更新
void ConfirmScene::Update()
{
	// カラーの初期化
	for (auto& itemColor : m_itemColorTable)
	{
		itemColor = normal_color;
	}

	// 選択肢を回す処理
	int itemTotalValue = static_cast<int>(ConfirmScene::State::NUM);
	if (InputState::IsTriggered(InputType::LEFT))
	{
		m_currentSelectItem = ((m_currentSelectItem - 1) + itemTotalValue) % itemTotalValue;
		SoundManager::GetInstance().PlaySE("Select");
	}
	else if (InputState::IsTriggered(InputType::RIGHT))
	{
		m_currentSelectItem = (m_currentSelectItem + 1) % itemTotalValue;
		SoundManager::GetInstance().PlaySE("Select");
	}

	// 選択されている項目の色を変える
	m_itemColorTable[m_currentSelectItem] = choose_color;

	// 決定
	if (InputState::IsTriggered(InputType::DECISION))
	{
		// どこのシーンに戻るか
		switch (static_cast<ConfirmScene::State>(m_currentSelectItem))
		{
		case State::YES:
			switch (m_state)
			{
			case PuseScene::State::STAGE_SELECT:
				m_manager.ChangeAndClearScene(std::make_shared<StageSelectScene>(m_manager));
				break;

			case PuseScene::State::TITLE:
				m_manager.ChangeAndClearScene(std::make_shared<TitleScene>(m_manager));
				break;
			}
			break;

		case State::NO:
			m_manager.PopScene();
			break;
		}
	}

	// 戻る
	if (InputState::IsTriggered(InputType::BACK))
	{
		m_manager.PopScene();
		return;
	}
}

// 描画
void ConfirmScene::Draw()
{
	DebugText::AddLog("currentSelectItem", {m_currentSelectItem});

	// モザイク処理
	const auto& size = Application::GetInstance().GetWindowSize();
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
	MessageManager::GetInstance().DrawStringCenter("ConfirmBack", size.width / 2.0f, size.height / 2.0f - 100, 0xffffff);
	MessageManager::GetInstance().DrawStringCenter("ConfirmYes", size.width / 2.0f - 100, size.height / 2.0f + 50, m_itemColorTable[static_cast<int>(ConfirmScene::State::YES)]);
	MessageManager::GetInstance().DrawStringCenter("ConfirmNo", size.width / 2.0f + 100, size.height / 2.0f + 50, m_itemColorTable[static_cast<int>(ConfirmScene::State::NO)]);
}
