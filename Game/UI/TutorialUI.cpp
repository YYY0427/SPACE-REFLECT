#include "TutorialUI.h"
#include "../Util/DrawFunctions.h"
#include "../Application.h"
#include "../String/MessageManager.h"
#include "../Util/Easing.h"
#include "../SoundManager.h"
#include <DxLib.h>

namespace
{
	// 画像ファイルパス
	const std::string xbox_stick_l_up = "Data/Image/xbox_stick_l_up.png";
	const std::string xbox_stick_l_down = "Data/Image/xbox_stick_l_down.png";
	const std::string xbox_stick_l_left = "Data/Image/xbox_stick_l_left.png";
	const std::string xbox_stick_l_right = "Data/Image/xbox_stick_l_right.png";
	const std::string xbox_stick_r_up = "Data/Image/xbox_stick_r_up.png";
	const std::string xbox_stick_r_down = "Data/Image/xbox_stick_r_down.png";
	const std::string xbox_stick_r_left = "Data/Image/xbox_stick_r_left.png";
	const std::string xbox_stick_r_right = "Data/Image/xbox_stick_r_right.png";
	const std::string xbox_rb_file_path = "Data/Image/xbox_rb_double.png";
	const std::string xbox_lb_file_path = "Data/Image/xbox_lb_double.png";

	// アルファ値の増減速度
	constexpr int alpha_speed = 5;

	// 描画する画像の切り替えフレーム
	constexpr int change_image_frame = 120;

	// 描画する画像が2枚以上ある場合の画像の間隔
	constexpr float image_interval = 50.0f;

	// 画像と文字の間隔
	constexpr float text_interval = 50.0f;

	// 描画フレーム
	constexpr int move_tutorial_frame = 600;	// 移動
	constexpr int shield_tutorial_frame = 600;	// シールド
	constexpr int reflect_tutorial_frame = 400;	// 反射
	constexpr int cube_tutorial_frame = 400;	// キューブ

	// チュートリアル用のUIが右下に残すのかどうかのフラグ
	// まだ未完成のためfalse
	constexpr bool is_tutorial_ui_hold_test = false;
}

// コンストラクタ
TutorialUI::TutorialUI() :
	m_state(TutorialState::MOVE)
{
	// 初期化
	m_changeImageFrame = change_image_frame;

	// 画面サイズの取得
	auto& screenSize = Application::GetInstance().GetWindowSize();

	// 移動のチュートリアルのデータの設定
	std::vector<int> imgHandle;
	imgHandle.push_back(my::MyLoadGraph(xbox_stick_l_up.c_str()));
	imgHandle.push_back(my::MyLoadGraph(xbox_stick_l_down.c_str()));
	imgHandle.push_back(my::MyLoadGraph(xbox_stick_l_left.c_str()));
	imgHandle.push_back(my::MyLoadGraph(xbox_stick_l_right.c_str()));
	m_tutorialDataMap[TutorialState::MOVE].imgHandle.push_back(imgHandle);
	m_tutorialDataMap[TutorialState::MOVE].goalPos = { screenSize.width / 2 + 125.0f, screenSize.height - 100.0f };
	m_tutorialDataMap[TutorialState::MOVE].messageId = "TutorialMove";
	m_tutorialDataMap[TutorialState::MOVE].drawFrame = move_tutorial_frame;

	imgHandle.clear();
	
	// シールドのチュートリアルのデータの設定
	imgHandle.push_back(my::MyLoadGraph(xbox_rb_file_path.c_str()));
	imgHandle.push_back(my::MyLoadGraph(xbox_lb_file_path.c_str()));
	m_tutorialDataMap[TutorialState::SHIELD].imgHandle.push_back(imgHandle);
	imgHandle.clear();
	imgHandle.push_back(my::MyLoadGraph(xbox_stick_r_up.c_str()));
	imgHandle.push_back(my::MyLoadGraph(xbox_stick_r_down.c_str()));
	imgHandle.push_back(my::MyLoadGraph(xbox_stick_r_left.c_str()));
	imgHandle.push_back(my::MyLoadGraph(xbox_stick_r_right.c_str()));
	m_tutorialDataMap[TutorialState::SHIELD].imgHandle.push_back(imgHandle);
	m_tutorialDataMap[TutorialState::SHIELD].goalPos = 
	{ screenSize.width / 2.0f, screenSize.height - 100.0f };
	m_tutorialDataMap[TutorialState::SHIELD].messageId = "TutorialShield";
	m_tutorialDataMap[TutorialState::SHIELD].drawFrame = shield_tutorial_frame;
	imgHandle.clear();

	// 反射のチュートリアルのデータの設定
	m_tutorialDataMap[TutorialState::REFLECT].goalPos = 
	{ screenSize.width / 2.0f - 225.0f, screenSize.height - 100.0f };
	m_tutorialDataMap[TutorialState::REFLECT].messageId = "TutorialReflect";
	m_tutorialDataMap[TutorialState::REFLECT].drawFrame = reflect_tutorial_frame;

	// 反射2のチュートリアルのデータの設定
	m_tutorialDataMap[TutorialState::REFLECT2].goalPos =
	{ screenSize.width / 2.0f - 120.0f, screenSize.height - 100.0f };
	m_tutorialDataMap[TutorialState::REFLECT2].messageId = "TutorialReflect2";
	m_tutorialDataMap[TutorialState::REFLECT2].drawFrame = -1;

	// キューブのチュートリアルのデータの設定
	m_tutorialDataMap[TutorialState::CUBE].goalPos = 
	{ screenSize.width / 2.0f - 240.0f, screenSize.height - 100.0f };
	m_tutorialDataMap[TutorialState::CUBE].messageId = "TutorialCube";
	m_tutorialDataMap[TutorialState::CUBE].drawFrame = cube_tutorial_frame;

	// キューブ2のチュートリアルのデータの設定
	m_tutorialDataMap[TutorialState::CUBE2].goalPos =
	{ screenSize.width / 2.0f, screenSize.height - 100.0f };
	m_tutorialDataMap[TutorialState::CUBE2].messageId = "TutorialCube2";
	m_tutorialDataMap[TutorialState::CUBE2].drawFrame = -1;

	// プレイのチュートリアルのデータの設定
	m_tutorialDataMap[TutorialState::PLAY].goalPos =
	{ screenSize.width / 2.0f, screenSize.height - 100.0f };
	m_tutorialDataMap[TutorialState::PLAY].messageId = "TutorialPlay";
	m_tutorialDataMap[TutorialState::PLAY].drawFrame = -1;
}

