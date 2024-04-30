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

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override final;

	/// <summary>
	/// 終了処理
	/// </summary>
	void End() override final;

	/// <summary>
	/// ステージを切り替え時の処理 
	/// </summary>
	void SelectStageProcess();

	/// <summary>
	/// スタート演出の開始
	/// </summary>
	void EnterStartAnimation();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override final;

	/// <summary>
	/// ステージ選択の更新
	/// </summary>
	void UpdateSelectStage();

	/// <summary>
	/// スタート演出の更新
	/// </summary>
	void UpdateStartAnimation();

	/// <summary>
	/// カメラの更新
	/// </summary>
	void UpdateCamera();

	/// <summary>
	/// ランキングのアルファ値の更新
	/// </summary>
	void UpdateRankingAlpha();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override final;

	/// <summary>
	/// スコアランキングの描画
	/// </summary>
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
		EXPLANATION_WINDOW,			// 説明ウィンドウ
		EXPLANATION_WINDOW_FRAME,	// 説明ウィンドウの枠
		LINE_3D,					// 3Dライン
		SCORE_RANKING,				// スコアランキング
		BUTTON,						// ボタン
		NUM							// 項目数
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
	int m_easeTime;
	int m_easeTime2;

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
	Vector2 m_explanationWindowEasingTime;	// ウィンドウのイージング時間
	Vector2 m_explanationWindowSize;		// ウィンドウのサイズ

	// スタート演出のSEの再生フラグ
	bool m_isStartAnimSE;	

	// ステージ選択時にカメラを移動するイージング時間
	int m_selectStageCameraEasingTime;

	// ステージ決定時のカメラの注視点のイージング時間
	int m_decisionStageTargetEasingTime;

	// ステージ決定時にカメラを移動するイージング時間
	int m_decisionStageCameraEasingTime;
};