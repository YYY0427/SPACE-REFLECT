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

// プロトタイプ宣言
class Camera;
class Model;
class PlanetManager;
class SkyDome;

// ステージ項目
enum class Stage
{
	TUTORIAL,	// チュートリアル
	STAGE_1,	// ステージ1
	NUM			// 項目数
};

/// <summary>
/// ステージ選択シーン
/// </summary>
class StageSelectScene final : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャー</param>
	StageSelectScene(SceneManager& manager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StageSelectScene();

	// ステージを切り替え時の処理
	void SelectStageProcess();

	// 開始
	void EnterStartAnimation();

	// 更新
	void Update() override final;
	void UpdateSelectStage();
	void UpdateStartAnimation();
	void UpdateCamera();
	void UpdateRankingAlpha();

	// 描画
	void Draw() override final;
	void DrawScoreRanking();

private:
	// ステージデータ
	struct StageData
	{
		Vector3					cameraPos;		// カメラの位置
		std::string				stageNameId;	// ステージ名
		std::string				missionNameId;	// ミッション名
		std::string				difficultyId;	// 脅威度
		std::string				conditionsId;	// 達成条件
		std::shared_ptr<Planet> pPlanet;		// 惑星のポインタ
	};

private:
	// ステート
	enum class State
	{
		STAGE_SELECT,		// ステージ選択
		START_ANIMATION,	// スタート演出
	};

	// UIの種類
	enum class UI
	{
		EXPLANATION_WINDOW,	// 説明ウィンドウ

		// 説明ウィンドウの枠
		EXPLANATION_WINDOW_FRAME,



		LINE_3D,			// 3Dライン
		SCORE_RANKING,		// スコアランキング
		BUTTON,				// ボタン

		NUM
	};

private:
	// ステートマシン
	StateMachine<State> m_stateMachine;

	// ポインタ
	std::unique_ptr<Camera> m_pCamera;
	std::unique_ptr<PlanetManager> m_pPlanetManager;
	std::unique_ptr<SkyDome> m_pSkyDome;

	// ステージデータ
	std::map<Stage, StageData> m_stageData;

	// スコアランキング
	std::vector<ScoreSaveData> m_scoreRanking;

	// カメラ
	Vector3 m_cameraStartPos;		// カメラの初期位置
	Vector3 m_cameraGoalPos;		// カメラの目標位置
	Vector3 m_cameraStartTargetPos;	// カメラの初期注視点
	Vector3 m_cameraGoalTargetPos;	// カメラの目標注視点

	// イージングの時間
	float m_easeTime;
	float m_easeTime2;

	// 入力があるかフラグ
	bool m_isInput;

	// アルファ値
	int m_line3DAlpha;
	int m_windowAlpha;
	int m_textAlpha;
	std::vector<int> m_rankingAlpha;
	int m_uiAlpha;

	// スクリーンハンドル
	int m_screenHandle;

	// 画像ハンドル
	int m_rbButtonImgHandle;
	int m_lbButtonImgHandle;
	int m_bButtonImgHandle;
	int m_aButtonImgHandle;

	// ライトハンドル
	int m_lightHandle;

	// 選択されている項目
	int m_currentSelectItem;

	// 説明ウィンドウ
	Vector2 m_explanationWindowEasingTime;
	Vector2 m_explanationWindowSize;	// ウィンドウのサイズ
};

