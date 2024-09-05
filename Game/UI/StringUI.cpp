#include "StringUI.h"
#include "../String/MessageManager.h"

// �R���X�g���N�^
StringUI::StringUI(const std::string& stringID) :
	m_stringID(stringID),
	m_color(0xffffff)
{
	m_pos = Math::Vector2(0.0f, 0.0f);
}

// �f�X�g���N�^
StringUI::~StringUI()
{
}

// �`��
void StringUI::Draw()
{
	// ������̕`��
	String::MessageManager::GetInstance()->DrawStringCenter(m_stringID, m_pos.x, m_pos.y, m_color);
}

// �ʒu�̐ݒ�
void StringUI::SetPos(const Math::Vector2& pos)
{
	m_pos = pos;
}

// �F�̐ݒ�
void StringUI::SetColor(const unsigned int color)
{
	m_color = color;
}
