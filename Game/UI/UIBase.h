#pragma once
#include "../Math/Vector2.h"

/// <summary>
/// UI�̊��N���X
/// </summary>
class UIBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	UIBase();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~UIBase();

	/// <summary>
	/// �X�V
	/// �������z�֐�
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �`��
	/// �������z�֐�
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// UI�̊i�[ 
	/// </summary>
	/// <param name="vec">�i�[����x�N�g��</param>
	/// <param name="storeSpeed">�i�[���x</param>
	void Store(const Math::Vector2& vec, const float storeSpeed);

	/// <summary>
	/// UI���L�����ǂ��� 
	/// </summary>
	/// <returns></returns>
	bool IsEnabled() const;

protected:
	// UI�̍��W
	Math::Vector2 m_pos;

	// UI���L�����ǂ���
	bool m_isEnabled;
};