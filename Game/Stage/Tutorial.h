#pragma once
#include "StageBase.h"
#include "../Util/StateMachine.h"
#include "../Util/Timer.h"
#include <string>
#include <memory>

// �v���g�^�C�v�錾
class TutorialUI;

/// <summary>
/// �`���[�g���A���X�e�[�W�N���X
/// </summary>
class Tutorial final : public StageBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	Tutorial(SceneManager& manager);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Tutorial();

	/// <summary>
	/// �X�^�[�g���o�X�e�[�g�̊J�n
	/// </summary>
	void EnterStartAnimation();

	/// <summary>
	/// �Q�[���N���A�X�e�[�g�̊J�n
	/// </summary>
	void EnterGameClear();

	/// <summary>
	/// �Q�[���I�[�o�[�X�e�[�g�̊J�n
	/// </summary>
	void EnterGameOver();

	/// <summary>
	/// ���U���g�X�e�[�g�̊J�n
	/// </summary>
	void EnterResult();

	/// <summary>
	/// �ʏ�̍X�V
	/// </summary>
	void Update() override final;

	/// <summary>
	/// �X�^�[�g���o�X�e�[�g�̍X�V
	/// </summary>
	void UpdateStartAnimation();

	/// <summary>
	/// �ړ��`���[�g���A���X�e�[�g�̍X�V
	/// </summary>
	void UpdateMoveTutorial();

	/// <summary>
	/// �V�[���h�`���[�g���A���X�e�[�g�̍X�V
	/// </summary>
	void UpdateShieldTutorial();

	/// <summary>
	/// ���˃`���[�g���A���X�e�[�g�̍X�V
	/// </summary>
	void UpdateReflectTutorial();

	/// <summary>
	/// �L���[�u�`���[�g���A���X�e�[�g�̍X�V
	/// </summary>
	void UpdateCubeTutorial();

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
		MOVE_TUTORIAL,		// �ړ��`���[�g���A��
		SHIELD_TUTORIAL,	// �V�[���h�`���[�g���A��
		REFLECT_TUTORIAL,	// ���˃`���[�g���A��
		CUBE_TUTORIAL,		// �L���[�u�`���[�g���A��
		PLAY,				// �v���C��
		GAME_CLEAR,			// �Q�[���N���A
		GAME_OVER,			// �Q�[���I�[�o�[
		RESULT,				// ���U���g
		NUM					// ��Ԃ̐�
	};

private:
	// �X�e�[�g�}�V��
	StateMachine<State> m_stateMachine;

	// �|�C���^
	std::shared_ptr<TutorialUI> m_pTutorialUI;
};