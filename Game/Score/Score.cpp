#include "Score.h"
#include "../String/MessageManager.h"

namespace
{
	// �X�R�A�̍ő�l
	constexpr int max_score = 9999;

	// �X�R�A
	constexpr int enemy_score = 100;
	constexpr int boss_score = 1000;
	constexpr int damage_score = -10;
	constexpr int death_score = -1000;
}

// �R���X�g���N�^
Score::Score()
{
	// �X�R�A�̏�����
	Reset();
}

// �f�X�g���N�^
Score::~Score()
{
}

// �X�R�A�̉��Z
void Score::AddScore(const ScoreType type)
{
	switch (type)
	{
	case ScoreType::ENEMY:
		m_score[static_cast<int>(type)] += enemy_score;
		break;
	case ScoreType::BOSS:
		m_score[static_cast<int>(type)] += boss_score;
		break;
	case ScoreType::ITEM:
		break;
	case ScoreType::DAMAGE:
		m_score[static_cast<int>(type)] += damage_score;
		break;
	default:
		break;
	}	

	// �X�R�A�̍ő�l�𒴂��Ȃ��悤�ɂ���
	m_score[static_cast<int>(type)] = (std::min)(m_score[static_cast<int>(type)], max_score);
}

// �X�R�A�̕`��
void Score::DrawScore()
{
	// �t�H���g�̎擾
	auto& data = String::MessageManager::GetInstance()->GetMessageData("ScoreNumber");

	// �X�R�A�̕�����̍쐬
	std::string str = std::to_string(GetTotalScore());
	while (str.size() < 4)
	{
		str = "0" + str;
	}
	str = "SCORE:  " + str;

	// �X�R�A�̕`��
	DrawStringToHandle(10, 10, str.c_str(), 0xffffff, data.fontHandle);
}

// �C���X�^���X�̎擾
Score& Score::GetInstance()
{
	// �B��̃C���X�^���X��Ԃ�
	static Score instance;
	return instance;
}

// �X�R�A�̎擾
int Score::GetScore(const ScoreType type) const
{
	return m_score[static_cast<int>(type)];
}

// ���v�X�R�A�̎擾
int Score::GetTotalScore() const
{
	int totalScore = 0;
	for (int i = 0; i < static_cast<int>(ScoreType::NUM); i++)
	{
		totalScore += m_score[i];
	}

	// �X�R�A�̍ő�l�A�ŏ��l�𒴂��Ȃ��悤�ɂ���
	if(totalScore < 0) totalScore = 0;
	if(totalScore > max_score) totalScore = max_score;

	return totalScore;
}

// �X�R�A�̃��Z�b�g
void Score::Reset()
{
	// �X�R�A�̏�����
	for (int i = 0; i < static_cast<int>(ScoreType::NUM); i++)
	{
		m_score[i] = 0;
	}
}