// デストラクタ
TutorialUI::~TutorialUI()
{
	// 画像の解放
	for (auto& img : m_tutorialDataMap)
	{
		for (auto& handle : img.second.imgHandle)
		{
			for (auto& data : handle)
			{
				DeleteGraph(data);
			}
		}
		DeleteGraph(img.second.messageTextImgHandle);
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
			// チュートリアル用のUIが右下に残すのかどうかのフラグが立っている場合
			if (is_tutorial_ui_hold_test)
			{
				// 拡大率を減少
				data.second.scale -= 0.01f;
				data.second.scale = (std::max)(data.second.scale, 0.6f);

				// 目的地の設定
				auto& screenSize = Application::GetInstance().GetWindowSize();
				Vector2 goalPos = { static_cast<float>(screenSize.width) - 200.0f, 
									static_cast<float>(screenSize.height - 30.0f) };

				// 終了している状態の数をカウント
				int count = 0;
				for (auto& data : m_tutorialDataMap)
				{
					if (data.second.isEnd)
					{
						count++;
					}
				}
				// カウントの数によって目的地を変更
				goalPos.y -= 30.0f * count;

				// 目的地へイージングを使用して移動
				data.second.easing++;
				data.second.imgPos.x = Easing::EaseOutCubic(data.second.easing, 300, goalPos.x, data.second.imgPos.x);
				data.second.imgPos.y = Easing::EaseOutCubic(data.second.easing, 300, goalPos.y, data.second.imgPos.y);
			}
			else
			{
				// 透明度を減少
				data.second.imgAlpha -= alpha_speed;

				// 徐々に上に移動
				data.second.imgPos.y -= 1.0f;
			}
		}
		// 状態が開始している場合
		else if (data.first == m_state && data.second.isStart)
		{
			// フレーム加算
			if (data.second.drawFrame > -1)
			{
				data.second.drawFrame--;
				data.second.drawFrame = (std::max)(data.second.drawFrame, 0);

				if (data.second.drawFrame <= 0)
				{
					EndState();
				}
			}

			// 透明度を増加
			data.second.imgAlpha += alpha_speed;
			data.second.imgAlpha = (std::min)(data.second.imgAlpha, 255);

			// 徐々に上に移動
			data.second.imgPos.y -= 1.0f;
			data.second.imgPos.y = (std::max)(data.second.imgPos.y, data.second.goalPos.y);

			// 特定のフレームごとに画像を切り替え
			m_changeImageFrame.Update(1);
			if (m_changeImageFrame.IsTimeOut())
			{
				// 初期化
				m_changeImageFrame.Reset();

				// 画像の切り替え
				for (size_t i = 0; i < data.second.imgHandle.size(); i++)
				{
					data.second.imgIndex[i] = (data.second.imgIndex[i] + 1) % data.second.imgHandle[i].size();
				}
			}
		}
	}
}

