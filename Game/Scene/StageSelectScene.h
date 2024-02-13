#pragma once
#include "SceneBase.h"
#include "../Score/ScoreRanking.h"
#include "../Math/Vector3.h"
#include "../Game/Planet.h"
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

	// デストラクタ
	~StageSelectScene();

	// 更新
	void Update() override final;
	void UpdateCamera();

	// 描画
	void Draw() override final;
	void DrawScoreRanking();

private:
	// ステージデータ
	struct StageData
	{
		// ステージ名
		std::string stageName;

		// カメラの位置
		Vector3 cameraPos;

		// 惑星のポインタ
		std::shared_ptr<Planet> pPlanet;

		// ミッション名
		std::string missionName;

		// 脅威度
		std::string difficultyId;

		// 達成条件
		std::string conditionsId;
	};

private:
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
	Vector3 m_cameraGoalTargetPos;	// カメラの目標注視点

	// イージングの時間
	float m_easeTime;	

	// 入力があるかフラグ
	bool m_isInput;

	// アルファ値
	int m_line3DAlpha;
	int m_windowAlpha;
	int m_textAlpa;
	std::vector<int> m_rankingAlpha;

	// 画像ハンドル
	int m_rbButtonImgHandle;
	int m_lbButtonImgHandle;
	int m_bButtonImgHandle;
	int m_aButtonImgHandle;

	// 選択されている項目
	int m_currentSelectItem;
};

