#include "StringUI.h"
#include "../String/MessageManager.h"

// コンストラクタ
StringUI::StringUI(const std::string& stringID) :
	m_stringID(stringID),
	m_color(0xffffff)
{
	m_pos = Vector2(0.0f, 0.0f);
}

// デストラクタ
StringUI::~StringUI()
{
}

// 描画
void StringUI::Draw()
{
	// 文字列の描画
	MessageManager::GetInstance().DrawStringCenter(m_stringID, m_pos.x, m_pos.y, m_color);
}

// 位置の設定
void StringUI::SetPos(const Vector2& pos)
{
	m_pos = pos;
}

// 色の設定
void StringUI::SetColor(const unsigned int color)
{
	m_color = color;
}
