#pragma once
#include "UIBase.h"
#include "../Util/Range.h"
#include <array>

/// <summary>
/// �{�X�o�ꎞ�̌x��UI
/// </summary>
class Warning final : public UIBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="drawFrame">�`��t���[��</param>
	Warning(const int drawFrame);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Warning();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override final;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override final;

	/// <summary>
	/// �I������
	/// </summary>
	/// <returns>�I�����Ă��邩</returns>
	bool IsEnd() const;

private:
	// ���ʗp
	enum class ID
	{
		CENTER,		// ����
		UP,			// ��
		DOWN,		// ��
	};

private:
	// UI�f�[�^
	struct Data
	{
		int imgHandle;
		Math::Vector2 pos;
		int scrollDirection;
		int imgWidth;
		int imgHeight;
	};

private:
	// UI�f�[�^�̔z��
	std::array<Data, 3> m_uiDataTable;	

	// �I���t���O
	bool m_isEnd;	

	// �X�N���[��
	int m_scroll;

	// �`��t���[��
	int m_drawFrame;

	// �x�����̃A���t�@�l
	int m_stringAlphaParam;

	// �x���摜�̃A���t�@�l
	int m_imgAlphaParam;

	// �x���摜�̃A���t�@�l�̑����l
	int m_addImgAlphaParam;

	// �x���摜�̃A���t�@�l�͈̔�
	Range<int> m_ImgAlphaParamRange;
};