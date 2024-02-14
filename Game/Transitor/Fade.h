#pragma once
#include "../Util/Range.h"

/// <summary>
/// フェード
/// </summary>
class Fade
{
public:
	// コンストラクタ
	Fade();

	// デストラクタ
	~Fade();

	// 更新
	void Update();

	// 通常フェードの描画
	void DrawFade(bool isDraw);

	// モザイクフェードの描画
	void DrawGaussFade(bool isDraw);

	/// <summary>
	/// フェードイン開始
	/// </summary>
	/// <param name="fadeBrightUpperLimitValue">フェードインの上限値 0~255</param>
	/// <param name="fadeSpeed">フェードの速度</param>
	void StartFadeIn(int fadeBrightUpperLimitValue, int fadeSpeed = 8);

	/// <summary>
	/// フェードアウト開始
	/// </summary>
	/// <param name="fadeBrightUpperLimitValue">フェードアウトの上限値 0~255</param>
	/// <param name="fadeSpeed">フェードの速度</param>
	void StartFadeOut(int fadeBrightUpperLimitValue, int fadeSpeed = 8);

	/// <summary>
	/// フェードイン中かどうか
	/// </summary>
	/// <returns>true : フェードイン中, false : フェードインしていない</returns>
	bool IsFadingIn() const;

	/// <summary>
	/// フェードアウト中かどうか
	/// </summary>
	/// <returns>true : フェードアウト中, false : フェードアウトしていない</returns>
	bool IsFadingOut() const;

	/// <summary>
	/// フェード中かどうか
	/// </summary>
	/// <returns>true : フェード中, false : フェードしてない</returns>
	bool IsFading() const;

	// フェードアウトが行われた後で終了したかどうか
	bool IsFadeOutEnd() const;

	// フェードの明るさの取得
	int GetFadeBright() const;

	/// <summary>
	/// フェードの明るさの設定
	/// </summary>
	/// <param name="fadeBright">設定したい明るさの値</param>
	void SetFadeBright(int fadeBright);

private:
	// フェードの明るさの範囲
	Range<int> m_fadeBrightRange;

	// フェードの色
	unsigned int m_fadeColor;

	// フェードの明るさ
	// 0(フェードしていない)～255(真っ黒)
	int m_fadeBright;

	// フェードする速度
	// 正数の場合フェードアウト、負数の場合フェードイン
	int m_fadeSpeed;

	// フェードアウトをおこなったかどうか
	bool m_isFadeOut;

	// モザイクフェード用の描画グラフィック
	int m_gaussScreen;
};