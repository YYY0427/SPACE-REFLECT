#pragma once
#include <string>
#include <vector>

namespace Score
{

}

// �X�R�A�̕ۑ��f�[�^
struct ScoreSaveData
{
	// �v���C���[��(20�����܂�)
	char playerName[20];

	// �X�R�A
	int score;
};

// �X�e�[�W���Ƃ̃X�R�A
struct StageScore
{
	// �X�e�[�W��
	std::string stageName;

	// �X�R�A
	std::vector<ScoreSaveData> scoreSaveData;
};

/// <summary>
/// �X�R�A�����L���O
/// </summary>
class ScoreRanking
{
public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ScoreRanking();

	/// <summary>
	/// �t�@�C������X�R�A�̓ǂݍ��� 
	/// </summary>
	void LoadScore();

	/// <summary>
	/// �X�R�A�̃f�[�^���o�C�i���t�@�C���ɕۑ����� 
	/// </summary>
	void SaveScore();

	/// <summary>
	/// �X�R�A�f�[�^��V�K�쐬
	/// </summary>
	/// <param name="stageName">�X�e�[�W��</param>
	/// <param name="playerName">�v���C���[��</param>
	/// <param name="score">�X�R�A</param>
	void CreateNewScoreData(const std::string& stageName, const std::string& playerName = "", const int score = 0);

	/// <summary>
	/// �X�R�A�f�[�^�̒ǉ�
	/// </summary>
	/// <param name="stageName">�X�e�[�W��</param>
	/// <param name="playerName">�v���C���[��</param>
	/// <param name="score">�X�R�A</param>
	void AddScore(const std::string& stageName, const char* playerName, const int score);

	/// <summary>
	/// �C���X�^���X�̎擾
	/// </summary>
	/// <returns>ScoreRanking�̃C���X�^���X</returns>
	static ScoreRanking& GetInstance();					 

	/// <summary>
	/// �X�R�A�f�[�^�e�[�u���̎擾
	/// </summary>
	/// <returns>�X�R�A�f�[�^�e�[�u��</returns>
	const std::vector<StageScore>& GetScoreData() const; 

	/// <summary>
	/// �X�e�[�W������X�R�A�f�[�^���擾
	/// </summary>
	/// <param name="stageName">�X�e�[�W��</param>
	/// <returns>�X�R�A�f�[�^</returns>
	std::vector<ScoreSaveData> GetScoreData(const std::string& stageName) const;		 

private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ScoreRanking();

	// �R�s�[�A����֎~
	ScoreRanking(const ScoreRanking&) = delete;
	void operator=(const ScoreRanking&) = delete;

private:
	// �X�R�A�̕ۑ��f�[�^
	std::vector<StageScore> m_scoreSaveData;	
};