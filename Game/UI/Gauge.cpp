#include "Gauge.h"
#include "../Util/DrawFunctions.h"
#include "../Sound/SoundManager.h"
#include <DxLib.h>

namespace
{
	// �Q�[�W�̃t���[���̃T�C�Y
	constexpr float gauge_frame_size = 3.0f;
}

// �R���X�g���N�^
Gauge::Gauge(
	const std::string& gaugeImgFilePath, 
	const std::string& gaugeBackImgFilePath, 
	const std::string& gaugeFrameImgFilePath, 
	const float maxValue, 
	const Math::Vector2& pos,
	const Math::Vector2& dimensions,
	const int   addGaugeFrame, 
	const int   subGaugeFrame,
	const int   isDelayFrame) :
	m_maxValue(maxValue),
	m_aimValue(maxValue),
	m_dimensions(dimensions),
	m_isDelayFrame(isDelayFrame),
	m_subGaugeFrame(subGaugeFrame),
	m_addGaugeFrame  (addGaugeFrame),
	m_currentValue(0.0f),
	m_backValue(0.0f),
	m_updateFunc(&Gauge::EntranceAnimUpdate),
	m_isEndEntranceAnim(false)
{
	// ������
	m_pos = pos;

	// �摜�̓ǂݍ���
	// �摜���w�肳��Ă��Ȃ��ꍇ��-1��������
	(gaugeImgFilePath == "") ? 
		m_gaugeImgH = -1      : m_gaugeImgH      = my::MyLoadGraph(gaugeImgFilePath.c_str());
	(gaugeBackImgFilePath == "") ?
		m_gaugeBackImgH = -1  : m_gaugeBackImgH  = my::MyLoadGraph(gaugeBackImgFilePath.c_str());
	(gaugeFrameImgFilePath == "") ?
		m_gaugeFrameImgH = -1 : m_gaugeFrameImgH = my::MyLoadGraph(gaugeFrameImgFilePath.c_str());
}

// �f�X�g���N�^
Gauge::~Gauge()
{
	// �摜�̍폜
	DeleteGraph(m_gaugeImgH);
	DeleteGraph(m_gaugeBackImgH);
	DeleteGraph(m_gaugeFrameImgH);
}

// �X�V
void Gauge::Update()
{
	// �X�V�֐����Ă�
	(this->*m_updateFunc)();
}

// �`��
void Gauge::Draw()
{
	// �Q�[�W��0��菬����������`�悵�Ȃ�
	if (m_backValue > 0.0f)
	{
		// �w�i�̃Q�[�W�̕`��
		DrawExtendGraphF(
			m_pos.x - m_dimensions.x / 2,
			m_pos.y - m_dimensions.y / 2,
			m_pos.x - (m_dimensions.x / 2) + ((m_dimensions.x) * (static_cast<float>(m_backValue / m_maxValue))),
			m_pos.y + m_dimensions.y / 2,
			m_gaugeBackImgH,
			true);

		// �Q�[�W�̕`��
		DrawExtendGraphF(
			m_pos.x - m_dimensions.x / 2,
			m_pos.y - m_dimensions.y / 2,
			m_pos.x - (m_dimensions.x / 2) + ((m_dimensions.x) * (static_cast<float>(m_currentValue / m_maxValue))),
			m_pos.y + m_dimensions.y / 2,
			m_gaugeImgH,
			true);
	}
	// �t���[���̕`��
	DrawExtendGraph(
		m_pos.x - (m_dimensions.x / 2),
		m_pos.y - m_dimensions.y / 2 - gauge_frame_size,
		m_pos.x + (m_dimensions.x / 2),
		m_pos.y - m_dimensions.y / 2,
		m_gaugeFrameImgH,
		true);
	DrawExtendGraph(
		m_pos.x - (m_dimensions.x / 2),
		m_pos.y + m_dimensions.y / 2,
		m_pos.x + (m_dimensions.x / 2),
		m_pos.y + m_dimensions.y / 2 + gauge_frame_size,
		m_gaugeFrameImgH,
		true);
	DrawExtendGraph(
		m_pos.x - (m_dimensions.x / 2) - gauge_frame_size,
		m_pos.y - m_dimensions.y / 2 - gauge_frame_size,
		m_pos.x - (m_dimensions.x / 2),
		m_pos.y + m_dimensions.y / 2 + gauge_frame_size,
		m_gaugeFrameImgH,
		true);
	DrawExtendGraph(
		m_pos.x + (m_dimensions.x / 2),
		m_pos.y - m_dimensions.y / 2 - gauge_frame_size,
		m_pos.x + (m_dimensions.x / 2) + gauge_frame_size,
		m_pos.y + m_dimensions.y / 2 + gauge_frame_size,
		m_gaugeFrameImgH,
		true);
}

// �Q�[�W�̒l��ݒ�
void Gauge::SetValue(const float afterValue)
{
	// �f�B���C�^�C���̐ݒ�
	m_damageFrameCount.SetTime(m_isDelayFrame);

	// �ڕW�l��ݒ�
	m_aimValue = afterValue;
	m_aimValue = (std::max)(m_aimValue, 0.0f);
}

// �o�[�X�g���o���I��������
bool Gauge::IsEndEntranceAnim() const
{
	return m_isEndEntranceAnim;
}

// �ʏ펞�̍X�V
void Gauge::NormalUpdate()
{
	// �ڕW�l�����݂̒l�ɍ��킹��
	m_currentValue = m_aimValue;
	m_currentValue = (std::max)(m_currentValue, 0.0f);

	// �_���[�W���o�̍X�V
	m_damageFrameCount.Update(-1);
	if (m_damageFrameCount.GetTime() <= 0)
	{
		// �w�i�̃Q�[�W�ƖڕW�l����v���Ă��Ȃ��ꍇ
		if (m_backValue != m_aimValue)
		{
			// �T�u�Q�[�W�t���[�����ݒ肳��Ă���ꍇ
			if (m_subGaugeFrame > 0)
			{
				// �ڕW�l�Ɍ������Č��߂�ꂽ�t���[�����Ō�������
				m_backValue -= static_cast<float>(m_maxValue / m_subGaugeFrame);
			}
			// �����łȂ��ꍇ
			else
			{
				// �ڕW�l�ɂ���
				m_backValue = m_aimValue;
			}
			
			// �ڕW�ɍ��v������~�߂�
			if (m_backValue < m_aimValue)
			{
				m_backValue = m_aimValue;
			}
		}
	}
}

// �o�ꉉ�o���̍X�V
void Gauge::EntranceAnimUpdate()
{
	// �Q�[�W�̑����t���[�����ݒ肳��Ă���ꍇ
	if (m_addGaugeFrame > 0)
	{
		// �ڕW�l�Ɍ������Č��߂�ꂽ�t���[�����ő�������
		m_currentValue += static_cast<float>(m_maxValue / m_addGaugeFrame);
		m_backValue    += static_cast<float>(m_maxValue / m_addGaugeFrame);
	}
	// �����łȂ��ꍇ
	else
	{
		// �ڕW�l�ɂ���
		m_currentValue = m_maxValue;
		m_backValue    = m_maxValue;
	}

	// �ڕW�l�𒴂�����
	if (m_currentValue >= m_maxValue && 
		m_backValue >= m_maxValue)
	{
		// ���o�I��
		m_isEndEntranceAnim = true;

		// �ڕW�l�ɂ���
		m_currentValue = m_maxValue;
		m_backValue    = m_maxValue;

		// �X�V�֐���ʏ펞�̍X�V�ɖ߂�
		m_updateFunc = &Gauge::NormalUpdate;
	}
}