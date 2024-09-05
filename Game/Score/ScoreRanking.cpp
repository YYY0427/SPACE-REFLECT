#include "ScoreRanking.h"
#include "../Util/FileUtil.h"
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <DxLib.h>

namespace
{
	// �X�R�A�����L���O�̍ő吔
	constexpr int score_ranking_max = 5;

	// �t�@�C���̃f�B���N�g��
	const std::string file_directory = "Data/Save/Score/";

	// �t�@�C���̊g���q
	const std::string file_extension = ".dat";
}

namespace Score
{

	// �R���X�g���N�^
	Ranking::Ranking()
	{
	}

	// �f�X�g���N�^
	Ranking::~Ranking()
	{
	}

	// �t�@�C������X�R�A�̓ǂݍ���
	void Ranking::LoadScore()
	{
		// �t�H���_���̈ꗗ���擾
		std::vector<std::string> directoryNames = FileUtil::LoadFileNames(file_directory);

		// �S�Ẵt�H���_��ǂݍ���
		for (auto& directory : directoryNames)
		{
			// �X�e�[�W���Ƃ̃X�R�A
			StageScore stageScore{};
			stageScore.stageName = directory;

			// �t�@�C�����̈ꗗ���擾
			std::vector<std::string> fileNames = FileUtil::LoadFileNames(file_directory + directory, file_extension);

			// �S�Ẵt�@�C����ǂݍ���
			for (auto& fileName : fileNames)
			{
				// �X�R�A�̓ǂݍ���
				std::string filePath = file_directory + directory + "/" + fileName;
				//std::ifstream inputFile(filePath, std::ios::binary);
				//inputFile.open(filePath, std::ios_base::binary);

				//// �t�@�C�����J������
				//if (inputFile.is_open())
				//{
				//	// �X�R�A�̓ǂݍ���
				//	ScoreSaveData scoreData{};
				//	inputFile.read(reinterpret_cast<char*>(&scoreData), sizeof(ScoreSaveData));
				//	stageScore.scoreSaveData.push_back(scoreData);

				//	// �t�@�C�������
				//	inputFile.close();
				//}

				// �X�R�A�̓ǂݍ���
				FILE* fp = nullptr;
				fopen_s(&fp, filePath.c_str(), "rb");
				if (fp)
				{
					// �X�R�A�̓ǂݍ���
					ScoreSaveData scoreData{};
					fread(&scoreData, sizeof(ScoreSaveData), 1, fp);
					fclose(fp);
					stageScore.scoreSaveData.push_back(scoreData);
				}
			}

			// �X�R�A�̃\�[�g
			std::sort(stageScore.scoreSaveData.begin(), stageScore.scoreSaveData.end(), [](const ScoreSaveData& a, const ScoreSaveData& b)
				{
					return a.score > b.score;
				});

			// �X�R�A�̕ۑ�
			m_scoreSaveData.push_back(stageScore);
		}
	}

	// �X�R�A�̃f�[�^���o�C�i���t�@�C���ɕۑ�����
	void Ranking::SaveScore()
	{
		for (auto& stageScore : m_scoreSaveData)
		{
			// �f�B���N�g���̍쐬
			std::filesystem::create_directories(file_directory + stageScore.stageName);

			// �X�R�A�̏�������
			for (int i = 0; i < stageScore.scoreSaveData.size(); i++)
			{
				// �t�@�C����
				std::string fileName = file_directory + stageScore.stageName + "/" + std::to_string(i) + file_extension;

				// �t�@�C���ւ̏�������
				std::ofstream outputFile(fileName, std::ios::binary);
				outputFile.write(reinterpret_cast<char*>(&stageScore.scoreSaveData[i]), sizeof(ScoreSaveData));

				// �t�@�C�������
				outputFile.close();
			}
		}
	}

	// �X�R�A�f�[�^�̐V�K�쐬
	void Ranking::CreateNewScoreData(const std::string& stageName, const std::string& playerName, const int score)
	{
		// �X�e�[�W�̃X�R�A���Ȃ��ꍇ�͒ǉ�
		for (auto& stageScore : m_scoreSaveData)
		{
			if (stageScore.stageName == stageName)
			{
				return;
			}
		}

		// �X�e�[�W���̒ǉ�
		StageScore stageScore{};
		stageScore.stageName = stageName;

		if (score > 0)
		{
			// �X�R�A�̒ǉ�
			ScoreSaveData scoreData{};
			strcpy_s(scoreData.playerName, playerName.c_str());
			if (playerName == "") strcpy_s(scoreData.playerName, "NO NAME");
			scoreData.score = score;
			stageScore.scoreSaveData.push_back(scoreData);
		}

		// �X�R�A�̕ۑ�(�V�K�쐬�Ȃ̂őS��0������)
		for (int i = 0; i < score_ranking_max; i++)
		{
			ScoreSaveData scoreData{};
			strcpy_s(scoreData.playerName, "NO NAME");
			scoreData.score = 0;
			stageScore.scoreSaveData.push_back(scoreData);
		}

		// �\�[�g
		std::sort(stageScore.scoreSaveData.begin(), stageScore.scoreSaveData.end(), [](const ScoreSaveData& a, const ScoreSaveData& b)
			{
				return a.score > b.score;
			});

		// �X�R�A�̕ۑ�
		m_scoreSaveData.push_back(stageScore);
	}

	// �X�R�A�̃f�[�^��ǉ�
	void Ranking::AddScore(const std::string& stageName, const char* playerName, const int score)
	{
		// �X�e�[�W�̃X�R�A���Ȃ��ꍇ�͒ǉ�
		for (auto& stageScore : m_scoreSaveData)
		{
			if (stageScore.stageName == stageName)
			{
				// �X�R�A�̒ǉ�
				ScoreSaveData scoreData{};
				strcpy_s(scoreData.playerName, playerName);
				scoreData.score = score;
				stageScore.scoreSaveData.push_back(scoreData);

				// �X�R�A�̃\�[�g
				std::sort(stageScore.scoreSaveData.begin(), stageScore.scoreSaveData.end(), [](const ScoreSaveData& a, const ScoreSaveData& b)
					{
						return a.score > b.score;
					});

				// �X�R�A�̍ő吔�𒴂�����폜
				if (stageScore.scoreSaveData.size() > score_ranking_max)
				{
					stageScore.scoreSaveData.pop_back();
				}
				return;
			}
		}
		// �X�e�[�W���Ȃ��ꍇ�͐V�K�쐬
		CreateNewScoreData(stageName, playerName, score);
	}

	// �X�R�A�f�[�^�e�[�u���̎擾
	const std::vector<StageScore>& Ranking::GetScoreData() const
	{
		return m_scoreSaveData;
	}

	// �X�e�[�W������X�R�A�f�[�^���擾
	std::vector<ScoreSaveData> Ranking::GetScoreData(const std::string& stageName) const
	{
		// �X�e�[�W������v������̂�Ԃ�
		for (auto& stageScore : m_scoreSaveData)
		{
			if (stageScore.stageName == stageName)
			{
				return stageScore.scoreSaveData;
			}
		}
		return std::vector<ScoreSaveData>();
	}
}