#include "TutorialUI.h"
#include "../Util/DrawFunctions.h"
#include "../Application.h"
#include "../String/MessageManager.h"
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
			// 透明度を減少
			data.second.imgAlpha -= alpha_speed;

			// 徐々に上に移動
			data.second.imgPos.y -= 1.0f;
		}
		// 状態が開始している場合
		else if (data.first == m_state && data.second.isStart)
		{
			// 透明度を増加
			data.second.imgAlpha += alpha_speed;
			data.second.imgAlpha = (std::min)(data.second.imgAlpha, 255);
			/*data.second.textAlpha += alpha_speed;
			data.second.textAlpha = (std::min)(data.second.textAlpha, 255);*/

			// 徐々に上に移動
			data.second.imgPos.y -= 1.0f;
			data.second.imgPos.y = (std::max)(data.second.imgPos.y, data.second.imgGoalPos.y);

			// 特定のフレームごとに画像を切り替え
			m_changeImageFrame.Update(1);
			if (m_changeImageFrame.IsTimeOut())
			{
				// 画像が2枚以下の場合は透明度を0にする
				if (data.second.imgHandle.size() >= 2)
				{
				//	data.second.imgAlpha = 0;
				}
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
	// 透明度の設定
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_tutorialDataMap[m_state].imgAlpha);

	// 画像の描画
	DrawRotaGraph(m_tutorialDataMap[m_state].imgPos.x, m_tutorialDataMap[m_state].imgPos.y, 
				  1.0, 0.0,	
				  m_tutorialDataMap[m_state].imgHandle[m_tutorialDataMap[m_state].imgIndex], TRUE);

	// テキストの描画
	MessageManager::GetInstance().DrawStringCenter(m_tutorialDataMap[m_state].messageId, 
				  m_tutorialDataMap[m_state].imgPos.x + 130.0f, m_tutorialDataMap[m_state].imgPos.y, 0xffffff);
	
	// 透明度を元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// 状態の設定
void TutorialUI::StartState(TutorialState state)
{
	m_state = state;
	m_tutorialDataMap[m_state].isStart = true;
	m_tutorialDataMap[m_state].isEnd = false;
	m_tutorialDataMap[m_state].imgPos = 
		{ m_tutorialDataMap[m_state].imgGoalPos.x, m_tutorialDataMap[m_state].imgGoalPos.y + 20.0f };
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