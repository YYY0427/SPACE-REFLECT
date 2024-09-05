#include "TransitorBase.h"
#include "../Application.h"
#include "DxLib.h"
#include <cassert>

// �R���X�g���N�^
TransitorBase::TransitorBase(int interval) : 
	m_interval(interval),
	m_frame(0),
	m_oldScene(-1),
	m_nextScene(-1),
	m_isOldScene(false)
{
}

// �f�X�g���N�^
TransitorBase::~TransitorBase()
{
	// ��ʂ��폜
	DeleteGraph(m_oldScene);
	DeleteGraph(m_nextScene);
}

// ���o�J�n
void TransitorBase::Start()
{
	// ��ʃT�C�Y���擾
	const auto& size = Application::GetInstance()->GetWindowSize();

	// ��ʂ��쐬
	m_oldScene = MakeGraph(size.width, size.height);
	m_nextScene = MakeScreen(size.width, size.height);

	// �`���ɐݒ肳��Ă���O���t�B�b�N�̈悩��w��̈�̃O���t�B�b�N��ǂ݂���
	SetOldScene();

	// ������
	m_frame = 0;
}

// ���o���I���������ǂ���
bool TransitorBase::IsEnd() const
{
	return m_frame >= m_interval;
}

// �t���[���̐ݒ�
void TransitorBase::SetFrame(int frame)
{
	m_frame = frame;
}

// �`���ɐݒ肳��Ă���O���t�B�b�N�̈悩��w��̈�̃O���t�B�b�N��ǂ݂���
void TransitorBase::SetOldScene()
{
	// ��ʃT�C�Y���擾
	const auto& size = Application::GetInstance()->GetWindowSize();

	// �`���ɐݒ肳��Ă���O���t�B�b�N�̈悩��w��̈�̃O���t�B�b�N��ǂ݂���
	int result = GetDrawScreenGraph(0, 0, size.width, size.height, m_oldScene, true);
	assert(result != -1);
}