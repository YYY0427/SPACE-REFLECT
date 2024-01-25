#pragma once
#include "UIBase.h"
#include <string>

/// <summary>
/// 文字列UIクラス
/// </summary>
class StringUI final : public UIBase
{
public:
	// コンストラクタ
	StringUI(std::string stringID);

	// デストラクタ
	~StringUI();

	// 更新
	void Update() override final {};

	// 描画
	void Draw() override final;

	// 位置の設定
	void SetPos(Vector2 pos);

	// 色の設定
	void SetColor(unsigned int color);

private:
	// 文字列ID
	std::string m_stringID;

	// 色
	unsigned int m_color;
};