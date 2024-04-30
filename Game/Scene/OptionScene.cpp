#include "OptionScene.h"
#include "SceneManager.h"
#include "StageSelectScene.h"
#include "../Util/DrawFunctions.h"
#include "../Util/InputState.h"
#include "../SaveData.h"
#include "../String/MessageManager.h"
#include "../Application.h"
#include "../Transitor/FadeTransitor.h"
#include "../StateMachine.h"
#include "../SoundManager.h"
#include "DxLib.h"
#include <cassert>
#include <string>
#include <array>

namespace
{
	// 画像ファイルのパス
	const std::string sound_icon_img_file_path = "Data/Image/Sound.png";
	const std::string xbox_rb_file_path = "Data/Image/xbox_rb.png";
	const std::string xbox_lb_file_path = "Data/Image/xbox_lb.png";

	// モザイクの使用ピクセル幅(8, 16, 32 の何れか)
	constexpr int gauss_param_pixel = 8;

	// モザイクの強さ(100で約1ピクセル分の幅)
	constexpr int gauss_param_power = 600;

	// 選択されていないときの描画色
	constexpr unsigned int normal_color = 0x444444;

	// 選択されているときの描画色
	constexpr unsigned int choose_color = 0xffffff;

	// 表示するテキストの全体の位置
	constexpr int draw_text_pos_x = 200;
	const int draw_text_pos_y = Application::GetInstance().GetWindowSize().height / 2 - 100;

	// テキストの文字間
	constexpr int text_space_y = 64;

	// 音量を何段階に分けるか
	constexpr int volume_division = 5;

	// 音量アイコンの選択されていないときの描画輝度(0 暗い ～ 255 明るい)
	constexpr int normal_draw_bright = 70;

	// 音量アイコンどうしの間隔
	constexpr int volume_icon_space = 70;
}

// コンストラクタ
OptionScene::OptionScene(SceneManager& manager, const State state) :
	SceneBase(manager),
	m_currentSelectItem(0),
	m_soundIconHandle(-1),
	m_lbButtonImgHandle(-1),
	m_rbButtonImgHandle(-1),
	m_gaussScreen(-1),
	m_state(state)
{
}

// デストラクタ
OptionScene::~OptionScene()
{
}

// 初期化
void OptionScene::Init()
{
	// 画像のロード
	m_soundIconHandle = my::MyLoadGraph(sound_icon_img_file_path.c_str());
	m_lbButtonImgHandle = my::MyLoadGraph(xbox_lb_file_path.c_str());
	m_rbButtonImgHandle = my::MyLoadGraph(xbox_rb_file_path.c_str());

	// ステートマシンの初期化
	m_updateStateMachine.AddState(State::STAGE_SELECT, {}, [this] { UpdateStageSelect(); }, {});
	m_updateStateMachine.AddState(State::PAUSE, {}, [this] { UpdatePause(); }, {});
	m_drawStateMachine.AddState(State::STAGE_SELECT, {}, [this] { DrawStageSelect(); }, {});
	m_drawStateMachine.AddState(State::PAUSE, {}, [this] { DrawPause(); }, {});
	m_updateStateMachine.SetState(m_state);
	m_drawStateMachine.SetState(m_state);

	// モザイク処理用のグラフィックの作成
	const auto& size = Application::GetInstance().GetWindowSize();
	m_gaussScreen = MakeScreen(size.width, size.height);

	// 項目の描画色を選択されていないときの色に初期化
	for (int i = 0; i < static_cast<int>(OptionItem::NUM); i++)
	{
		m_itemColorTable.push_back(normal_color);
	}
}

// 終了処理
void OptionScene::End()
{
	// 画像のアンロード
	DeleteGraph(m_soundIconHandle);
	DeleteGraph(m_lbButtonImgHandle);
	DeleteGraph(m_rbButtonImgHandle);

	// モザイク処理用のグラフィックの削除
	DeleteGraph(m_gaussScreen);
}

