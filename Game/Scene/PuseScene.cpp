#include "PuseScene.h"
#include "SceneManager.h"
#include "OptionScene.h"
#include "StageSelectScene.h"
#include "TitleScene.h"
#include "ConfirmScene.h"
#include "../Input/InputManager.h"
#include "../Sound/SoundManager.h"
#include "../MyDebug/DebugText.h"
#include "../Application.h"
#include "../String/MessageManager.h"

namespace
{ 
	// モザイクの使用ピクセル幅(8, 16, 32 の何れか)
	constexpr int gauss_param_pixel = 8;

	// モザイクの強さ(100で約1ピクセル分の幅)
	constexpr int gauss_param_power = 600;

	// 画面を暗くする強さ(最大255)
	constexpr int dark_param_power = 200;

	// テキストの文字間
	constexpr int text_space_y = 64;

	// 選択されていないときの描画色
	constexpr unsigned int normal_color = 0x444444;

	// 選択されているときの描画色
	constexpr unsigned int choose_color = 0xffffff;

	// 表示するテキストの全体の位置
	const int draw_text_pos_x = Application::GetInstance()->GetWindowSize().width / 2;
	const int draw_text_pos_y = Application::GetInstance()->GetWindowSize().height / 2 - 50;
}

namespace Scene
{
	// コンストラクタ	
	Puse::Puse(const std::shared_ptr<Scene::Manager>& pSceneManager, const Stage stage) :
		Base(pSceneManager),
		m_currentSelectItem(0),
		m_stage(stage),
		m_gaussScreen(-1)
	{
		// ロード画面を使用しない
		m_isUseLoadScene = false;
	}

	// デストラクタ
	Puse::~Puse()
	{
	}

	// 初期化
	void Puse::Init()
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
	void Puse::End()
	{
		// モザイク処理用のグラフィックの削除
		DeleteGraph(m_gaussScreen);
	}

	// 更新
	void Puse::Update()
	{
		// カラーの初期化
		for (auto& itemColor : m_itemColorTable)
		{
			itemColor = normal_color;
		}

		// 選択肢を回す処理
		int itemTotalValue = static_cast<int>(State::NUM);
		if (Input::Manager::IsTriggered(Input::Type::UP))
		{
			m_currentSelectItem = ((m_currentSelectItem - 1) + itemTotalValue) % itemTotalValue;
			Sound::Manager::GetInstance()->PlaySE("Select");
		}
		else if (Input::Manager::IsTriggered(Input::Type::DOWN))
		{
			m_currentSelectItem = (m_currentSelectItem + 1) % itemTotalValue;
			Sound::Manager::GetInstance()->PlaySE("Select");
		}

		// 選択されている項目の色を変える
		m_itemColorTable[m_currentSelectItem] = choose_color;

		// 決定
		if (Input::Manager::IsTriggered(Input::Type::DECISION))
		{
			// 選択されている項目がどれか
			switch (static_cast<State>(m_currentSelectItem))
			{
				// 続ける
			case State::CONTINUE:
				m_pSceneManager->PopScene();
				break;

				// オプション
			case State::OPTION:
				m_pSceneManager->PushScene(std::make_shared<Scene::Option>(m_pSceneManager, Scene::Option::State::PAUSE));
				break;

				// ステージセレクトに戻る
			case State::STAGE_SELECT:
				m_pSceneManager->PushScene(std::make_shared<Scene::Confirm>(m_pSceneManager, State::STAGE_SELECT));
				break;

				// タイトルに戻る
			case State::TITLE:
				m_pSceneManager->PushScene(std::make_shared<Scene::Confirm>(m_pSceneManager, State::TITLE));
				break;
			}
		}

		// ポーズ解除
		if (Input::Manager::IsTriggered(Input::Type::PAUSE) ||
			Input::Manager::IsTriggered(Input::Type::BACK))
		{
			m_pSceneManager->PopScene();
			return;
		}
	}

	// 描画
	void Puse::Draw()
	{
		// デバッグ用ログ
		Debug::Text::AddLog("currentSelectState", { m_currentSelectItem });

		// 画面サイズの取得
		auto& screenSize = Application::GetInstance()->GetWindowSize();

		// モザイク処理
		GetDrawScreenGraph(0, 0, screenSize.width, screenSize.height, m_gaussScreen);
		GraphFilter(m_gaussScreen, DX_GRAPH_FILTER_GAUSS, gauss_param_pixel, gauss_param_power);
		DrawGraph(0, 0, m_gaussScreen, true);

		// 画面を暗くする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, dark_param_power);
		DrawBox(0, 0, screenSize.width, screenSize.height, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// メッセージマネージャの取得
		const auto& messageManager = String::MessageManager::GetInstance();

		// メニューの描画
		messageManager->DrawStringCenter("PauseMenu", screenSize.width / 2.0f, 150, 0xffffff, 0xffffff);

		// 項目の描画
		int continueItem = static_cast<int>(State::CONTINUE);
		messageManager->DrawStringCenter("PauseContinue",
			draw_text_pos_x,
			draw_text_pos_y + text_space_y * continueItem,
			m_itemColorTable[continueItem]);

		int optionItem = static_cast<int>(State::OPTION);
		messageManager->DrawStringCenter("PauseOption",
			draw_text_pos_x,
			draw_text_pos_y + text_space_y * optionItem,
			m_itemColorTable[optionItem]);

		int stageSelectItem = static_cast<int>(State::STAGE_SELECT);
		messageManager->DrawStringCenter("PauseStageSelect",
			draw_text_pos_x,
			draw_text_pos_y + text_space_y * stageSelectItem,
			m_itemColorTable[stageSelectItem]);

		int restartItem = static_cast<int>(State::TITLE);
		messageManager->DrawStringCenter("PauseTitle",
			draw_text_pos_x,
			draw_text_pos_y + text_space_y * restartItem,
			m_itemColorTable[restartItem]);
	}
}