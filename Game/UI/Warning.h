#pragma once
#include "UIBase.h"
#include "../Util/Range.h"
#include <array>

/// <summary>
/// ボス登場時の警告UI
/// </summary>
class Warning final : public UIBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="drawFrame">描画フレーム</param>
	Warning(const int drawFrame);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Warning();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override final;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override final;

	/// <summary>
	/// 終了判定
	/// </summary>
	/// <returns>終了しているか</returns>
	bool IsEnd() const;

private:
	// 識別用
	enum class ID
	{
		CENTER,		// 中央
		UP,			// 上
		DOWN,		// 下
	};

private:
	// UIデータ
	struct Data
	{
		int imgHandle;
		Vector2 pos;
		int scrollDirection;
		int imgWidth;
		int imgHeight;
	};

private:
	// UIデータの配列
	std::array<Data, 3> m_uiDataTable;	

	// 終了フラグ
	bool m_isEnd;	

	// スクロール
	int m_scroll;

	// 描画フレーム
	int m_drawFrame;

	// 警告文のアルファ値
	int m_stringAlphaParam;

	// 警告画像のアルファ値
	int m_imgAlphaParam;

	// 警告画像のアルファ値の増減値
	int m_addImgAlphaParam;

	// 警告画像のアルファ値の範囲
	Range<int> m_ImgAlphaParamRange;
};