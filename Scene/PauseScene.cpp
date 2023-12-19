#include "PauseScene.h"
#include "SceneManager.h"
#include "OptionScene.h"
#include "TitleScene.h"
#include "DebugScene.h"
#include "../Util/InputState.h"
#include "../Util/SoundManager.h"
#include "../Util/StringManager.h"
#include "../common.h"
#include <DxLib.h>
#include <cassert>

namespace
{
	// 表示するテキストの全体の位置
	constexpr int draw_text_pos_x = common::screen_width / 2;
	constexpr int draw_text_pos_y = common::screen_height / 2 - 100;

	// テキストの文字間
	constexpr int text_space_y = 64;
}

// コンストラクタ
PauseScene::PauseScene(SceneManager& manager) :
	SceneBase(manager),
	currentSelectItem_(0)
{
	// フェードインを行わない
	SetFadeBright(0);

	// 各項目によってフェードの設定
	fadeConfigTable_[static_cast<int>(Item::CONTINUE)] = { 0, 8, false, false, false };
	fadeConfigTable_[static_cast<int>(Item::OPTION)] = { 255, 16, false , false, true };
	fadeConfigTable_[static_cast<int>(Item::TITLE)] = { 255, 8, true, true, false };
}

// デストラクタ
PauseScene::~PauseScene()
{
	// 処理なし
}

// 更新
void PauseScene::Update()
{
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

	// 決定ボタンを押したらフェードアウト開始
	if (InputState::IsTriggered(InputType::DECISION))
	{
		// 選択肢によってフェードの値を設定
		// フェードアウト開始
		StartFadeOut(fadeConfigTable_[currentSelectItem_].fadeValue, fadeConfigTable_[currentSelectItem_].fadeSpeed);
	}

	// ポーズ画面から戻る
	if (InputState::IsTriggered(InputType::BACK) || InputState::IsTriggered(InputType::PAUSE))
	{
		manager_.PopScene(); 
		return;
	}

	// フェードアウトが終わりしだい選択された項目に飛ぶ
	if (IsStartFadeOutAfterFadingOut())
	{
		switch (static_cast<Item>(currentSelectItem_))
		{
		// 続ける
		case Item::CONTINUE:
			manager_.PopScene();
			return;

		// オプション
		case Item::OPTION:
			manager_.PushScene(new OptionScene(manager_));
			break;

		// タイトル
		case Item::TITLE:
			// 全てのサウンドを止める
			SoundManager::GetInstance().StopAllSound();
#ifdef _DEBUG
			// タイトルに遷移
			manager_.PopAllSceneAndChangeScene(new DebugScene(manager_));
#else 
			manager_.PopAllSceneAndChangeScene(new TitleScene(manager_));
#endif
			return;

		// ありえないので止める
		default:
			assert(0);
		}

		// PushSceneした場合シーンが残ったままなので
		// フェードインの設定
		StartFadeIn();
		return;
	}

	// フェードの更新
	UpdateFade();
}

// 描画
void PauseScene::Draw()
{
	// インスタンスの取得
	auto& stringManager = StringManager::GetInstance();

	// 項目に設定されたフェード設定に該当するときだけ文字をフェードする
	if (fadeConfigTable_[currentSelectItem_].isOnlyStringFade)
	{
		// 描画透明度の設定
		int fade = 255 - GetFadeBright();
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade);
	}

	// 項目の表示
	stringManager.DrawStringCenter("PauseTitle", draw_text_pos_x, 100, 0xffffff);
	stringManager.DrawStringCenter("PauseItemContinue", draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(Item::CONTINUE), 0xffffff);
	stringManager.DrawStringCenter("PauseItemOption", draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(Item::OPTION), 0xffffff);
	stringManager.DrawStringCenter("PauseItemTitle", draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(Item::TITLE), 0xffffff);

	// 選択されている項目にバーを描画
	stringManager.DrawStringCenter("PausetemSelectBarRight", draw_text_pos_x - 100, draw_text_pos_y + text_space_y * currentSelectItem_, 0xffffff);
	stringManager.DrawStringCenter("PausetemSelectBarLeft", draw_text_pos_x + 100, draw_text_pos_y + text_space_y * currentSelectItem_, 0xffffff);

	// 描画の設定の初期化
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// フェードの描画 
	// 項目に設定されたフェード設定に該当するときだけフェードする
	DrawFade(fadeConfigTable_[currentSelectItem_].isFade);

	// モザイクフェードの描画
	// 項目に設定されたフェード設定に該当するときだけフェードする
	DrawGaussFade(fadeConfigTable_[currentSelectItem_].isGaussFade);
}