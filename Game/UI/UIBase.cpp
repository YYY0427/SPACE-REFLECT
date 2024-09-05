#include "UIBase.h"

// �R���X�g���N�^
UIBase::UIBase() :
	m_pos({ 0, 0}),
	m_isEnabled(true)
{
}

// �f�X�g���N�^
UIBase::~UIBase()
{
}

// UI�̊i�[
void UIBase::Store(const Math::Vector2& vec, const float storeSpeed)
{
	m_pos += vec.Normalized() * storeSpeed;
}

// UI���L�����ǂ���
bool UIBase::IsEnabled() const
{
	return m_isEnabled;
}
