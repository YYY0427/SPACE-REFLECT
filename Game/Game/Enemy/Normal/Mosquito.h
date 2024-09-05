#pragma once
#include "../EnemyBase.h"
#include "../../../Util/Timer.h"
#include "../../../Game/Laser/LaserManager.h"
#include <memory>

/// <summary>
/// �G���G�N���X
/// </summary>
class Mosquito final : public EnemyBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="data">�G�̃f�[�^</param>
	/// <param name="pPlayer">�v���C���[�̃|�C���^</param>
	/// <param name="pLaserManager">���[�U�[�}�l�[�W���[�̃|�C���^</param>
	Mosquito(const EnemyData& data, 
		     const std::shared_ptr<Player>& pPlayer, 
		     const std::shared_ptr<LaserManager>& pLaserManager);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Mosquito();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override final;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override final;

	/// <summary>
	/// �֊s���̕`��
	/// </summary>
	void DrawOutLine();

	//// �Q�b�^�[ ////

	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	const Math::Vector3& GetPos() const;

private:

	//// ������ ////

	/// <summary>
	/// �ړ��̏�����
	/// </summary>
	void EntarMove();	


	//// �X�V ////

	/// <summary>
	/// �ҋ@��Ԃ̍X�V
	/// </summary>
	void UpdateIdle();

	/// <summary>
	/// �ړ���Ԃ̍X�V
	/// </summary>
	void UpdateMove();

	/// <summary>
	/// �U����Ԃ̍X�V
	/// </summary>
	void UpdateAttack();



	/// <summary>
	/// �S�[�����W��ݒ�
	/// </summary>
	void SetGoalPos();	

private:
	// ���
	enum class State
	{
		IDLE,		// �ҋ@
		MOVE,		// �ړ�
		ATTACK,		// �U��
	};

private:
	// ��ԃ}�V��
	StateMachine<State> m_state;	

	// �s���f�[�^
	EnemyActionData m_actionData;	

	// ���[�U�[��Key
	int m_laserKey;

	// �ҋ@���ԃt���[��
	int m_idleFrame;
};