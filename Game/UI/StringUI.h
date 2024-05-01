#pragma once
#include "UIBase.h"
#include <string>

/// <summary>
/// 文字列UIクラス
/// </summary>
class StringUI final : public UIBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stringID">文字列ID</param>
	StringUI(const std::string& stringID);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StringUI();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override final {};

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override final;

	/// <summary>
	/// 位置の設定 
	/// </summary>
	/// <param name="pos">位置</param>
	void SetPos(const Vector2& pos);

	/// <summary>
	/// 色の設定
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(const unsigned int color);

private:
	// 文字列ID
	std::string m_stringID;

	// 色
	unsigned int m_color;
};