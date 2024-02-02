#pragma once
#include <string>
#include <vector>

// スコアの保存データ
struct ScoreSaveData
{
	// プレイヤー名(20文字まで)
	char playerName[20];

	// スコア
	int score;
};

// ステージごとのスコア
struct StageScore
{
	// ステージ名
	std::string stageName;

	// スコア
	std::vector<ScoreSaveData> scoreSaveData;
};

/// <summary>
/// スコアランキング
/// </summary>
class ScoreRanking
{
public:
	// デストラクタ
	~ScoreRanking();

	// ファイルからスコアの読み込み
	void LoadScore();

	// スコアのデータをバイナリファイルに保存する
	void SaveScore();

	// 新規作成
	void CreateNewScoreData(const std::string& stageName, std::string playerName = "", int score = 0);

	// スコアのデータを追加
	void AddScore(const std::string& stageName, const char* playerName, int score);

	// ゲッター
	static ScoreRanking& GetInstance();					 // インスタンスの取得
	const std::vector<StageScore>& GetScoreData() const; // スコアデータの取得

private:
	// コンストラクタ
	ScoreRanking();

	// コピー、代入禁止
	ScoreRanking(const ScoreRanking&) = delete;
	void operator=(const ScoreRanking&) = delete;

private:
	// スコアの保存データ
	std::vector<StageScore> m_scoreSaveData;	
};