#pragma once
#include "../Math/Vector2.h"
#include <string>

/// <summary>
/// リザルト画面
/// </summary>
class ResultWindow
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ResultWindow();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ResultWindow();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了フラグの取得 
	/// </summary>
	/// <returns>終了フラグ</returns>	
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

	// サウンドを鳴らしたか
	bool m_isOpenMenuSound;
};