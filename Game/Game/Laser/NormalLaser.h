#pragma once
#include "LaserBase.h"
#include "../../StateMachine.h"
#include "../../Math/Matrix.h"
#include "../../Math/Vector2.h"
#include <vector>

// �v���g�^�C�v�錾
class EnemyBase;
class Player;

/// <summary>
/// �ʏ탌�[�U�[�N���X
/// </summary>
class NormalLaser final : public LaserBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pEnemy">���ˌ��̓G�̃|�C���^</param>
	/// <param name="pPlayer">�v���C���[�̃|�C���^</param>
	/// <param name="laserChargeFrame">���[�U�[�̃`���[�W�t���[��</param>
	/// <param name="laserFireFrame">���[�U�[�̔��ˎ��ԃt���[��</param>
	/// <param name="laserSpeed">���[�U�[�̈ړ����x</param>
	/// <param name="isPlayerFollowing">�v���C���[��Ǐ]���邩</param>
	NormalLaser(const std::shared_ptr<EnemyBase>& pEnemy, 
				const std::shared_ptr<Player>& pPlayer, 
				const int laserChargeFrame, 
				const int laserFireFrame, 
				const float laserSpeed, 
				const bool isPlayerFollowing);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~NormalLaser();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override final;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override final;

	/// <summary>
	/// ���[�U�[���~�߂�
	/// </summary>
	/// <param name="pos">�~�߂���W</param>
	void Stop(const Vector3& pos) override final;

	/// <summary>
	/// ���[�U�[�̔���
	/// </summary>
	/// <param name="pos">���˂�����W</param>
	void Reflect(const Vector3& pos) override final;

	/// <summary>
	/// ���˂����ɖ߂�
	/// </summary>
	void UndoReflect() override final;


	//// �Q�b�^�[ ///////////////////////////////////////

	/// <summary>
	/// ���[�U�[�̌������擾
	/// </summary>
	/// <returns>���[�U�[�̌���</returns>
	Vector3 GetDirection() const override final;

private:
	//// ������ ///////////////////////////////////////

	/// <summary>
	/// ���[�U�[�̃`���[�W�̏�����
	/// </summary>
	void EnterCharge();					


	//// �X�V //////////////////////////////////////////

	/// <summary>
	/// ���[�U�[�̃`���[�W�̍X�V
	/// </summary>
	void UpdateCharge();		

	/// <summary>
	/// ���[�U�[�̒ʏ픭�˂̍X�V
	/// </summary>
	void UpdateNormalFire();	

	/// <summary>
	/// ���[�U�[�̃v���C���[�Ǐ]���˂̍X�V
	/// </summary>
	void UpdateFirePlayerFollowing();	

private:
	// ���
	enum class State
	{
		CHARGE,					// �`���[�W
		FIRE_PLYER_FOLLOWING,	// �v���C���[��Ǐ]���Ĕ���
		NORMAL_FIRE,			// �ʏ픭��
		NUM
	};

private:
	// �|�C���^
	std::shared_ptr<EnemyBase> m_pEnemy;	// ���ˌ��̃|�C���^
	std::shared_ptr<Player>    m_pPlayer;	// �v���C���[�̃|�C���^

	// ��������
	Vector3 m_directionPos;	// �������W
	Vector3 m_directionVec; // �x�N�g��

	// ��ԃ}�V��
	StateMachine<State> m_stateMachine;	

	// ��]�s��
	Math::Matrix m_rotMtx;	

	// ���[�U�[�̃G�t�F�N�g�̃n���h��
	int m_laserEffectHandle;

	// �t���[��
	int m_laserFireFrame;		// ���[�U�[�̔��˃t���[��
	int m_chargeEffectFrame;	// �G�t�F�N�g�`���[�W�t���[��
	int m_reflectFrame;			// ���˃t���[��	

	// �v���C���[��Ǐ]���邩
	bool m_isPlayerFollowing;

	// �ʏ픭�ˎ�
	int     m_normalFireMovePointIndex;				// �ړ��|�C���g�C���f�b�N�X
	Vector3 m_normalFireGoalPos;					// �ړ��|�C���g�̍��W
	std::vector<Math::Vector2> m_normalFireMovePointList;	// �ړ��|�C���g�̍��W�̃��X�g
};