#include "Score.h"
#include "../String/MessageManager.h"

namespace
{
	// スコアの最大値
	constexpr int max_score = 9999;

	// スコア
	constexpr int enemy_score = 100;
	constexpr int boss_score = 1000;
	constexpr int damage_score = -10;
	constexpr int death_score = -1000;
}

// コンストラクタ
Score::Score()
{
	// スコアの初期化
	Reset();
}

// デストラクタ
Score::~Score()
{
}

// スコアの加算
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

	// スコアの最大値を超えないようにする
	m_score[static_cast<int>(type)] = (std::min)(m_score[static_cast<int>(type)], max_score);
}

// スコアの描画
void Score::DrawScore()
{
	// フォントの取得
	auto data = MessageManager::GetInstance().GetMessageData("ScoreNumber");

	// スコアの文字列の作成
	std::string str = std::to_string(GetTotalScore());
	while (str.size() < 4)
	{
		str = "0" + str;
	}
	str = "SCORE:  " + str;

	// スコアの描画
	DrawStringToHandle(10, 10, str.c_str(), 0xffffff, data.fontHandle);
}

// インスタンスの取得
Score& Score::GetInstance()
{
	// 唯一のインスタンスを返す
	static Score instance;
	return instance;
}

// スコアの取得
int Score::GetScore(const ScoreType type) const
{
	return m_score[static_cast<int>(type)];
}

// 合計スコアの取得
int Score::GetTotalScore() const
{
	int totalScore = 0;
	for (int i = 0; i < static_cast<int>(ScoreType::NUM); i++)
	{
		totalScore += m_score[i];
	}

	// スコアの最大値、最小値を超えないようにする
	if(totalScore < 0) totalScore = 0;
	if(totalScore > max_score) totalScore = max_score;

	return totalScore;
}

// スコアのリセット
void Score::Reset()
{
	// スコアの初期化
	for (int i = 0; i < static_cast<int>(ScoreType::NUM); i++)
	{
		m_score[i] = 0;
	}
}