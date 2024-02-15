#include "Fade.h"
#include "DxLib.h"
#include "../Application.h"
#include "../Util/Range.h"

namespace
{
	// 通常のフェードの速度
	constexpr int fade_normal_speed = 8;

	// モザイクパラメーターの最大値
	constexpr int gauss_max_value = 1400;
}

// コンストラクタ
Fade::Fade() :
	m_isFadeOut(false),
	m_fadeColor(0x000000),
	m_fadeBright(255),
	m_fadeSpeed(0),
	m_fadeBrightRange(0, 255)
{
	// モザイク処理用のグラフィックの作成
	auto& app = Application::GetInstance();
	m_gaussScreen = MakeScreen(app.GetWindowSize().width, app.GetWindowSize().height);
}

// デストラクタ
Fade::~Fade()
{
	// モザイク処理用のグラフィックの削除
	DeleteGraph(m_gaussScreen);
}

// 更新
void Fade::Update()
{
	// フェードの明るさの更新
	m_fadeBright += m_fadeSpeed;

	// フェードの明るさが設定した範囲を超えたらフェードを止める
	if (!m_fadeBrightRange.IsInside(m_fadeBright))
	{
		m_fadeSpeed = 0;
	}

	// フェードの明るさを設定した範囲内にクランプする
	m_fadeBright = m_fadeBrightRange.Clamp(m_fadeBright);
}

// 通常フェードの描画
void Fade::DrawFade(bool isDraw)
{
	if (!isDraw) return;

	auto& app = Application::GetInstance();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeBright);
	DrawBox(0, 0, app.GetWindowSize().width, app.GetWindowSize().height, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// モザイクフェードの描画
void Fade::DrawGaussFade(bool isDraw)
{
	if (!isDraw) return;

	// 0~255の範囲を0~モザイクパラメーターの最大値に変換
	int gaussParameter = m_fadeBright * gauss_max_value / 255;

	// モザイク画像の作成
	auto& app = Application::GetInstance();
	GetDrawScreenGraph(0, 0, app.GetWindowSize().width, app.GetWindowSize().height, m_gaussScreen);
	GraphFilter(m_gaussScreen, DX_GRAPH_FILTER_GAUSS, 8, gaussParameter);
	DrawGraph(0, 0, m_gaussScreen, true);
}

// フェードインの開始
void Fade::StartFadeIn(int fadeBrightUpperLimitValue, int fadeSpeed)
{
	// 初期化
	m_isFadeOut = false;

	// フェードインをどのくらいまで行うのか値を設定
	// 255(フェードしない)～ 0(最後までフェードを行う)
	m_fadeBrightRange = Range<int>(fadeBrightUpperLimitValue, 255);

	// フェード速度の設定
	m_fadeSpeed = -abs(fadeSpeed);
}

// フェードアウトの開始
void Fade::StartFadeOut(int fadeBrightUpperLimitValue, int fadeSpeed)
{
	// フェードアウトが行われている場合は何もしない
	if(m_isFadeOut) return;

	// フェードアウトが行われたかどうかのフラグを立てる
	m_isFadeOut = true;

	// フェードアウトをどのくらいまで行うのか値を設定
	// 0(フェードしない)～255(最後までフェードを行う)
	m_fadeBrightRange = Range<int>(0, fadeBrightUpperLimitValue);

	// フェード速度の設定
	m_fadeSpeed = abs(fadeSpeed);
}

// フェードイン中かどうか
bool Fade::IsFadingIn() const
{
	return (m_fadeSpeed < 0);
}

// フェードアウト中かどうか
bool Fade::IsFadingOut() const
{
	return (m_fadeSpeed > 0);
}

// フェード中かどうか
bool Fade::IsFading() const
{
	return IsFadingIn() || IsFadingOut();
}

// フェードアウトが行われて終了したかどうか
bool Fade::IsFadeOutEnd() const
{
	return (m_isFadeOut && !IsFadingOut());
}

// フェードの明るさの取得
int Fade::GetFadeBright() const
{
	return m_fadeBright;
}

// フェードの明るさの設定
void Fade::SetFadeBright(int fadeBright)
{
	m_fadeBright = fadeBright;
}