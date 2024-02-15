#include "TitleScene.h"
#include "DebugScene.h"
#include "StageSelectScene.h"
#include "SceneManager.h"
#include "../Game/Camera.h"
#include "../Game/SkyDome.h"
#include "../Transitor/Fade.h"
#include "../Util/InputState.h"
#include "../Application.h"
#include "../String/MessageManager.h"
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
	constexpr int string_alpha_add = 2;

	// レーザーの位置範囲
	constexpr int laser_height = 25;
}

// コンストラクタ
TitleScene::TitleScene(SceneManager& manager) :
	SceneBase(manager),
	m_currentSelectSceneItem(0),
	m_alpha(200),
	m_frame(0),
	m_isInput(false),
	m_alphaAdd(string_alpha_add)
{
	// フェードインの演出
	m_pFade = std::make_unique<Fade>();
	m_pFade->StartFadeIn(0);

	// インスタンスの作成
	m_pCamera = std::make_shared<Camera>(Vector3(0, 0, 0), Vector3(0, 0, 1));
	m_pSkyDome = std::make_shared<SkyDome>(Vector3(0, 0, 0));

	// レーザーの配置
	SetLaser({ 0, 0 }, { 1280, 720 }, 0xff0000);
	SetLaser({ 1280, 0 }, { 0, 720 }, 0x0000ff);

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

		// PRESS ANY BUTTONの点滅を止める
		m_alphaAdd = 0;

		// フラグを立てる
		m_isInput = true;
	}

	// スカイドームの更新
	m_pSkyDome->Update(Vector3(0, 0, 0));

	// レーザーの更新
	for (auto& laser : m_laserData)
	{
		laser.alpha = GetRand(100);
		laser.thickness = GetRand(9) + 1;
		int rand = GetRand(laser_height * 2) - laser_height;
		laser.pos1.y = laser.startPos.x + rand;
		laser.pos2.y = laser.startPos.y + rand;
	}

	// PRESS ANY BUTTONの点滅処理
	// 255と50を行ったり来たりする
	m_alpha += m_alphaAdd;
	if (m_alpha >= 255 || m_alpha <= 50)
	{
		m_alphaAdd *= -1;
	}

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
}

void TitleScene::Draw()
{
	// 画面をクリア
	ClearDrawScreen();
	
	// インスタンス取得
	auto& size = Application::GetInstance().GetWindowSize();	
	auto& messageManager = MessageManager::GetInstance();

	// 描画スクリーンを切り替え
	SetDrawScreen(m_gaussHandle);

	// 切替後の描画スクリーンをクリア
	ClearDrawScreen();

	// レーザーの描画
	for (auto& laser : m_laserData)
	{
		// レーザーの白いところの描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
		DrawLine(laser.pos1.x, laser.startPos.x, laser.pos2.x, laser.startPos.y, 0xffffff, 14);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// レーザーの外側の色の描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, laser.alpha);
		DrawLine(laser.pos1.x, laser.pos1.y, laser.pos2.x, laser.pos2.y, laser.color, laser.thickness);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// タイトルロゴの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
	messageManager.DrawStringCenter("TitleLogo", size.width / 2, 300, 0xffffff, 0x000000);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ガウスフィルターの適用
	GraphFilter(m_gaussHandle, DX_GRAPH_FILTER_GAUSS, 32, 1400);

	// 描画スクリーンを戻す
	SetDrawScreen(DX_SCREEN_BACK);

	// 加算ブレンドでガウス画像を何回か重ねて描画
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	for (int i = 0; i < 3; i++)
	{
		// さっき作成したガウス画像を描画
		DrawGraph(0, 0, m_gaussHandle, true);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); 

	// スカイドームの描画
	m_pSkyDome->Draw();

	// タイトルロゴをもう一度描画
	messageManager.DrawStringCenter("TitleLogo", size.width / 2, 300, 0xffffff, GetColor(130, 130, 130));

	// 項目の描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
	messageManager.DrawStringCenter("TitleItemStart", size.width / 2, 
		draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::GAME), 0xffffff);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// フェードの描画
	m_pFade->DrawFade(true);
}

// レーザーの描画
void TitleScene::SetLaser(Vector2 pos1, Vector2 pos2, unsigned int color)
{
	for (int i = 0; i < 10; i++)
	{
		// レーザーのデータを作成
		LaserData data;
		data.alpha = GetRand(100);
		data.color = color;
		data.thickness = GetRand(9) + 1;
		data.pos1.x = pos1.x;
		data.pos2.x = pos2.x;
		data.startPos = { pos1.y, pos2.y };

		int rand = GetRand(laser_height * 2) - laser_height;
		data.pos1.y = pos1.y + rand;
		data.pos2.y = pos2.y + rand;

		m_laserData.push_back(data);
	}
}
