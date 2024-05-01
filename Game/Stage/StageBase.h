#pragma once
#include "../Scene/SceneBase.h"
#include <memory>
#include <string>

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
class ResultWindow;
class Fade;

/// <summary>
/// ステージ基底クラス
/// </summary>
class StageBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	StageBase(SceneManager& manager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~StageBase();

	/// <summary>
	/// 更新
	/// 純粋仮想関数
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// リザルトの更新
	/// </summary>
	/// <param name="stageName">ステージ名</param>
	virtual void UpdateResult(const std::string& stageName);

	/// <summary>
	/// 描画
	/// 純粋仮想関数
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 当たり判定
	/// </summary>
	void Collision();

protected:
	// シーンマネージャー
	SceneManager& m_manager;

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
	std::shared_ptr<ResultWindow> m_pResultWindow;
	std::unique_ptr<Fade> m_pFade;

	// 現在のフレーム
	int m_currentFrame;

	// ウェーブがスタートしたかのフラグ
	bool m_isWaveStart;

	// ライトハンドル
	int m_directionalLightHandle;

	// 隕石がプレイヤーに与えるダメージ
	int m_meteorDamage;

	// レーザーがプレイヤーに与えるダメージ
	int m_laserDamage;

	// プレイヤーが反射レーザーでボス敵に与えるダメージ
	int m_playerToBossDamage;

	// プレイヤーが反射レーザーで敵に与えるダメージ
	int m_playerToEnemyDamage;

	// プレイヤーと敵が当たった時のプレイヤーに与えるダメージ
	int m_enemyDamage;
};