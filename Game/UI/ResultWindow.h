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
	// ウィンドウの位置
	Vector2 m_windowPos;

	// 終了フラグ
	bool m_isEnd;
};