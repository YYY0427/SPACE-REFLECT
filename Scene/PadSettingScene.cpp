#include "PadSettingScene.h"
#include "SceneManager.h"
#include "../Util/StringManager.h"
#include "../Util/InputState.h"
#include "../Util/SaveData.h"
#include "../common.h"
#include <cassert>

namespace
{
	// 表示するテキストの全体の位置
	constexpr int draw_text_pos_x = 350;
	constexpr int draw_text_pos_y = common::screen_height / 2 - 120;

	// テキストの文字間
	constexpr int text_space_y = 64;

	// 選択されていないときの描画色
	constexpr int normal_color = 0x666666;

	// 選択されているときの描画色
	constexpr int choose_color = 0xffffff;
}

// コンストラクタ
PadSettingScene::PadSettingScene(SceneManager& manager) : 
	SceneBase(manager),
	currentSelectItem_(0)
{
	// 項目の描画色を選択されていないときの色に初期化
	for (int i = 0; i < static_cast<int>(Item::TOTAL_VALUE); i++)
	{
		itemColorTable_.push_back(normal_color);
	}
}

// デストラクタ
PadSettingScene::~PadSettingScene()
{
	// 処理なし
}

// 更新
void PadSettingScene::Update()
{
	// カラーの初期化
	for (auto& item : itemColorTable_)
	{
		item = normal_color;
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
	// パッドスティックの横感度
	case Item::PAD_SENS_X:
		SaveData::GetInstance().SetPadStickSensitivityX();
		break;

	// パッドスティックの縦感度
	case Item::PAD_SENS_Y:
		SaveData::GetInstance().SetPadStickSensitivityY();
		break;

	// パッドスティックの横反転
	case Item::PAD_REVERSE_X:
		SaveData::GetInstance().SetPadStickReverseX();
		break;

	// パッドスティックの縦反転
	case Item::PAD_REVERSE_Y:
		SaveData::GetInstance().SetPadStickReverseY();
		break;

	// 戻る
	case Item::BACK:
		if(InputState::IsTriggered(InputType::DECISION)) manager_.PopScene();
		return;

	// ありえないので止める
	default:
		assert(0);
	}

	// 戻るボタンが押されたか
	if (InputState::IsTriggered(InputType::BACK))
	{
		// ひとつ前のシーンに戻る
		manager_.PopScene();
		return;
	}
}

// 描画
void PadSettingScene::Draw()
{
	// インスタンスの取得
	auto& stringManager = StringManager::GetInstance();
	auto& saveData = SaveData::GetInstance();

	// タイトルの表示
	stringManager.DrawStringCenter("PadSettingTitle", common::screen_width / 2, 100, 0xffffff);

	// 項目の表示
	int padSensXItem = static_cast<int>(Item::PAD_SENS_X);
	int padSensYItem = static_cast<int>(Item::PAD_SENS_Y);
	int padReverseXItem = static_cast<int>(Item::PAD_REVERSE_X);
	int padReverseYItem = static_cast<int>(Item::PAD_REVERSE_Y);
	int backItem = static_cast<int>(Item::BACK);
	stringManager.DrawStringCenter("PadSettingItemSensX", draw_text_pos_x, draw_text_pos_y + text_space_y * padSensXItem, itemColorTable_[padSensXItem]);
	stringManager.DrawStringCenter("PadSettingItemSensY", draw_text_pos_x, draw_text_pos_y + text_space_y * padSensYItem, itemColorTable_[padSensYItem]);
	stringManager.DrawStringCenter("PadSettingItemReverseX", draw_text_pos_x, draw_text_pos_y + text_space_y * padReverseXItem, itemColorTable_[padReverseXItem]);
	stringManager.DrawStringCenter("PadSettingItemReverseY", draw_text_pos_x, draw_text_pos_y + text_space_y * padReverseYItem, itemColorTable_[padReverseYItem]);
	stringManager.DrawStringCenter("PadSettingItemBack", common::screen_width / 2, draw_text_pos_y + text_space_y * backItem, itemColorTable_[backItem]);
	
	// パッドの感度の表示
	stringManager.DrawFormatStringCenter("PadSettingNumber", saveData.GetSaveData().padStickSensitivityX, common::screen_width / 2, draw_text_pos_y + text_space_y * padSensXItem, itemColorTable_[padSensXItem]);
	stringManager.DrawFormatStringCenter("PadSettingNumber", saveData.GetSaveData().padStickSensitivityY, common::screen_width / 2, draw_text_pos_y + text_space_y * padSensYItem, itemColorTable_[padSensYItem]);

	// パッドの反転の表示
	DrawPadReverse(saveData.GetSaveData().padStickReverseX, padReverseXItem);
	DrawPadReverse(saveData.GetSaveData().padStickReverseY, padReverseYItem);
}

// パッドの反転のオン、オフの表示
void PadSettingScene::DrawPadReverse(bool isOn, int item)
{
	// インスタンスの取得
	auto& stringManager = StringManager::GetInstance();

	// isOnによってオン、オフ表示
	(isOn) ? 
		// true : オン表示
		(stringManager.DrawStringCenter("PadSettingItemReverseOn", common::screen_width / 2, draw_text_pos_y + text_space_y * item, itemColorTable_[item])) : 

		// false : オフ表示
		(stringManager.DrawStringCenter("PadSettingItemReverseOff", common::screen_width / 2, draw_text_pos_y + text_space_y * item, itemColorTable_[item]));
}
