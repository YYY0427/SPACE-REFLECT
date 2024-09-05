#pragma once
#include "../EnemyBase.h"
#include <vector>

// �v���g�^�C�v�錾
class LaserManager;
class Gauge;
class StringUI;
namespace Effect { class Flash; }
namespace Effect { class ScreenShaker; }

/// <summary>
/// �{�X�G
/// �}�g���b�N�X
/// </summary>
class BossMatrix : public EnemyBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pPlayer">�v���C���[�̃|�C���^</param>
	/// <param name="pLaserManager">���[�U�[�}�l�[�W���[�̃|�C���^</param>
	/// <param name="pScreenShaker">��ʗh��̃|�C���^</param>
	BossMatrix(const std::shared_ptr<Player>& pPlayer, 
			   const std::shared_ptr<LaserManager>& pLaserManager,
			   const std::shared_ptr<Effect::ScreenShaker>& pScreenShaker);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~BossMatrix();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override final;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override final;

	/// <summary>
	/// �_���[�W����
	/// </summary>
	/// <param name="damage">�_���[�W��</param>
	/// <param name="pos">�_���[�W���󂯂��ʒu</param>
	void OnDamage(const int damage, const Vector3& pos) override final;

private:

	//// �X�e�[�g�}�V���̏����� ////

	/// <summary>
	/// �ړ����Ȃ���z�[�~���O���[�U�[�U���̏�����
	/// </summary>
	void EnterMoveHormingLaserAttack(); 

	/// <summary>
	/// �L���[�u���[�U�[�U���̏�����
	/// </summary>
	void EnterCubeLaserAttack();	

	/// <summary>
	/// �o�ꎞ�̏�����
	/// </summary>
	void EnterDie();	


	//// �X�e�[�g�̍X�V ////

	/// <summary>
	/// �o�ꎞ�̍X�V
	/// </summary>
	void UpdateEntry();

	/// <summary>
	/// �ҋ@���̍X�V
	/// </summary>
	void UpdateIdle();

	/// <summary>
	/// ���S���̍X�V
	/// </summary>
	void UpdateDie();		

	/// <summary>
	/// �Q�[���I�[�o�[���̍X�V
	/// </summary>
	void UpdateGameOver();

	/// <summary>
	/// �ړ����Ȃ���z�[�~���O���[�U�[�U���̍X�V
	/// </summary>
	void UpdateMoveHomingLaserAttack();	

	/// <summary>
	/// �L���[�u���[�U�[�U���̍X�V
	/// </summary>
	void UpdateCubeLaserAttack();		


	//// �ړ� ////

	/// <summary>
	/// �ړ��̏�����
	/// </summary>
	void InitMove();		

	/// <summary>
	/// �ڕW���W�̐ݒ�
	/// </summary>
	void SetGoalPos();

	/// <summary>
	/// �����ʒu�Ɉړ�
	/// </summary>
	void MoveInitPos();	

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();


	//// �U�� ////

	/// <summary>
	/// �U�������̃V���b�t��
	/// </summary>
	void ShuffleAttackState();

	/// <summary>
	/// �U���X�e�[�g�̐ݒ�
	/// </summary>
	void SetAttackState();


	//// �Q�b�^�[ ////

	/// <summary>
	/// ���S���̉��o����
	/// </summary>
	/// <returns>true : ���o���Afalse : ���o���ł͂Ȃ�</returns>
	bool IsDeadAnim() override;	

private:
	/// <summary>
	/// �v���C���[�̕���������
	/// </summary>
	void LookPlayerDir();

private:
	// �X�e�[�g
	enum class State
	{
		// ��{
		ENTRY,		// �o��
		IDLE,		// �ҋ@
		BARIER,		// �o���A
		DIE,		// ���S
		GAME_OVER,	// �Q�[���I�[�o�[

		// �U��
		MOVE_HOMING_LASER_ATTACK,	// �ړ����Ȃ���z�[�~���O���[�U�[�U��
		CUBE_LASER_ATTACK,			// �L���[�u���[�U�[�U��
	};

private:
	// �X�e�[�g�}�V��
	StateMachine<State> m_stateMachine;

	// �|�C���^
	std::shared_ptr<Gauge>        m_pHpGauge;		// HP�Q�[�W
	std::shared_ptr<StringUI>     m_pBossName;		// �{�X�̖��O
	std::shared_ptr<LaserManager> m_pLaserManager;	// ���[�U�[�}�l�[�W���[
	std::shared_ptr<Effect::ScreenShaker> m_pScreenShaker;	// ��ʗh��
	std::shared_ptr<Effect::Flash>        m_pFlash;			// �t���b�V��

	// �ړ�
	std::vector<Vector3> m_movePointTable;	// �ړ��|�C���g�e�[�u��
	int m_movePointIndex;					// �ړ��|�C���g�̃C���f�b�N�X
	Vector3 m_goalPos;						// �ڕW���W
	bool m_isGoal;							// �ڕW���W�ɓ��B������
	bool m_isMoveEnd;						// �ړ����I��������

	// �U���X�e�[�g
	std::vector<State> m_attackStateTable;	// �U���X�e�[�g�e�[�u��
	int m_attackStateIndex;					// �U���X�e�[�g�̃C���f�b�N�X

	// �t���[��
	int m_idleFrame;						// �ҋ@�t���[��
	int m_laserFrame;						// ���[�U�[���˃t���[��
	int m_cubeLaserIntervalFrame;			// �L���[�u���[�U�[�̔��ˊԊu�t���[��
	int m_dieIdleFrame;						// ���S���̑ҋ@�t���[��	
	int m_dieDrawStopFrame;					// ���S���̕`����~�߂�t���[��

	// ���[�U�[��Key
	int m_laserKey;

	// �G�t�F�N�g�n���h��
	int m_damageEffectHandle;
	int m_dieEffectHandle;

	// ���X�Ƀv���C���[�̕����Ɍ����鎞�Ɏg�p���郉�[�v�̒l
	int m_playerDirLerp;

	int  m_timer;
	bool m_isExplosionSound;
};