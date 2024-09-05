#pragma once
#include "../Math/Vector3.h"
#include "../Editor/DataReaderFromUnity.h"
#include <memory>

// �v���g�^�C�v�錾
class Model;

/// <summary>
/// �f���N���X
/// </summary>
class Planet
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="modelHandle">���f���n���h��</param>
	/// <param name="data">�z�u�f�[�^</param>
	Planet(const int modelHandle, const UnityGameObject& data);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Planet();

	/// <summary>
	/// �Q�[���v���C���̍X�V
	/// </summary>
	/// <param name="playerVec">�v���C���[�̈ړ��x�N�g��</param>
	/// <param name="rotVec">���f���̉�]�x�N�g��</param>
	void Update(const Math::Vector3& playerVec, const Math::Vector3& rotVec);

	/// <summary>
	/// �X�e�[�W�I�����̍X�V
	/// </summary>
	/// <param name="rotVec">���f���̉�]�x�N�g��</param>
	void UpdateStageSelect(const Math::Vector3& rotVec);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �ʒu���̎擾
	/// </summary>
	/// <returns>�ʒu���</returns>
	const Math::Vector3& GetPos() const;

private:
	// �|�C���^
	std::unique_ptr<Model> m_pModel;

	// �ʒu���
	Math::Vector3 m_pos;

	// ��]���
	Math::Vector3 m_rot;

	// �g�嗦
	Math::Vector3 m_scale;
};