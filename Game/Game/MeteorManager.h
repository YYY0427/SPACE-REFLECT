#pragma once
#include <memory>
#include <list>
#include "Meteor.h"
#include "../Math/Vector3.h"

// �v���g�^�C�v�錾
class Player;

/// <summary>
/// 覐΂̊Ǘ��N���X
/// </summary>
class MeteorManager
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="objectDataFileName">�I�u�W�F�N�g�f�[�^�̃t�@�C����</param>
	/// <param name="pPlayer">�v���C���[�̃|�C���^</param>
	MeteorManager(const std::string& objectDataFileName, const std::shared_ptr<Player>& pPlayer);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~MeteorManager();

	/// <summary>
	/// �Q�[���X�^�[�g���̍X�V
	/// </summary>
	void UpdateStart();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="cameraPos">�J�����̍��W</param>
	void Update(const Math::Vector3& cameraPos);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ������覐΂̐���
	/// </summary>
	void CreateSmallMeteor(const float playerRelativePos);

	/// <summary>
	/// �S�Ă�覐΂̍폜
	/// </summary>
	void DeleteAllMeteor();

	/// <summary>
	/// 覐΃��X�g�̎擾
	/// </summary>
	/// <returns>覐΃��X�g</returns>
	const std::list<std::shared_ptr<Meteor>>& GetMeteor() const;

private:
	// 覐΃��X�g
	std::list<std::shared_ptr<Meteor>> m_pMeteorList;

	// �|�C���^
	std::shared_ptr<Player> m_pPlayer;

	// �����Ԋu�t���[��
	int m_smallCreateIntervalFrame;
};