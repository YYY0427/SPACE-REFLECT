#pragma once
#include "UIBase.h"
#include "../Math/Vector2.h"
#include "../Util/Timer.h"
#include <string>

/// <summary>
/// �Q�[�W�N���X
/// HP,MP�Ȃǂ̃Q�[�W�Ȃ�
/// </summary>
class Gauge final : public UIBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="gaugeImgFilePath">�Q�[�W�̃t�@�C���p�X</param>
	/// <param name="gaugeBackImgFilePath">�Q�[�W�w�i�̃t�@�C���p�X</param>
	/// <param name="gaugeFrameImgFilePath">�Q�[�W�t���[���̃t�@�C���p�X</param>
	/// <param name="maxValue">�Q�[�W�̍ő�l</param>
	/// <param name="pos">�Q�[�W�̈ʒu</param>
	/// <param name="dimensions">�Q�[�W�̃T�C�Y</param>
	/// <param name="addGaugeFrame">�o�ꉉ�o�̃Q�[�W�̑�����t���[����(0�̏ꍇ�o�ꉉ�o���s��Ȃ�)</param>
	/// <param name="subGaugeFrame">�f�B���C���o�̃Q�[�W�̌���t���[��</param>
	/// <param name="isDelayFrame">�f�B���C���o�̃t���[����(0�ȉ��̏ꍇ�s��Ȃ�)</param>
	Gauge(
		const std::string& gaugeImgFilePath,
		const std::string& gaugeBackImgFilePath,
		const std::string& gaugeFrameImgFilePath,
		const float maxValue,
		const Math::Vector2& pos,
		const Math::Vector2& dimensions,
		const int   addGaugeFrame = 0,
		const int   subGaugeFrame = 0,
		const int   isDelayFrame  = 0);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Gauge();

	/// �X�V
	/// </summary>
	void Update() override final;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override final;

	/// <summary>
	/// �Q�[�W�̒l��ݒ�
	/// </summary>
	/// <param name="afterHp">�ݒ肷��l</param>
	void SetValue(const float afterHp);

	/// <summary>
	/// �o�ꉉ�o���I�������� 
	/// </summary>
	/// <returns>true : �I���Afalse : �r��</returns>
	bool IsEndEntranceAnim() const;

private:
	/// <summary>
	/// �ʏ펞�̍X�V
	/// </summary>
	void NormalUpdate();	

	/// <summary>
	/// �o�ꉉ�o�̍X�V
	/// </summary>
	void EntranceAnimUpdate();

private:
	// �Q�[�W�̍X�V�֐�
	void(Gauge::*m_updateFunc)();

	// �摜�n���h��
	int m_gaugeImgH;		// �Q�[�W
	int m_gaugeBackImgH;	// �Q�[�W�̔w�i
	int m_gaugeFrameImgH;	// �Q�[�W�̘g

	// �Q�[�W�̒l
	float m_currentValue;	// ���݂̒l
	float m_backValue;		// �w�i�̒l
	float m_aimValue;		// �ڕW�̒l
	float m_maxValue;		// �ő�l

	Math::Vector2 m_dimensions;	// �Q�[�W�̑傫��

	// �o�ꉉ�o�ŃQ�[�W��������t���[����
	int m_addGaugeFrame;	

	// �f�B���C���o�ŃQ�[�W�����鑬�x
	int m_subGaugeFrame;	

	// �t���O
	int  m_isDelayFrame;				// �_���[�W���󂯂Ă���Q�[�W������܂ł̎��Ԃ����邩
	bool m_isEndEntranceAnim;		// �o�ꉉ�o���I��������(���o���s��Ȃ��ꍇ��true)

	// �_���[�W���󂯂Ă���̃t���[����
	Timer<int> m_damageFrameCount;
};

