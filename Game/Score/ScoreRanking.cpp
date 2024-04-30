#include "ScoreRanking.h"
#include "../Util/FileUtil.h"
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <DxLib.h>

namespace
{
	// スコアランキングの最大数
	constexpr int score_ranking_max = 5;

	// ファイルのディレクトリ
	const std::string file_directory = "Data/Save/Score/";

	// ファイルの拡張子
	const std::string file_extension = ".dat";
}

// コンストラクタ
ScoreRanking::ScoreRanking()
{
}

// デストラクタ
ScoreRanking::~ScoreRanking()
{
}

// ファイルからスコアの読み込み
void ScoreRanking::LoadScore()
{
	// フォルダ名の一覧を取得
	std::vector<std::string> directoryNames = FileUtil::LoadFileNames(file_directory);

	// 全てのフォルダを読み込む
	for (auto& directory : directoryNames)
	{
		// ステージごとのスコア
		StageScore stageScore{};
		stageScore.stageName = directory;

		// ファイル名の一覧を取得
		std::vector<std::string> fileNames = FileUtil::LoadFileNames(file_directory + directory, file_extension);

		// 全てのファイルを読み込む
		for (auto& fileName : fileNames)
		{
			// スコアの読み込み
			std::string filePath = file_directory + directory + "/" + fileName;
			//std::ifstream inputFile(filePath, std::ios::binary);
			//inputFile.open(filePath, std::ios_base::binary);

			//// ファイルが開けたか
			//if (inputFile.is_open())
			//{
			//	// スコアの読み込み
			//	ScoreSaveData scoreData{};
			//	inputFile.read(reinterpret_cast<char*>(&scoreData), sizeof(ScoreSaveData));
			//	stageScore.scoreSaveData.push_back(scoreData);

			//	// ファイルを閉じる
			//	inputFile.close();
			//}

			// スコアの読み込み
			FILE* fp = nullptr;
			fopen_s(&fp, filePath.c_str(), "rb");
			if (fp)
			{
				// スコアの読み込み
				ScoreSaveData scoreData{};
				fread(&scoreData, sizeof(ScoreSaveData), 1, fp);
				fclose(fp);
				stageScore.scoreSaveData.push_back(scoreData);
			}
		}

		// スコアのソート
		std::sort(stageScore.scoreSaveData.begin(), stageScore.scoreSaveData.end(), [](const ScoreSaveData& a, const ScoreSaveData& b)
			{
			return a.score > b.score;
		});

		// スコアの保存
		m_scoreSaveData.push_back(stageScore);
	}
}

// スコアのデータをバイナリファイルに保存する
void ScoreRanking::SaveScore()
{
	for (auto& stageScore : m_scoreSaveData)
	{
		// ディレクトリの作成
		std::filesystem::create_directories(file_directory + stageScore.stageName);

		// スコアの書き込み
		for (int i = 0; i < stageScore.scoreSaveData.size(); i++)
		{
			// ファイル名
			std::string fileName = file_directory + stageScore.stageName + "/" + std::to_string(i) + file_extension;

			// ファイルへの書き込み
			std::ofstream outputFile(fileName, std::ios::binary);
			outputFile.write(reinterpret_cast<char*>(&stageScore.scoreSaveData[i]), sizeof(ScoreSaveData));
			
			// ファイルを閉じる
			outputFile.close();
						
		}
	}
}

// スコアデータの新規作成
void ScoreRanking::CreateNewScoreData(const std::string& stageName, const std::string& playerName, const int score)
{
	// ステージのスコアがない場合は追加
	for (auto& stageScore : m_scoreSaveData)
	{
		if (stageScore.stageName == stageName)
		{
			return;
		}
	}

	// ステージ名の追加
	StageScore stageScore{};
	stageScore.stageName = stageName;

	if (score > 0)
	{
		// スコアの追加
		ScoreSaveData scoreData{};
		strcpy_s(scoreData.playerName, playerName.c_str());
		if (playerName == "") strcpy_s(scoreData.playerName, "NO NAME");
		scoreData.score = score;
		stageScore.scoreSaveData.push_back(scoreData);
	}

	// スコアの保存(新規作成なので全て0を入れる)
	for (int i = 0; i < score_ranking_max; i++)
	{
		ScoreSaveData scoreData{};
		strcpy_s(scoreData.playerName, "NO NAME");
		scoreData.score = 0;
		stageScore.scoreSaveData.push_back(scoreData);
	}

	// ソート
	std::sort(stageScore.scoreSaveData.begin(), stageScore.scoreSaveData.end(), [](const ScoreSaveData& a, const ScoreSaveData& b)
		{
		return a.score > b.score;
	});
	
	// スコアの保存
	m_scoreSaveData.push_back(stageScore);
}

// スコアのデータを追加
void ScoreRanking::AddScore(const std::string& stageName, const char* playerName, const int score)
{
	// ステージのスコアがない場合は追加
	for (auto& stageScore : m_scoreSaveData)
	{
		if (stageScore.stageName == stageName)
		{
			// スコアの追加
			ScoreSaveData scoreData{};
			strcpy_s(scoreData.playerName, playerName);
			scoreData.score = score;
			stageScore.scoreSaveData.push_back(scoreData);

			// スコアのソート
			std::sort(stageScore.scoreSaveData.begin(), stageScore.scoreSaveData.end(), [](const ScoreSaveData& a, const ScoreSaveData& b)
				{
				return a.score > b.score;
			});

			// スコアの最大数を超えたら削除
			if (stageScore.scoreSaveData.size() > score_ranking_max)
			{
				stageScore.scoreSaveData.pop_back();
			}
			return;
		}
	}
	// ステージがない場合は新規作成
	CreateNewScoreData(stageName, playerName, score);
}

// インスタンスの取得
ScoreRanking& ScoreRanking::GetInstance()
{
	// 唯一のインスタンスの取得
	static ScoreRanking instance;
	return instance;
}

// スコアデータテーブルの取得
const std::vector<StageScore>& ScoreRanking::GetScoreData() const
{
	return m_scoreSaveData;
}

// ステージ名からスコアデータを取得
std::vector<ScoreSaveData> ScoreRanking::GetScoreData(const std::string& stageName) const
{
	// ステージ名が一致するものを返す
	for (auto& stageScore : m_scoreSaveData)
	{
		if (stageScore.stageName == stageName)
		{
			return stageScore.scoreSaveData;
		}
	}
	return std::vector<ScoreSaveData>();
}