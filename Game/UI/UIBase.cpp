#include "UIBase.h"

// コンストラクタ
UIBase::UIBase() :
	m_pos({ 0, 0}),
	m_isEnabled(true)
{
}

// デストラクタ
UIBase::~UIBase()
{
}

// UIの格納
void UIBase::Store(const Vector2& vec, const float storeSpeed)
{
	m_pos += vec.Normalized() * storeSpeed;
}

// UIが有効かどうか
bool UIBase::IsEnabled() const
{
	return m_isEnabled;
}
