#pragma once
#include "TransitorBase.h"

/// <summary>
/// 画面切り替え演出
/// アイリスイン・アウト
/// </summary>
class IrisTransitor : public TransitorBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="irisOut">アウトかどうか</param>
	/// <param name="interval">切り替わる時間</param>
	/// <param name="isTiled">タイル状に描画するか</param>
	/// <param name="gHandle">演出につかいたい画像ハンドル</param>
	IrisTransitor(bool irisOut = false, int interval = 60, bool isTiled = false, int gHandle = -1);

	// デストラクタ
	~IrisTransitor();

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

private:
	// マスク転送用グラフィックハンドル
	int m_handleForMaskScreen;

	// マスクハンドル
	int m_maskH;

	// 対角線の長さ
	float m_diagonalLength;

	// アイリスアウトフラグ
	bool m_irisOut;

	// 演出につかいたい画像ハンドル
	int m_handle;

	// 通常とは違う演出フラグ
	bool m_isTiled;
};