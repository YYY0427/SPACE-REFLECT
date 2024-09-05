#include "ResultWindow.h"
#include "../Application.h"
#include "../Util/InputState.h"
#include "../Util/Easing.h"
#include "../String/MessageManager.h"
#include "../Score/Score.h"
#include "../Sound/SoundManager.h"
#include <DxLib.h>

namespace
{
	// �E�B���h�E�T�C�Y
	const Math::Vector2 window_size = { 1280, 570 };

	// MISSION�̈ʒu
	const Math::Vector2 init_mission_pos = { -500, 200 };
	const Math::Vector2 goal_mission_pos = { 300, 200 };

	// COMPLETE�̈ʒu
	const Math::Vector2 init_complete_pos = { 1700, 270 };
	const Math::Vector2 goal_complete_pos = { 440, 270 };

	// YOUR SCORE�̈ʒu
	const Math::Vector2 init_your_score_pos = { 1700, 425 };
	const Math::Vector2 your_score_pos = { 800, 425 };

	// �X�R�A�̈ʒu
	const Math::Vector2 init_score_pos = { 1700, 450 };
	const Math::Vector2 score_pos = { 750, 450 };
}

// �R���X�g���N�^
ResultWindow::ResultWindow() :
	m_isEnd(false),
	m_isEndWindow(false),
	m_easing({ 0.0f, 0.0f }),
	m_missionPos(init_mission_pos),
	m_completePos(init_complete_pos),
	m_isTitleEnd(false),
	m_scorePos(init_score_pos),
	m_yourScorePos(init_your_score_pos),
	m_isOpenMenuSound(false)
{
	auto& screenSize = Application::GetInstance()->GetWindowSize();
	m_windowPos = { screenSize.width / 2.0f, screenSize.height / 2.0f };
	m_windowSize = { 0, 0 };
}

// �f�X�g���N�^
ResultWindow::~ResultWindow()
{
}

// �X�V
void ResultWindow::Update()
{
	// ���X�ɃE�B���h�E��傫������
	if (!m_isEndWindow)
	{
		m_easing.x++;
		m_windowSize.x = Easing::EaseOutCubic(m_easing.x, 100, window_size.x, m_windowSize.x);
		if (m_windowSize.x >= window_size.x)
		{
			if (!m_isOpenMenuSound)
			{
				Sound::Manager::GetInstance()->PlaySE("ResultOpenMenu");
				m_isOpenMenuSound = true;
			}

			m_easing.y++;
			m_windowSize.y = Easing::EaseOutCubic(m_easing.y, 100, window_size.y, m_windowSize.y);
			if (m_windowSize.y >= window_size.y)
			{
				m_isEndWindow = true;
				m_easing = { 0.0f, 0.0f };
			}
		}
	}

	// �E�B���h�E���I��������
	if (m_isEndWindow && !m_isTitleEnd)
	{
		// MISSION�̈ʒu���ړ�
		m_easing.x++;
		m_missionPos.x = Easing::EaseOutCubic(m_easing.x, 100, goal_mission_pos.x, m_missionPos.x);
		m_missionPos.y = Easing::EaseOutCubic(m_easing.x, 100, goal_mission_pos.y, m_missionPos.y);

		// COMPLETE�̈ʒu���ړ�
		m_easing.y++;
		m_completePos.x = Easing::EaseOutCubic(m_easing.y, 100, goal_complete_pos.x, m_completePos.x);
		m_completePos.y = Easing::EaseOutCubic(m_easing.y, 100, goal_complete_pos.y, m_completePos.y);

		// �^�C�g���I���t���O
		if (m_completePos.x == goal_complete_pos.x && m_completePos.y == goal_complete_pos.y)
		{
			// ������
			m_isTitleEnd = true;
			m_easing = { 0.0f, 0.0f };
		}
	}

	if (m_isTitleEnd)
	{
		m_easing.x++;
		m_scorePos.x = Easing::EaseOutCubic(m_easing.x, 100, score_pos.x, m_scorePos.x);
		m_scorePos.y = Easing::EaseOutCubic(m_easing.x, 100, score_pos.y, m_scorePos.y);

		m_easing.y++;
		m_yourScorePos.x = Easing::EaseOutCubic(m_easing.y, 100, your_score_pos.x, m_yourScorePos.x);
		m_yourScorePos.y = Easing::EaseOutCubic(m_easing.y, 100, your_score_pos.y, m_yourScorePos.y);
	}

	// ����{�^���������ꂽ��V�[���J��
	if (InputState::IsTriggered(InputType::DECISION))
	{
		m_isEnd = true;
	}
}

// �`��
void ResultWindow::Draw()
{
	auto& screenSize = Application::GetInstance()->GetWindowSize();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(m_windowPos.x - (m_windowSize.x / 2), m_windowPos.y - (m_windowSize.y / 2),
			m_windowPos.x + (m_windowSize.x / 2), m_windowPos.y + (m_windowSize.y / 2), 0xeeeeee, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// MISSION�̕`��
	String::MessageManager::GetInstance()->DrawStringCenter(
		"ResultMission", m_missionPos.x, m_missionPos.y, GetColor(255, 255, 255), 0xffffff);
	// COMPLETE�̕`��
	String::MessageManager::GetInstance()->DrawStringCenter(
		"ResultComplete", m_completePos.x, m_completePos.y, GetColor(255, 255, 255), 0xffffff);

	// �^�C�g�����I��������
	if (m_isTitleEnd)
	{
		// �t�H���g�̎擾
		auto data = String::MessageManager::GetInstance()->GetMessageData("ResultScore");
		int score = Score::GetInstance().GetTotalScore();
		std::string str = std::to_string(score);
		while (str.size() < 4)
		{
			str = "0" + str;
		}

		// YOUR SCORE�̕`��
		String::MessageManager::GetInstance()->DrawStringCenter("ResultScoreName", m_yourScorePos.x, m_yourScorePos.y, 0xffffff, 0xffffff);

		// �X�R�A��1�������`��
		for (int i = 0; i < 4; i++)
		{
			DrawStringToHandle(m_scorePos.x + (i * 100), m_scorePos.y, str.substr(i, 1).c_str(), 0xffffff, data.fontHandle, 0xffffff);
		}
	}
}

// �I���t���O�̎擾
bool ResultWindow::IsEnd() const
{
	return m_isEnd;
}
