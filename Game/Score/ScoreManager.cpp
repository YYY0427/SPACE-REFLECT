#include "ScoreManager.h"
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

namespace Score
{
	// コンストラクタ
	Manager::Manager()
	{
		// スコアの初期化
		Reset();
	}

	// デストラクタ
	Manager::~Manager()
	{
	}

	// スコアの加算
	void Manager::AddScore(const Type type)
	{
		switch (type)
		{
		case Type::ENEMY:
			m_score[static_cast<int>(type)] += enemy_score;
			break;
		case Type::BOSS:
			m_score[static_cast<int>(type)] += boss_score;
			break;
		case Type::ITEM:
			break;
		case Type::DAMAGE:
			m_score[static_cast<int>(type)] += damage_score;
			break;
		default:
			break;
		}

		// スコアの最大値を超えないようにする
		m_score[static_cast<int>(type)] = (std::min)(m_score[static_cast<int>(type)], max_score);
	}

	// スコアの描画
	void Manager::DrawScore() const
	{
		// フォントの取得
		auto& data = String::MessageManager::GetInstance()->GetMessageData("ScoreNumber");

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

	// スコアの取得
	int Manager::GetScore(const Type type) const
	{
		return m_score[static_cast<int>(type)];
	}

	// 合計スコアの取得
	int Manager::GetTotalScore() const
	{
		int totalScore = 0;
		for (int i = 0; i < static_cast<int>(Type::NUM); i++)
		{
			totalScore += m_score[i];
		}

		// スコアの最大値、最小値を超えないようにする
		if (totalScore < 0) totalScore = 0;
		if (totalScore > max_score) totalScore = max_score;

		return totalScore;
	}

	// スコアのリセット
	void Manager::Reset()
	{
		// スコアの初期化
		for (int i = 0; i < static_cast<int>(Type::NUM); i++)
		{
			m_score[i] = 0;
		}
	}
}