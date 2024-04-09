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
	Vector2 m_scorePos;		// スコア
	Vector2 m_yourScorePos;	// YOUR SCORE

	// イージング
	Vector2 m_easing;

	// タイトル終了フラグ
	bool m_isTitleEnd;

	// 終了フラグ
	bool m_isEnd;

	bool m_isOpenMenuSound;
};