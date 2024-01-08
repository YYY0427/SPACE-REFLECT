#include "UIBase.h"

// コンストラクタ
UIBase::UIBase() :
	m_pos({}),
	m_isEnabled(true)
{
}

// デストラクタ
UIBase::~UIBase()
{
}

// UIの格納
void UIBase::Store(Vector2 vec)
{
	m_pos += vec;
}

// UIが有効かどうか
bool UIBase::IsEnabled() const
{
	return m_isEnabled;
}
