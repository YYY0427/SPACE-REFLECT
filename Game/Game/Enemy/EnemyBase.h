#pragma once
#include "../../Math/Vector3.h"
#include "EnemyManager.h"
#include "../../StateMachine.h"
#include <memory>

// �v���g�^�C�v�錾
class Model;
class Player;
class LaserManager;

/// <summary>
/// �G�̊��N���X
/// </summary>
class EnemyBase : public std::enable_shared_from_this<EnemyBase>
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	EnemyBase();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~EnemyBase();

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// �_���[�W����
	/// </summary>
	/// <param name="damage">�_���[�W��</param>
	/// <param name="pos">�_���[�W���󂯂��ꏊ</param>
	virtual void OnDamage(const int damage, const Math::Vector3& pos);

	/// <summary>
	/// ���S���o
	/// </summary>
	virtual void PerformDeathEffect() {};


	//// �Q�b�^�[ ////

	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	const Math::Vector3& GetPos() const;

	/// <summary>
	/// ���[�U�[�̔��ˈʒu���擾
	/// </summary>
	/// <returns>���[�U�[�̔��ˈʒu</returns>
	const Math::Vector3& GetLaserFirePos() const;

	/// <summary>
	/// ���݃t���O���擾
	/// </summary>
	/// <returns>true : ���݁Afalse : ���݂��Ă��Ȃ�</returns>
	bool IsEnabled() const;			

	/// <summary>
	/// �����蔻��̔��a���擾
	/// </summary>
	/// <returns>�����蔻��̔��a</returns>
	float GetCollisionRadius() const;	

	/// <summary>
	/// �{�X�̎��S���o�����ǂ���
	/// </summary>
	/// <returns></returns>
	virtual bool IsDeadAnim();

protected:
	// �|�C���^
	std::shared_ptr<Model> m_pModel;
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<LaserManager> m_pLaserManager;

	// �s���f�[�^���X�g
	std::vector<EnemyActionData> m_actionDataList;

	// �ʒu���
	Math::Vector3 m_pos;

	// ��]���
	Math::Vector3 m_rot;

	// �ړ��x�N�g��
	Math::Vector3 m_moveVec;

	// �ړI�n
	Math::Vector3 m_goalPos;

	// ���[�U�[�̔��ˈʒu
	Math::Vector3 m_laserFirePos;

	// �傫��
	Math::Vector3 m_scale;

	// �����蔻��̔��a
	float m_collisionRadius;	

	// �ړ����x
	float m_moveSpeed;

	// �s�����x
	float m_opacity;

	// �U����
	int m_attackPower;

	// HP
	int m_hp;		

	// ���݃t���O
	bool m_isEnabled;

	// ���݂̍s���f�[�^�̃C���f�b�N�X
	int m_movePointIndex;

	// ���S�G�t�F�N�g�n���h��
	int m_deadEffectHandle;

	// �ړI�n�ɓ��B�������t���O
	bool m_isGoal;
};