// 更新
void OptionScene::Update()
{
	// カラーの初期化
	for (auto& itemColor : m_itemColorTable)
	{
		itemColor = normal_color;
	}

	// 選択肢を回す処理
	int itemTotalValue = static_cast<int>(OptionItem::NUM);
	if (InputState::IsTriggered(InputType::UP))
	{
		m_currentSelectItem = ((m_currentSelectItem - 1) + itemTotalValue) % itemTotalValue;
		SoundManager::GetInstance().PlaySE("Select");
	}
	else if (InputState::IsTriggered(InputType::DOWN))
	{
		m_currentSelectItem = (m_currentSelectItem + 1) % itemTotalValue;
		SoundManager::GetInstance().PlaySE("Select");
	}

	// 選択されている項目の色を変える
	m_itemColorTable[m_currentSelectItem] = choose_color;

	// 選択されている項目がどれか
	switch(static_cast<OptionItem>(m_currentSelectItem))
	{ 
		// ウィンドウモードの設定
	case OptionItem::WINDOW_MODE:
		SaveData::GetInstance().SetWindowMode();
		break;

		// 全体音量の調整
	case OptionItem::MASTER_VOLUME:
		SaveData::GetInstance().SetMasterVolume(volume_division);
		break;

		// BGM音量の調整
	case OptionItem::BGM_VOLUME:
		SaveData::GetInstance().SetBgmVolume(volume_division);
		break;

		// SE音量の調整
	case OptionItem::SE_VOLUME:
		SaveData::GetInstance().SetSeVolume(volume_division);
		break;
	default:
		// ありえないので止める
		assert(false);
	}

	// ステートマシンの更新
	m_updateStateMachine.Update();
}

// ステージセレクトの更新
void OptionScene::UpdateStageSelect()
{
	// 特定のボタンが押されたとき
	if (InputState::IsTriggered(InputType::LEFT_SHOULDER))
	{
		// 終了
		SoundManager::GetInstance().PlaySE("Select");
		m_manager.PopScene();
		return;
	}
}

// ポーズの更新
void OptionScene::UpdatePause()
{
	// 戻るボタンが押されたとき
	if (InputState::IsTriggered(InputType::BACK) || InputState::IsTriggered(InputType::PAUSE))
	{
		// 終了
		m_manager.PopScene();
		return;
	}
}

