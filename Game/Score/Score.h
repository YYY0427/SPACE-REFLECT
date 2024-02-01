#pragma once
#include <array>

// スコアの種類
enum class ScoreType
{
	// 増加項目
	ENEMY,		// 敵を倒したスコア
	BOSS,		// ボスを倒したスコア
	ITEM,		// アイテムを取得したスコア

	// 減少項目
	DAMAGE,		// ダメージをスコア
	DEATH,		// 死亡時のスコア

	// スコアの種類の数
	NUM			
};

/// <summary>
/// スコアクラス
/// </summary>
class Score
{
public:
	// デストラクタ
	~Score();

	// スコアの加算
	void AddScore(ScoreType type);

	// スコアの描画
	void DrawScore();

	// ゲッター
	static Score& GetInstance();		// インスタンスの取得
	int GetScore(ScoreType type) const;	// スコアの取得
	int GetTotalScore() const;			// 合計スコアの取得

private:
	// コンストラクタ
	Score();

	// コピー、代入禁止
	Score(const Score&) = delete;
	void operator=(const Score&) = delete;

private:
	// スコア
	std::array<int, static_cast<int>(ScoreType::NUM)> m_score;
};