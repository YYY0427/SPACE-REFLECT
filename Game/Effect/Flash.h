#pragma once
#include "../Math/Vector2.h"

/// <summary>
/// 画面がフラッシュするクラス
/// </summary>
class Flash
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enableFrame">消えるまでのフレーム数</param>
	Flash(const int enableFrame);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Flash();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pos">フラッシュが広がっていく中心地</param>
	/// <param name="color">フラッシュの色</param>
	void Update(const Vector2& pos, const unsigned int color);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了したか
	/// </summary>
	///	<returns>true : 終了、false : 途中</returns>
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