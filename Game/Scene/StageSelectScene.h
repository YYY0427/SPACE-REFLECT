#pragma once
#include "SceneBase.h"
#include "../Score/ScoreRanking.h"
#include "../Math/Vector3.h"
#include "../Math/Vector2.h"
#include "../Game/Planet.h"
#include "../StateMachine.h"
#include <memory>
#include <vector>
#include <map>
#include <string>

// �v���g�^�C�v�錾
class Camera;
class Model;
class PlanetManager;
class SkyDome;

// �X�e�[�W����
enum class Stage
{
	TUTORIAL,	// �`���[�g���A��
	STAGE_1,	// �X�e�[�W1
	NUM			// ���ڐ�
};

/// <summary>
/// �X�e�[�W�I���V�[��
/// </summary>
class StageSelectScene final : public SceneBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[</param>
	StageSelectScene(SceneManager& manager);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~StageSelectScene();

	/// <summary>
	/// ������
	/// </summary>
	void Init() override final;

	/// <summary>
	/// �I������
	/// </summary>
	void End() override final;

	/// <summary>
	/// �X�e�[�W��؂�ւ����̏��� 
	/// </summary>
	void SelectStageProcess();

	/// <summary>
	/// �X�^�[�g���o�̊J�n
	/// </summary>
	void EnterStartAnimation();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override final;

	/// <summary>
	/// �X�e�[�W�I���̍X�V
	/// </summary>
	void UpdateSelectStage();

	/// <summary>
	/// �X�^�[�g���o�̍X�V
	/// </summary>
	void UpdateStartAnimation();

	/// <summary>
	/// �J�����̍X�V
	/// </summary>
	void UpdateCamera();

	/// <summary>
	/// �����L���O�̃A���t�@�l�̍X�V
	/// </summary>
	void UpdateRankingAlpha();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override final;

	/// <summary>
	/// �X�R�A�����L���O�̕`��
	/// </summary>
	void DrawScoreRanking();

private:
	// �X�e�[�W�f�[�^
	struct StageData
	{
		Vector3					cameraPos;		// �J�����̈ʒu
		std::string				stageNameId;	// �X�e�[�W��
		std::string				missionNameId;	// �~�b�V������
		std::string				difficultyId;	// ���Гx
		std::string				conditionsId;	// �B������
		std::shared_ptr<Planet> pPlanet;		// �f���̃|�C���^
	};

private:
	// �X�e�[�g
	enum class State
	{
		STAGE_SELECT,		// �X�e�[�W�I��
		START_ANIMATION,	// �X�^�[�g���o
	};

	// UI�̎��
	enum class UI
	{
		EXPLANATION_WINDOW,			// �����E�B���h�E
		EXPLANATION_WINDOW_FRAME,	// �����E�B���h�E�̘g
		LINE_3D,					// 3D���C��
		SCORE_RANKING,				// �X�R�A�����L���O
		BUTTON,						// �{�^��
		NUM							// ���ڐ�
	};

private:
	// �X�e�[�g�}�V��
	StateMachine<State> m_stateMachine;

	// �|�C���^
	std::unique_ptr<Camera> m_pCamera;
	std::unique_ptr<PlanetManager> m_pPlanetManager;
	std::unique_ptr<SkyDome> m_pSkyDome;

	// �X�e�[�W�f�[�^
	std::map<Stage, StageData> m_stageData;

	// �X�R�A�����L���O
	std::vector<ScoreSaveData> m_scoreRanking;

	// �J����
	Vector3 m_cameraStartPos;		// �J�����̏����ʒu
	Vector3 m_cameraGoalPos;		// �J�����̖ڕW�ʒu
	Vector3 m_cameraStartTargetPos;	// �J�����̏��������_
	Vector3 m_cameraGoalTargetPos;	// �J�����̖ڕW�����_

	// �C�[�W���O�̎���
	int m_easeTime;
	int m_easeTime2;

	// ���͂����邩�t���O
	bool m_isInput;

	// �A���t�@�l
	int m_line3DAlpha;
	int m_windowAlpha;
	int m_textAlpha;
	std::vector<int> m_rankingAlpha;
	int m_uiAlpha;

	// �X�N���[���n���h��
	int m_screenHandle;

	// �摜�n���h��
	int m_rbButtonImgHandle;
	int m_lbButtonImgHandle;
	int m_bButtonImgHandle;
	int m_aButtonImgHandle;

	// ���C�g�n���h��
	int m_lightHandle;

	// �I������Ă��鍀��
	int m_currentSelectItem;

	// �����E�B���h�E
	Math::Vector2 m_explanationWindowEasingTime;	// �E�B���h�E�̃C�[�W���O����
	Math::Vector2 m_explanationWindowSize;		// �E�B���h�E�̃T�C�Y

	// �X�^�[�g���o��SE�̍Đ��t���O
	bool m_isStartAnimSE;	

	// �X�e�[�W�I�����ɃJ�������ړ�����C�[�W���O����
	int m_selectStageCameraEasingTime;

	// �X�e�[�W���莞�̃J�����̒����_�̃C�[�W���O����
	int m_decisionStageTargetEasingTime;

	// �X�e�[�W���莞�ɃJ�������ړ�����C�[�W���O����
	int m_decisionStageCameraEasingTime;
};