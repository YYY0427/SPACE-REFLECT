#include "TutorialUI.h"
#include "../Util/DrawFunctions.h"
#include "../Application.h"
#include "../String/MessageManager.h"
#include "../Util/Easing.h"
#include <DxLib.h>

namespace
{
	// 画像ファイルパス
	const std::string xbox_stick_l_up = "Data/Image/xbox_stick_l_up.png";
	const std::string xbox_stick_l_down = "Data/Image/xbox_stick_l_down.png";
	const std::string xbox_stick_l_left = "Data/Image/xbox_stick_l_left.png";
	const std::string xbox_stick_l_right = "Data/Image/xbox_stick_l_right.png";

	// アルファ値の増減速度
	constexpr int alpha_speed = 5;
}

// コンストラクタ
TutorialUI::TutorialUI() :
	m_state(TutorialState::MOVE)
{
	// 初期化
	m_changeImageFrame = 120;

	// 画面サイズの取得
	auto& screenSize = Application::GetInstance().GetWindowSize();

	// 画像のロード
	m_tutorialDataMap[TutorialState::MOVE].imgHandle.push_back(my::MyLoadGraph(xbox_stick_l_up.c_str()));
	m_tutorialDataMap[TutorialState::MOVE].imgHandle.push_back(my::MyLoadGraph(xbox_stick_l_down.c_str()));
	m_tutorialDataMap[TutorialState::MOVE].imgHandle.push_back(my::MyLoadGraph(xbox_stick_l_left.c_str()));
	m_tutorialDataMap[TutorialState::MOVE].imgHandle.push_back(my::MyLoadGraph(xbox_stick_l_right.c_str()));
	m_tutorialDataMap[TutorialState::MOVE].imgGoalPos = { screenSize.width / 2 + 150.0f, screenSize.height - 150.0f };
	m_tutorialDataMap[TutorialState::MOVE].messageId = "TutorialMove";
}

// デストラクタ
TutorialUI::~TutorialUI()
{
	// 画像の解放
	for (auto& img : m_tutorialDataMap)
	{
		for (auto& img : img.second.imgHandle)
		{
			DeleteGraph(img);
		}
	}
}

// 更新
void TutorialUI::Update()
{
	for (auto& data : m_tutorialDataMap)
	{
		// 状態が終了している場合
		if (data.second.isEnd)
		{
			//// 透明度を減少
			//data.second.imgAlpha -= alpha_speed;

			//// 徐々に上に移動
			//data.second.imgPos.y -= 1.0f;

			data.second.scale -= 0.01f;
			data.second.scale = (std::max)(data.second.scale, 0.6f);

			// 目的地の設定
			auto& screenSize = Application::GetInstance().GetWindowSize();
			Vector2 goalPos = { static_cast<float>(screenSize.width) - 200.0f, static_cast<float>(screenSize.height - 30.0f) };
			int count = 0;
			for (auto& data : m_tutorialDataMap)
			{
				if (data.second.isEnd)
				{
					count++;
				}
			}
			goalPos.y -= 30.0f * count;

			// 目的地へイージングを使用して移動
			data.second.easing++;
			data.second.imgPos.x = Easing::EaseOutCubic(data.second.easing, 300, goalPos.x, data.second.imgPos.x);
			data.second.imgPos.y = Easing::EaseOutCubic(data.second.easing, 300, goalPos.y, data.second.imgPos.y);
		}
		// 状態が開始している場合
		else if (data.first == m_state && data.second.isStart)
		{
			// 透明度を増加
			data.second.imgAlpha += alpha_speed;
			data.second.imgAlpha = (std::min)(data.second.imgAlpha, 255);

			// 徐々に上に移動
			data.second.imgPos.y -= 1.0f;
			data.second.imgPos.y = (std::max)(data.second.imgPos.y, data.second.imgGoalPos.y);

			// 特定のフレームごとに画像を切り替え
			m_changeImageFrame.Update(1);
			if (m_changeImageFrame.IsTimeOut())
			{
				// 初期化
				m_changeImageFrame.Reset();

				// 画像の切り替え
				data.second.imgIndex = (data.second.imgIndex + 1) % data.second.imgHandle.size();
			}
		}
	}
}

// 描画
void TutorialUI::Draw()
{
	for (auto& data : m_tutorialDataMap)
	{
		// 透明度の設定
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, data.second.imgAlpha);

		// 書き込み
		SetDrawScreen(data.second.messageTextImgHandle);
		ClearDrawScreen();
		MessageManager::GetInstance().DrawStringCenter(data.second.messageId,
			data.second.imgPos.x + 130.0f, data.second.imgPos.y, 0xffffff);
		SetDrawScreen(DX_SCREEN_BACK);

		// メッセージ画像の描画
		auto& screenSize = Application::GetInstance().GetWindowSize();
		DrawRotaGraph(screenSize.width / 2.0f, screenSize.height / 2.0f, data.second.scale, 0.0f, data.second.messageTextImgHandle, true);

		// 画像の描画
		DrawRotaGraph(data.second.imgPos.x, data.second.imgPos.y,
			data.second.scale, 0.0,
			data.second.imgHandle[data.second.imgIndex], TRUE);

		// 透明度を元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//// 透明度の設定
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_tutorialDataMap[m_state].imgAlpha);

	//// 画像の描画
	//DrawRotaGraph(m_tutorialDataMap[m_state].imgPos.x, m_tutorialDataMap[m_state].imgPos.y, 
	//			  1.0, 0.0,	
	//			  m_tutorialDataMap[m_state].imgHandle[m_tutorialDataMap[m_state].imgIndex], TRUE);

	//// テキストの描画
	//MessageManager::GetInstance().DrawStringCenter(m_tutorialDataMap[m_state].messageId, 
	//			  m_tutorialDataMap[m_state].imgPos.x + 130.0f, m_tutorialDataMap[m_state].imgPos.y, 0xffffff);
	//
	//// 透明度を元に戻す
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// 状態の設定
void TutorialUI::StartState(TutorialState state)
{
	m_state = state;
	m_tutorialDataMap[m_state].isStart = true;
	m_tutorialDataMap[m_state].isEnd = false;
	m_tutorialDataMap[m_state].imgPos = 
		{ m_tutorialDataMap[m_state].imgGoalPos.x, m_tutorialDataMap[m_state].imgGoalPos.y + 20.0f };

	// TODO : メッセージの高さと横幅を取得して画像を作成
//	m_tutorialDataMap[TutorialState::MOVE].messageTextImgHandle = MakeScreen(data.TRUE);
}

// 現在の状態を終了
void TutorialUI::EndState()
{
	m_tutorialDataMap[m_state].isEnd = true;
}

// 状態の取得
TutorialState TutorialUI::GetState() const
{
	return m_state;
}

// 状態が終了しているか
bool TutorialUI::IsEndState(TutorialState state)
{
	return m_tutorialDataMap[state].isEnd;
}