#pragma once
#include "SceneBase.h"
#include "../Util/StateMachine.h"
#include <vector>

/// <summary>
/// �ݒ�V�[��
/// </summary>
class OptionScene final : public SceneBase
{
public:
	// �X�e�[�g
	enum class State
	{
		STAGE_SELECT,	// �X�e�[�W�Z���N�g
		PAUSE,			// �I�v�V����
	};

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	OptionScene(SceneManager& manager, const State state);
	
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~OptionScene();
	
	/// <summary>
	/// ������
	/// </summary>
	void Init() override final;

	/// <summary>
	/// �I������
	/// </summary>
	void End() override final;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override final;

	/// <summary>
	/// �X�e�[�W�Z���N�g���̃I�v�V�����̍X�V
	/// </summary>
	void UpdateStageSelect();

	/// <summary>
	/// �|�[�Y���̃I�v�V�����̍X�V
	/// </summary>
	void UpdatePause();
	
	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override final;

	/// <summary>
	/// �X�e�[�W�Z���N�g���̃I�v�V�����̕`��
	/// </summary>
	void DrawStageSelect();

	/// <summary>
	/// �|�[�Y���̃I�v�V�����̕`��
	/// </summary>
	void DrawPause();

private:
	// �ݒ荀��
	enum class OptionItem
	{
		WINDOW_MODE,	// �E�B���h�E���[�h
		MASTER_VOLUME,	// �}�X�^�[�{�����[��
		BGM_VOLUME,		// BGM�{�����[��
		SE_VOLUME,		// SE�{�����[��
		NUM				// ���ڐ�
	};

private:
	// �X�e�[�g�}�V��
	StateMachine<State> m_updateStateMachine;
	StateMachine<State> m_drawStateMachine;

	// �ǂ̃V�[������J�ڂ��Ă�����
	State m_state;

	// �I�𒆂̍���
	int m_currentSelectItem;

	// ���U�C�N�����p�X�N���[��
	int m_gaussScreen;

	// �摜�n���h��
	int m_soundIconHandle;
	int m_rbButtonImgHandle;
	int m_lbButtonImgHandle;

	// ���ڂ̕`��F�f�[�^
	std::vector<unsigned int> m_itemColorTable;
};