#pragma once
#include <memory>
#include "../Math/Vector3.h"

// �v���g�^�C�v�錾
class Model;

/// <summary>
/// �X�J�C�h�[���N���X
/// </summary>
class SkyDome
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�ʒu���</param>
	SkyDome(const Math::Vector3& pos);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SkyDome();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="pos">�ʒu���</param>
	void Update(const Math::Vector3& pos);

	/// <summary>
	/// �`��s
	/// </summary>
	void Draw();

private:
	// �|�C���^
	std::unique_ptr<Model> m_pModel;

	// �ʒu���
	Math::Vector3 m_pos;
};