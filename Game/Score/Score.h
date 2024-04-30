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

	// スコアの種類の数
	NUM			
};

/// <summary>
/// スコアを管理するシングルトンクラス
/// </summary>
class Score
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Score();

	/// <summary>
	/// スコアの追加
	/// </summary>
	/// <param name="type">スコアの種類</param>
	void AddScore(const ScoreType type);

	/// <summary>
	/// スコアの描画
	/// </summary>
	void DrawScore();

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns>Scoreクラスのインスタンス</returns>
	static Score& GetInstance(); 

	/// <summary>
	/// スコアの取得
	/// </summary>
	/// <param name="type">スコアの種類</param>
	/// <returns>スコア</returns>
	int GetScore(const ScoreType type) const;	

	/// <summary>
	/// 合計スコアの取得
	/// </summary>
	/// <returns>合計スコア</returns>
	int GetTotalScore() const;			

	/// <summary>
	/// スコアのリセット
	/// </summary>
	void Reset();

private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Score();

	// コピー、代入禁止
	Score(const Score&) = delete;
	void operator=(const Score&) = delete;

private:
	// スコア
	std::array<int, static_cast<int>(ScoreType::NUM)> m_score;
};