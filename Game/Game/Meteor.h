#pragma once
#include "../Math/Vector3.h"
#include "../Editor/DataReaderFromUnity.h"
#include <memory>

// �v���g�^�C�v�錾
class Model;
class Player;

// 覐΂̎��
enum class MeteorType
{
	SMALL,	// ������覐�
	NORMAL	// �ʏ��覐�
};

/// <summary>
/// 覐΃N���X
/// </summary>
class Meteor
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// �����_������
	/// </summary>
	/// <param name="type">覐΂̎��</param>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	Meteor(const MeteorType type, const std::shared_ptr<Player>& pPlayer, const float playerRelativePos);

	/// <summary>
	/// �R���X�g���N�^
	/// �z�u�f�[�^�Ő���
	/// </summary>
	/// <param name="data">�z�u�f�[�^</param>
	Meteor(const UnityGameObject& data, const std::shared_ptr<Player>& pPlayer);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Meteor();

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


	//// �Q�b�^�[ ////

	/// <summary>
	/// ���݃t���O�̎擾
	/// </summary>
	/// <returns>���݃t���O</returns>
	bool IsEnabled() const;		

	/// <summary>
	/// ���f���̃n���h���̎擾
	/// </summary>
	/// <returns>���f���̃n���h��</returns>
	int GetModelHandle() const;

	/// <summary>
	/// 覐΂̎�ނ̎擾
	/// </summary>
	/// <returns>覐΂̎��</returns>
	MeteorType GetType() const;	

private:
	// �|�C���^
	std::unique_ptr<Model> m_pModel;
	std::shared_ptr<Player> m_pPlayer;

	// 覐΂̎��
	MeteorType m_type;

	// �ʒu���
	Math::Vector3 m_pos;

	// ��]�p�x
	Math::Vector3 m_rot;

	// �g�嗦
	Math::Vector3 m_scale;

	// �ړ��x�N�g��
	Math::Vector3 m_moveVec;

	// ��]�x�N�g��
	Math::Vector3 m_rotVec;

	// �����x
	float m_opacity;

	// ���݃t���O
	bool m_isEnabled;
};