// 描画
void OptionScene::Draw()
{
	// ステートマシンの更新
	m_drawStateMachine.Update();

	// ウィンドウサイズの取得
	const auto& size = Application::GetInstance().GetWindowSize();

	// インスタンスの取得
	auto& messageManager = MessageManager::GetInstance();

	// 項目の描画
	int windowMode = static_cast<int>(OptionItem::WINDOW_MODE);
	messageManager.DrawString("OptionItemWindowMode", draw_text_pos_x,
		draw_text_pos_y + text_space_y * windowMode, m_itemColorTable[windowMode]);

	int masterVolume = static_cast<int>(OptionItem::MASTER_VOLUME);
	messageManager.DrawString("OptionItemMasterVolume", draw_text_pos_x,
		draw_text_pos_y + text_space_y * masterVolume, m_itemColorTable[masterVolume]);

	int bgmVolume = static_cast<int>(OptionItem::BGM_VOLUME);
	messageManager.DrawString("OptionItemBgmVolume", draw_text_pos_x,
		draw_text_pos_y + text_space_y * bgmVolume, m_itemColorTable[bgmVolume]);

	int seVolume = static_cast<int>(OptionItem::SE_VOLUME);
	messageManager.DrawString("OptionItemSeVolume", draw_text_pos_x,
		draw_text_pos_y + text_space_y * seVolume, m_itemColorTable[seVolume]);

	// ウィンドウモードの状態の表示
	auto& saveData = SaveData::GetInstance();
	(saveData.GetSaveData().windowMode) ?
		messageManager.DrawStringCenter("OptionItemWindowModeOff", size.width / 2 + 120, 
			draw_text_pos_y + text_space_y * windowMode, m_itemColorTable[windowMode]):
		messageManager.DrawStringCenter("OptionItemWindowModeOn", size.width / 2 + 120,
			draw_text_pos_y + text_space_y * windowMode, m_itemColorTable[windowMode]);

	// 音量の表示
	int masterVolumeValue = saveData.GetSaveData().masterVolume;
	int bgmVolumeValue = saveData.GetSaveData().bgmVolume;
	int seVolumeValue = saveData.GetSaveData().seVolume;

	// 音量の配列
	std::array<int, 3> volumeValue = 
	{ masterVolumeValue, bgmVolumeValue, seVolumeValue };

	// 音量の項目
	int volumeItem = static_cast<int>(OptionItem::MASTER_VOLUME);

	// 音量の描画
	for (int i = 0; i < volumeValue.size(); i++)
	{
		for (int j = 0; j < volumeValue[i]; j++)
		{
			// 描画輝度を設定
			// 暗く描画
			SetDrawBright(normal_draw_bright, normal_draw_bright, normal_draw_bright);

			// 選択されている項目の場合は描画輝度を戻す
			if (m_currentSelectItem == volumeItem) SetDrawBright(255, 255, 255);

			// 音量の間隔を計算
			int textSpaceX = j * volume_icon_space;

			// 音量のアイコンを描画
			DrawRotaGraph(620 + textSpaceX,
				draw_text_pos_y + text_space_y * volumeItem + 10, 0.2, 0.0, m_soundIconHandle, true);
		}
		volumeItem++;
	}
	// 描画輝度をもとに戻す
	SetDrawBright(255, 255, 255);
}

// ステージセレクトの描画
void OptionScene::DrawStageSelect()
{
	// 背景の描画
	auto& screenSize = Application::GetInstance().GetWindowSize();
	DrawBox(0, 0, screenSize.width, screenSize.height, 0x222222, true);

	// ステージセレクトタイトルの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawRoundRectAA((screenSize.width / 2.0f) - 325, 50, (screenSize.width / 2.0f) - 50, 110, 5, 5, 8, 0xffffff, true);
	MessageManager::GetInstance().DrawStringCenter("MissionTitle", (screenSize.width / 2.0f) - 187, 80, 0x000000);

	// RBボタンの描画
	DrawRotaGraph((screenSize.width / 2.0f) + 375, 95, 1.0f, 0.0f, m_rbButtonImgHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// オプションタイトルの描画
	DrawRoundRectAA((screenSize.width / 2.0f) + 325, 50, (screenSize.width / 2.0f) + 50, 110, 5, 5, 8, 0xffffff, true);
	MessageManager::GetInstance().DrawStringCenter("OptionTitle", (screenSize.width / 2.0f) + 187, 80, 0x000000);

	// LBボタンの描画
	DrawRotaGraph((screenSize.width / 2.0f) - 375, 95, 1.0f, 0.0f, m_lbButtonImgHandle, true);

	// 線の描画
	DrawLineAA(0 + 100, 120, screenSize.width - 100, 120, 0xffffff, 3.0f);
	DrawLineAA(450, 170, 450, screenSize.height - 100, 0xffffff, 3.0f);
}

// ポーズの描画
void OptionScene::DrawPause()
{
	// モザイク処理
	const auto& size = Application::GetInstance().GetWindowSize();
	GetDrawScreenGraph(0, 0, size.width, size.height, m_gaussScreen);
	GraphFilter(m_gaussScreen, DX_GRAPH_FILTER_GAUSS, gauss_param_pixel, gauss_param_power);
	DrawGraph(0, 0, m_gaussScreen, true);

	// 画面を暗くする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, size.width, size.height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// シーンタイトルの描画
	auto& messageManager = MessageManager::GetInstance();
	messageManager.DrawStringCenter("PauseOptionTitle", size.width / 2, 100, 0xffffff);
}
