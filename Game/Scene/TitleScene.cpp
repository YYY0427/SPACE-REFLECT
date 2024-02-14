#include "TitleScene.h"
#include "DebugScene.h"
#include "OptionScene.h"
#include "../Transitor/Fade.h"
#include "SceneManager.h"
#include "../Util/InputState.h"
#include "../Application.h"
#include "../String/MessageManager.h"
#include "StageSelectScene.h"
#include "../Transitor/WipeTransitor.h"
#include "../Transitor/TileTransitor.h"
#include "../Transitor/FadeTransitor.h"
#include "../Transitor/IrisTransitor.h"
#include "../Transitor/PushTransitor.h"
#include "../Transitor/StripTransitor.h"
#include "DxLib.h"

namespace
{
	// 表示するテキストの全体の位置
	const int draw_text_pos_y = Application::GetInstance().GetWindowSize().height / 2 + 150;

	// テキストの文字間
	constexpr int text_space_y = 50;

	// 文字の点滅の速さ
	constexpr float string_fade_speed = 0.05f;
}

// コンストラクタ
TitleScene::TitleScene(SceneManager& manager) :
	SceneBase(manager),
	m_currentSelectSceneItem(0),
	m_alpha(255),
	m_frame(0),
	m_isInput(false)
{
	// フェードインの演出
	m_pFade = std::make_unique<Fade>();
	m_pFade->StartFadeIn(0);

	// ゲームのプレイ動画を再生


	// 画面切り替え演出の設定
	/*m_pTransitor = std::make_unique<TileTransitor>();
	m_pTransitor->Start();*/

	// ガウスハンドルの作成
	auto& size = Application::GetInstance().GetWindowSize();
	m_gaussHandle = MakeScreen(size.width, size.height);
}

// デストラクタ
TitleScene::~TitleScene()
{
}

// 更新
void TitleScene::Update()
{
	// どこかのボタンが押されたら次のシーンに遷移
	if (InputState::IsTriggered(InputType::ANY_BUTTON))
	{
		// フェードアウトの演出の開始
		m_pFade->StartFadeOut(255);

		// フラグを立てる
		m_isInput = true;
	}

	// PRESS ANY BUTTONの点滅処理
	// 255と0を行ったり来たりする
	m_frame++;
	m_alpha = (0.5f * sinf(m_frame * string_fade_speed) + 0.5f) * 255.0f;

	// ボタンが押されてフェードアウトが終了したら次のシーンに遷移
	if (!m_pFade->IsFading() && m_isInput)
	{
		switch (static_cast<SceneItem>(m_currentSelectSceneItem))
		{
		case SceneItem::GAME:
			m_manager.ChangeScene(std::make_shared<StageSelectScene>(m_manager));
			return;
		}
	}

	// フェードの更新
	m_pFade->Update();
//	m_pTransitor->Update();
}

void TitleScene::Draw()
{
	// 画面をクリア
	ClearDrawScreen();

	// タイトルロゴの描画
	auto& size = Application::GetInstance().GetWindowSize();	
	auto& messageManager = MessageManager::GetInstance();
	SetDrawScreen(m_gaussHandle);
	ClearDrawScreen();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
	messageManager.DrawStringCenter("TitleLogo", size.width / 2, 300, 0xffffff);
	GraphFilter(m_gaussHandle, DX_GRAPH_FILTER_GAUSS, 32, 1400);
	SetDrawScreen(DX_SCREEN_BACK);

	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	DrawGraph(0, 0, m_gaussHandle, true);
	DrawGraph(0, 0, m_gaussHandle, true);
	DrawGraph(0, 0, m_gaussHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); 
	messageManager.DrawStringCenter("TitleLogo", size.width / 2, 300, 0xffffff, 0xffffff);

	// 項目の描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
	messageManager.DrawStringCenter("TitleItemStart", size.width / 2, 
		draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::GAME), 0xffffff);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// フェードの描画
	m_pFade->DrawFade(true);

	// 画面切り替え演出の描画
//	m_pTransitor->Draw();
}
