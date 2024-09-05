#pragma once
#include "UIBase.h"
#include <string>

/// <summary>
/// ������UI�N���X
/// </summary>
class StringUI final : public UIBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="stringID">������ID</param>
	StringUI(const std::string& stringID);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~StringUI();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override final {};

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override final;

	/// <summary>
	/// �ʒu�̐ݒ� 
	/// </summary>
	/// <param name="pos">�ʒu</param>
	void SetPos(const Math::Vector2& pos);

	/// <summary>
	/// �F�̐ݒ�
	/// </summary>
	/// <param name="color">�F</param>
	void SetColor(const unsigned int color);

private:
	// ������ID
	std::string m_stringID;

	// �F
	unsigned int m_color;
};