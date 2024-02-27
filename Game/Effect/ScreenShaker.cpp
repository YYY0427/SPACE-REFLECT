#include "ScreenShaker.h"
#include "../Game/Camera.h"
#include "../Application.h"
#include <DxLib.h>
#include <cassert>

namespace
{
	// 揺れの減衰率
	constexpr float quake_attenuation = -0.95f;
}

// コンストラクタ
ScreenShaker::ScreenShaker(const std::shared_ptr<Camera>& pCamera) :
	m_pCamera(pCamera),
	m_quakeSize(0, 0),
	m_isPreDraw(false),
	m_screenHandle(-1)
{
	// 初期化
	m_quakeTimer.SetTime(0);

	// 画面のサイズを取得
	auto& screenSize = Application::GetInstance().GetWindowSize();

	// 画面のサイズで画面を作成
	m_screenHandle = MakeScreen(screenSize.width, screenSize.height, true);
}

// デストラクタ
ScreenShaker::~ScreenShaker()
{
}

// 更新
void ScreenShaker::Update()
{
	// 揺れの処理
	if (m_quakeTimer.GetTime() > 0)
	{
		// 画面を揺らす
		m_quakeSize.x *= quake_attenuation;
		m_quakeSize.y *= quake_attenuation;

		// タイマーを更新
		m_quakeTimer.Update(-1);
	}
	else
	{
		// 揺れが終わったら揺れの大きさを0にする
		m_quakeSize.x = 0.0f;
		m_quakeSize.y = 0.0f;
	}
}

// 描画前
void ScreenShaker::PreDraw()
{
	// フラグを立てる
	m_isPreDraw = true;

	// 描画先を切り替え
	SetDrawScreen(m_screenHandle);

	// 画面をクリア
	ClearDrawScreen();

	// 描画先を切り替えたらカメラがリセットされるので再設定
	m_pCamera->SetCamera();
}

// 描画
void ScreenShaker::Draw()
{
	// PreDraw()を呼んでいなかったらエラー
	if (!m_isPreDraw)
	{
		assert(!"ScreenShaker::PreDraw()を呼んでください");
	}

	// 描画先を裏画面に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	// 描画先を切り替えたらカメラがリセットされるので再設定
	m_pCamera->SetCamera();

	// 初期化
	m_isPreDraw = false;

	// 画面の揺れを反映して描画
	DrawGraph(static_cast<int>(m_quakeSize.x), static_cast<int>(m_quakeSize.y), m_screenHandle, false);
}

// 画面を揺らす
void ScreenShaker::StartShake(const Vector2& quakeSize, const int frame)
{
	// すでに揺れていたら何もしない
	if (m_quakeTimer.GetTime() > 0) return;

	// 揺れの大きさを設定
	m_quakeSize = quakeSize;

	// タイマーを設定
	m_quakeTimer.SetTime(frame);
}
