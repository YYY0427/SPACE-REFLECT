#pragma once
#include "../Math/Vector2.h"

/// <summary>
/// 画面がフラッシュするクラス
/// </summary>
class Flash
{
public:
	// コンストラクタ
	Flash(int enableFrame);

	// デストラクタ
	~Flash();

	// 更新
	void Update(unsigned int color);

	// 描画
	void Draw();

	// 終了したかどうか
	bool IsEnd() const;

private:
	// 位置
	Vector2 m_pos;

	// カラー
	unsigned int m_color;

	// アルファ値
	int m_alpha;

	// 半径
	float m_radius;

	// 消えるまでのフレーム数
	int m_enableFrame;
};