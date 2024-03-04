#pragma once

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
	/// <param name="color">フラッシュの色</param>
	void Update(const unsigned int color);

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
	// カラー
	unsigned int m_color;

	// アルファ値
	int m_alpha;

	// 消えるまでのフレーム数
	int m_enableFrame;
};