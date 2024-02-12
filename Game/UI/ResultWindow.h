#pragma once
#include "../Math/Vector2.h"
#include <string>

/// <summary>
/// リザルト画面
/// </summary>
class ResultWindow
{
public:
	// コンストラクタ
	ResultWindow();

	// デストラクタ
	~ResultWindow();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 終了フラグの取得
	bool IsEnd() const;

private:
	// ウィンドウ
	Vector2 m_windowPos;	// 位置
	Vector2 m_windowSize;	// サイズ
	bool m_isEndWindow;		// 終了フラグ

	// テキスト
	Vector2 m_missionPos;	// ミッション
	Vector2 m_completePos;	// コンプリート

	// イージング
	Vector2 m_easing;

	// 終了フラグ
	bool m_isEnd;
};