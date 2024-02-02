#pragma once
#include "../Scene/SceneManager.h"
#include "StageBase.h"
#include "../StateMachine.h"
#include <string>
#include <memory>

// プロトタイプ宣言
class UIManager;
class Player;
class Camera;
class SkyDome;
class MeteorManager;
class DamageFlash;
class ScreenShaker;
class PlanetManager;
class LaserManager;
class EnemyManager;

/// <summary>
/// ステージ1
/// </summary>
class Stage1 final : public StageBase
{
public:
	// コンストラクタ
	Stage1(SceneManager& manager);

	// デストラクタ
	~Stage1();

	// 更新
	void Update() override final;
	void UpdateStartAnimation();
	void UpdatePlay();
	void UpdateGameClear();
	void UpdateResult();

	// 描画
	void Draw() override final;

	// 当たり判定
	void Collision();

private:
	// 状態
	enum class State
	{
		START_ANIMATION,	// スタート演出
		PLAY,				// プレイ中
		GAME_OVER,			// ゲームオーバー
		GAME_CLEAR,			// ゲームクリア
		RESULT,				// リザルト
		NUM					// 状態の数
	};

private:
	// ステートマシン
	StateMachine<State> m_stateMachine;

	// ポインタ
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<SkyDome> m_pSkyDome;
	std::shared_ptr<MeteorManager> m_pMeteorManager;
	std::shared_ptr<DamageFlash> m_pDamageFlash;
	std::shared_ptr<ScreenShaker> m_pScreenShaker;
	std::shared_ptr<PlanetManager> m_pPlanetManager;
	std::shared_ptr<LaserManager> m_pLaserManager;
	std::shared_ptr<EnemyManager> m_pEnemyManager;
};