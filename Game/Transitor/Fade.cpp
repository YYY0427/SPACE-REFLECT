#include "Fade.h"
#include "DxLib.h"
#include "../Application.h"
#include "../Util/Range.h"

namespace
{
	// �ʏ�̃t�F�[�h�̑��x
	constexpr int fade_normal_speed = 8;

	// ���U�C�N�p�����[�^�[�̍ő�l
	constexpr int gauss_max_value = 1400;
}

// �R���X�g���N�^
Fade::Fade() :
	m_isFadeOut(false),
	m_fadeColor(0x000000),
	m_fadeBright(255),
	m_fadeSpeed(0),
	m_fadeBrightRange(0, 255)
{
	// ���U�C�N�����p�̃O���t�B�b�N�̍쐬
	const auto& app = Application::GetInstance();
	m_gaussScreen = MakeScreen(app->GetWindowSize().width, app->GetWindowSize().height);
}

// �f�X�g���N�^
Fade::~Fade()
{
	// ���U�C�N�����p�̃O���t�B�b�N�̍폜
	DeleteGraph(m_gaussScreen);
}

// �X�V
void Fade::Update()
{
	// �t�F�[�h�̖��邳�̍X�V
	m_fadeBright += m_fadeSpeed;

	// �t�F�[�h�̖��邳���ݒ肵���͈͂𒴂�����t�F�[�h���~�߂�
	if (!m_fadeBrightRange.IsInside(m_fadeBright))
	{
		m_fadeSpeed = 0;
	}

	// �t�F�[�h�̖��邳��ݒ肵���͈͓��ɃN�����v����
	m_fadeBright = m_fadeBrightRange.Clamp(m_fadeBright);
}

// �ʏ�t�F�[�h�̕`��
void Fade::DrawFade(bool isDraw)
{
	if (!isDraw) return;

	const auto& app = Application::GetInstance();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeBright);
	DrawBox(0, 0, app->GetWindowSize().width, app->GetWindowSize().height, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// ���U�C�N�t�F�[�h�̕`��
void Fade::DrawGaussFade(bool isDraw)
{
	if (!isDraw) return;

	// 0~255�͈̔͂�0~���U�C�N�p�����[�^�[�̍ő�l�ɕϊ�
	int gaussParameter = m_fadeBright * gauss_max_value / 255;

	// ���U�C�N�摜�̍쐬
	const auto& app = Application::GetInstance();
	GetDrawScreenGraph(0, 0, app->GetWindowSize().width, app->GetWindowSize().height, m_gaussScreen);
	GraphFilter(m_gaussScreen, DX_GRAPH_FILTER_GAUSS, 8, gaussParameter);
	DrawGraph(0, 0, m_gaussScreen, true);
}

// �t�F�[�h�C���̊J�n
void Fade::StartFadeIn(int fadeBrightUpperLimitValue, int fadeSpeed)
{
	// ������
	m_isFadeOut = false;

	// �t�F�[�h�C�����ǂ̂��炢�܂ōs���̂��l��ݒ�
	// 255(�t�F�[�h���Ȃ�)�` 0(�Ō�܂Ńt�F�[�h���s��)
	m_fadeBrightRange = Range<int>(fadeBrightUpperLimitValue, 255);

	// �t�F�[�h���x�̐ݒ�
	m_fadeSpeed = -abs(fadeSpeed);
}

// �t�F�[�h�A�E�g�̊J�n
void Fade::StartFadeOut(int fadeBrightUpperLimitValue, int fadeSpeed)
{
	// �t�F�[�h�A�E�g���s���Ă���ꍇ�͉������Ȃ�
	if(m_isFadeOut) return;

	// �t�F�[�h�A�E�g���s��ꂽ���ǂ����̃t���O�𗧂Ă�
	m_isFadeOut = true;

	// �t�F�[�h�A�E�g���ǂ̂��炢�܂ōs���̂��l��ݒ�
	// 0(�t�F�[�h���Ȃ�)�`255(�Ō�܂Ńt�F�[�h���s��)
	m_fadeBrightRange = Range<int>(0, fadeBrightUpperLimitValue);

	// �t�F�[�h���x�̐ݒ�
	m_fadeSpeed = abs(fadeSpeed);
}

// �t�F�[�h�C�������ǂ���
bool Fade::IsFadingIn() const
{
	return (m_fadeSpeed < 0);
}

// �t�F�[�h�A�E�g�����ǂ���
bool Fade::IsFadingOut() const
{
	return (m_fadeSpeed > 0);
}

// �t�F�[�h�����ǂ���
bool Fade::IsFading() const
{
	return IsFadingIn() || IsFadingOut();
}

// �t�F�[�h�A�E�g���s���ďI���������ǂ���
bool Fade::IsFadeOutEnd() const
{
	return (m_isFadeOut && !IsFadingOut());
}

// �t�F�[�h�̖��邳�̎擾
int Fade::GetFadeBright() const
{
	return m_fadeBright;
}

// �t�F�[�h�̖��邳�̐ݒ�
void Fade::SetFadeBright(int fadeBright)
{
	m_fadeBright = fadeBright;
}