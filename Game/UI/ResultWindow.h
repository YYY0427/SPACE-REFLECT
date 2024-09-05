#pragma once
#include "../Math/Vector2.h"
#include <string>

/// <summary>
/// ���U���g���
/// </summary>
class ResultWindow
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ResultWindow();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ResultWindow();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �I���t���O�̎擾 
	/// </summary>
	/// <returns>�I���t���O</returns>	
	bool IsEnd() const;

private:
	// �E�B���h�E
	Math::Vector2 m_windowPos;	// �ʒu
	Math::Vector2 m_windowSize;	// �T�C�Y
	bool m_isEndWindow;		// �I���t���O

	// �e�L�X�g
	Math::Vector2 m_missionPos;	// �~�b�V����
	Math::Vector2 m_completePos;	// �R���v���[�g
	Math::Vector2 m_scorePos;		// �X�R�A
	Math::Vector2 m_yourScorePos;	// YOUR SCORE

	// �C�[�W���O
	Math::Vector2 m_easing;

	// �^�C�g���I���t���O
	bool m_isTitleEnd;

	// �I���t���O
	bool m_isEnd;

	// �T�E���h��炵����
	bool m_isOpenMenuSound;
};