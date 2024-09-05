#pragma once
#include <string>
#include <vector>
#include "../Util/SingletonBase.h"

namespace Score
{
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
	class Ranking : public SingletonBase<Ranking>
	{
	public:
		// SingletonBaseクラスでのみインスタンス生成を許可する
		friend class SingletonBase<Ranking>;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Ranking();

		/// <summary>
		/// ファイルからスコアの読み込み 
		/// </summary>
		void LoadScore();

		/// <summary>
		/// スコアのデータをバイナリファイルに保存する 
		/// </summary>
		void SaveScore();

		/// <summary>
		/// スコアデータを新規作成
		/// </summary>
		/// <param name="stageName">ステージ名</param>
		/// <param name="playerName">プレイヤー名</param>
		/// <param name="score">スコア</param>
		void CreateNewScoreData(const std::string& stageName, const std::string& playerName = "", const int score = 0);

		/// <summary>
		/// スコアデータの追加
		/// </summary>
		/// <param name="stageName">ステージ名</param>
		/// <param name="playerName">プレイヤー名</param>
		/// <param name="score">スコア</param>
		void AddScore(const std::string& stageName, const char* playerName, const int score);

		/// <summary>
		/// スコアデータテーブルの取得
		/// </summary>
		/// <returns>スコアデータテーブル</returns>
		const std::vector<StageScore>& GetScoreData() const;

		/// <summary>
		/// ステージ名からスコアデータを取得
		/// </summary>
		/// <param name="stageName">ステージ名</param>
		/// <returns>スコアデータ</returns>
		std::vector<ScoreSaveData> GetScoreData(const std::string& stageName) const;

	private:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		Ranking();

	private:
		// スコアの保存データ
		std::vector<StageScore> m_scoreSaveData;
	};
}