// 描画
void TutorialUI::Draw()
{
	// チュートリアル用のUIが右下に残すのかどうかのフラグが立っている場合
	if (is_tutorial_ui_hold_test)
	{
		for (auto& data : m_tutorialDataMap)
		{
			// 状態がスタートしていない場合は描画しない	
			if (!data.second.isStart) return;

			// 書き込み
			SetDrawScreen(data.second.messageTextImgHandle);
			ClearDrawScreen();
			auto& screenSize = Application::GetInstance().GetWindowSize();
			MessageManager::GetInstance().DrawStringCenter(data.second.messageId,
				screenSize.width / 2.0f, screenSize.height / 2.0f, 0xffffff);
			SetDrawScreen(DX_SCREEN_BACK);

			// 透明度の設定
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, data.second.imgAlpha);

			// メッセージ画像の描画
			DrawRotaGraph(data.second.imgPos.x + 130.0f, data.second.imgPos.y,
				data.second.scale, 0.0f, data.second.messageTextImgHandle, true);

			// 画像の描画
		/*	DrawRotaGraph(data.second.imgPos.x, data.second.imgPos.y,
				data.second.scale, 0.0,
				data.second.imgHandle[data.second.imgIndex], TRUE);*/

			// 透明度を元に戻す
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
	else
	{
		// 状態がスタートしていない場合は描画しない
		if(m_tutorialDataMap[m_state].isStart == false) return;

		// 透明度の設定
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_tutorialDataMap[m_state].imgAlpha);

		// 描画位置の保存
		Vector2 savePos = m_tutorialDataMap[m_state].imgPos;

		// 画像の描画
		for (size_t i = 0; i < m_tutorialDataMap[m_state].imgHandle.size(); i++)
		{
			// 画像の間隔を設定
			if (i >= 1)
			{
				//	画像のサイズを取得
				float width = 0.0f, height = 0.0f;
				GetGraphSizeF(m_tutorialDataMap[m_state].imgHandle[i - 1][m_tutorialDataMap[m_state].imgIndex[i -1]],
					&width, &height);

				// 画像の位置をずらす
				m_tutorialDataMap[m_state].imgPos.x += (width + image_interval);
			}

			// 画像の描画
			DrawRotaGraph(m_tutorialDataMap[m_state].imgPos.x, m_tutorialDataMap[m_state].imgPos.y,
						  1.0, 0.0,
						  m_tutorialDataMap[m_state].imgHandle[i][m_tutorialDataMap[m_state].imgIndex[i]], TRUE);

			// 画像と画像の間にテキストを挿入
			if (m_tutorialDataMap[m_state].imgHandle.size() >= 2 && 
				i != m_tutorialDataMap[m_state].imgHandle.size() - 1)
			{
				//	画像のサイズを取得
				float width = 0.0f, height = 0.0f;
				GetGraphSizeF(m_tutorialDataMap[m_state].imgHandle[i][m_tutorialDataMap[m_state].imgIndex[i]],
					&width, &height);

				// テキストの描画
				MessageManager::GetInstance().DrawStringCenter(
					"TutorialUI+", 
					m_tutorialDataMap[m_state].imgPos.x + ((width + image_interval) / 2.0f),
					m_tutorialDataMap[m_state].imgPos.y, 
					0xffffff);
			}

		}

		// テキストの描画
		auto messageHeight = MessageManager::GetInstance().GetMessageHeight(m_tutorialDataMap[m_state].messageId);
		MessageManager::GetInstance().DrawString(m_tutorialDataMap[m_state].messageId, 
					  m_tutorialDataMap[m_state].imgPos.x + text_interval,
					  m_tutorialDataMap[m_state].imgPos.y - (messageHeight / 2.0f), 0xffffff);
		
		// 描画位置の復元
		m_tutorialDataMap[m_state].imgPos.x = savePos.x;

		// 透明度を元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

// 状態の設定
void TutorialUI::StartState(const TutorialState state)
{
	// すでにスタートしている場合は何もしない
	if (m_tutorialDataMap[state].isStart) return;

	// 現在の状態が終了していない場合は何もしない
	if (m_tutorialDataMap[m_state].imgAlpha > 0) return;
	if (m_tutorialDataMap[m_state].isStart && !m_tutorialDataMap[m_state].isEnd) return;

	SoundManager::GetInstance().PlaySE("TutorialUI");

	// 状態の設定
	m_state = state;
	m_tutorialDataMap[m_state].isStart = true;
	m_tutorialDataMap[m_state].isEnd = false;

	// 位置の設定
	m_tutorialDataMap[m_state].imgPos = 
		{ m_tutorialDataMap[m_state].goalPos.x, m_tutorialDataMap[m_state].goalPos.y + 20.0f };

	// 画像のインデックスの初期化
	m_tutorialDataMap[m_state].imgIndex.resize(m_tutorialDataMap[m_state].imgHandle.size(), 0);

	// チュートリアル用のUIが右下に残すのかどうかのフラグが立っている場合
	if (is_tutorial_ui_hold_test)
	{
		// メッセージ画像用のスクリーンの作成
		MessageManager& message = MessageManager::GetInstance();
		m_tutorialDataMap[m_state].messageTextImgHandle =
			MakeScreen(message.GetMessageWidth(m_tutorialDataMap[m_state].messageId),
					   message.GetMessageHeight(m_tutorialDataMap[m_state].messageId));
	}
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
bool TutorialUI::IsEndState(const TutorialState state)
{
	return m_tutorialDataMap[state].isEnd;
}