#include "OptionScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "PadSettingScene.h"
#include "../Util/InputState.h"
#include "../Util/SoundManager.h"
#include "../Util/SaveData.h"
#include "../Util/StringManager.h"
#include "../Util/DrawFunctions.h"
#include "../common.h"
#include <DxLib.h>
#include <array>
#include <cassert>

namespace
{
	// 表示するテキストの全体の位置
	constexpr int draw_text_pos_x = 350;
	constexpr int draw_text_pos_y = common::screen_height / 2 - 150;

	// テキストの文字間
	constexpr int text_space_y = 64;

	// 選択されていないときの描画色
	constexpr int normal_color = 0x666666;

	// 選択されているときの描画色
	constexpr int choose_color = 0xffffff;
}

// コンストラクタ
OptionScene::OptionScene(SceneManager& manager) :
	SceneBase(manager),
	currentSelectItem_(0),
	soundIconImgHandle_(-1)
{
	// フェードインを行わない
	SetFadeBright(0);

	// 画像のロード
	soundIconImgHandle_ = my::MyLoadGraph("Data/Image/Sound.png");

	// 項目の描画色を選択されていないときの色に初期化
	for (int i = 0; i < static_cast<int>(Item::TOTAL_VALUE); i++)
	{
		itemColorTable_.push_back(normal_color);
	}
}

// デストラクタ
OptionScene::~OptionScene()
{
	// 画像ハンドルの削除
	DeleteGraph(soundIconImgHandle_);
}

// 更新
void OptionScene::Update()
{
	// カラーの初期化
	for (auto& itemColor : itemColorTable_)
	{
		itemColor = normal_color;
	}

	// 選択肢を回す処理
	int itemTotalValue = static_cast<int>(Item::TOTAL_VALUE);
	if (InputState::IsTriggered(InputType::UP))
	{
		currentSelectItem_ = ((currentSelectItem_ - 1) + itemTotalValue) % itemTotalValue;
	}
	else if (InputState::IsTriggered(InputType::DOWN))
	{
		currentSelectItem_ = (currentSelectItem_ + 1) % itemTotalValue;
	}

	// 選択されている項目の色を変える
	itemColorTable_[currentSelectItem_] = choose_color;

	// 選択されている項目がどれか
	switch (static_cast<Item>(currentSelectItem_))
	{
	// 言語設定
	case Item::LANGUAGE:
		break;

	// ウィンドウモードの設定
	case Item::WINDOW_MODE:
		SaveData::GetInstance().SetWindowMode();
		break;

	// 全体音量の調整
	case Item::MASTER_VOLUME:
		SaveData::GetInstance().SetMasterVolume();
		break;

	// BGM音量の調整
	case Item::BGM_VOLUME:
		SaveData::GetInstance().SetBgmVolume();
		break;

	// SE音量の調整
	case Item::SE_VOLUME:
		SaveData::GetInstance().SetSeVolume();
		break;

	// パッドの設定シーンに遷移するためにフェードアウト開始
	case Item::PAD_SETTING:
		if(InputState::IsTriggered(InputType::DECISION)) StartFadeOut(255, 16);
		break;

	// ひとつ前のシーンに戻る
	case Item::BACK:
		if (InputState::IsTriggered(InputType::DECISION)) manager_.PopScene();
		return;

	// ありえないので止める
	default:
		assert(0);
	}
	
	// フェードアウトが終了したか
	if (IsStartFadeOutAfterFadingOut())
	{
		// PushSceneした場合シーンが残るのでフェードイン設定
		StartFadeIn();

		// パッド設定シーンへ遷移
		manager_.PushScene(new PadSettingScene(manager_));
		return;
	}

	// 戻るボタンが押されたとき
	if (InputState::IsTriggered(InputType::BACK))
	{
		// ひとつ前のシーンに戻る
		manager_.PopScene();
		return;
	}

	// フェードの更新
	UpdateFade();
}

// 描画
void OptionScene::Draw()
{
	// インスタンスの取得
	auto& stringManager = StringManager::GetInstance();
	auto& saveData = SaveData::GetInstance();

	// 描画透明度の設定
	int fade = 255 - GetFadeBright();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade);

	// シーンタイトルの描画
	stringManager.DrawStringCenter("OptionTitle", common::screen_width / 2, 100, 0xffffff);

	// 項目の描画
	int language = static_cast<int>(Item::LANGUAGE);
	int windowMode = static_cast<int>(Item::WINDOW_MODE);
	int masterVolume = static_cast<int>(Item::MASTER_VOLUME);
	int bgmVolume = static_cast<int>(Item::BGM_VOLUME);
	int seVolume = static_cast<int>(Item::SE_VOLUME);
	int padSetting = static_cast<int>(Item::PAD_SETTING);
	int back = static_cast<int>(Item::BACK);
	stringManager.DrawStringCenter("OptionItemLanguage", draw_text_pos_x, draw_text_pos_y + text_space_y * language, itemColorTable_[language]);
	stringManager.DrawStringCenter("OptionItemWindowMode", draw_text_pos_x, draw_text_pos_y + text_space_y * windowMode, itemColorTable_[windowMode]);
	stringManager.DrawStringCenter("OptionItemMasterVolume", draw_text_pos_x, draw_text_pos_y + text_space_y * masterVolume, itemColorTable_[masterVolume]);
	stringManager.DrawStringCenter("OptionItemBgmVolume", draw_text_pos_x, draw_text_pos_y + text_space_y * bgmVolume, itemColorTable_[bgmVolume]);
	stringManager.DrawStringCenter("OptionItemSeVolume", draw_text_pos_x, draw_text_pos_y + text_space_y * seVolume, itemColorTable_[seVolume]);
	stringManager.DrawStringCenter("OptionItemPadSetting", common::screen_width / 2, draw_text_pos_y + text_space_y * padSetting, itemColorTable_[padSetting]);
	stringManager.DrawStringCenter("OptionItemBack", common::screen_width / 2, draw_text_pos_y + text_space_y * back, itemColorTable_[back]);

	// ウィンドウモードの状態の表示
	if (saveData.GetSaveData().windowMode)
	{
		stringManager.DrawStringCenter("OptionItemWindowModeOff", common::screen_width / 2, draw_text_pos_y + text_space_y * windowMode, itemColorTable_[windowMode]);
	}
	else
	{
		stringManager.DrawStringCenter("OptionItemWindowModeOn", common::screen_width / 2, draw_text_pos_y + text_space_y * windowMode, itemColorTable_[windowMode]);
	}

	// 各サウンド音量の表示
	std::array<int, 3> volume = {};
	volume[0] = SaveData::GetInstance().GetSaveData().masterVolume;
	volume[1] = SaveData::GetInstance().GetSaveData().bgmVolume;
	volume[2] = SaveData::GetInstance().GetSaveData().seVolume;
	int item = masterVolume;
	for(int i = 0; i < volume.size(); i++)
	{
		for (int j = 0; j < volume[i]; j++)
		{
			SetDrawBright(70, 70, 70);
			if (currentSelectItem_ == item)
			{
				SetDrawBright(255, 255, 255);
			}
			int textSpaceX = j * 70;
			DrawRotaGraph(draw_text_pos_x + 170 + textSpaceX, draw_text_pos_y + text_space_y * item + 10, 0.2, 0.0, soundIconImgHandle_, true);
		}
		item++;
	}
	SetDrawBright(255, 255, 255);

	// 描画の設定の初期化
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}