#pragma once
#include <string>
#include <vector>

/// <summary>
/// スコアランキング
/// </summary>
class ScoreRanking
{
public:
	// コンストラクタ
	ScoreRanking();

	// デストラクタ
	~ScoreRanking();

	// ファイルからスコアの読み込み
	void LoadScore(std::string fileName);

private:
	// スコアの保存データ
	struct ScoreSaveData
	{
		// プレイヤー名(20文字まで)
		char playerName[20];

		// スコア
		int score;
	};

private:
	// スコアの保存データ
	std::vector<std::vector<ScoreSaveData>> m_scoreSaveData;	
};