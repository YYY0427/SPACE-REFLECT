#pragma once
#include "StageBase.h"
#include "../Util/StateMachine.h"
#include <memory>

/// <summary>
/// �X�e�[�W1
/// </summary>
class Stage1 final : public StageBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	Stage1(SceneManager& manager);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Stage1();

	/// <summary>
	/// �v���C�X�e�[�g�̊J�n
	/// </summary>
	void EnterPlay();

	/// <summary>
	/// ���U���g�X�e�[�g�̊J�n
	/// </summary>
	void EnterResult();

	/// <summary>
	/// �Q�[���N���A�X�e�[�g�̊J�n
	/// </summary>
	void EnterGameClear();

	/// <summary>
	/// �Q�[���I�[�o�[�X�e�[�g�̊J�n
	/// </summary>
	void EnterGameOver();

	/// <summary>
	/// �ʏ�̍X�V
	/// </summary>
	void Update() override final;

	/// <summary>
	/// �X�^�[�g���o�X�e�[�g�̍X�V
	/// </summary>
	void UpdateStartAnimation();

	/// <summary>
	/// �v���C�X�e�[�g�̍X�V
	/// </summary>
	void UpdatePlay();

	/// <summary>
	/// �Q�[���N���A�X�e�[�g�̍X�V
	/// </summary>
	void UpdateGameClear();

	/// <summary>
	/// �Q�[���I�[�o�[�X�e�[�g�̍X�V
	/// </summary>
	void UpdateGameOver();

	/// <summary>
	/// ���U���g�X�e�[�g�̍X�V
	/// </summary>
	void UpdateResult();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override final;

private:
	// ���
	enum class State
	{
		START_ANIMATION,	// �X�^�[�g���o
		PLAY,				// �v���C��
		GAME_OVER,			// �Q�[���I�[�o�[
		GAME_CLEAR,			// �Q�[���N���A
		RESULT,				// ���U���g
		NUM					// ��Ԃ̐�
	};

private:
	// �X�e�[�g�}�V��
	StateMachine<State> m_stateMachine;
};