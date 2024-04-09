#pragma once
#include "../EnemyBase.h"
#include <vector>

// プロトタイプ宣言
class LaserManager;
class Gauge;
class StringUI;
class Flash;
class Triangle;
class ScreenShaker;

/// <summary>
/// ボス敵
/// マトリックス
/// </summary>
class BossMatrix : public EnemyBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pPlayer">プレイヤーのポインタ</param>
	/// <param name="pLaserManager">レーザーマネージャーのポインタ</param>
	/// <param name="pScreenShaker">画面揺れのポインタ</param>
	BossMatrix(const std::shared_ptr<Player>& pPlayer, 
			   const std::shared_ptr<LaserManager>& pLaserManager,
			   const std::shared_ptr<ScreenShaker>& pScreenShaker);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BossMatrix();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override final;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override final;

	/// <summary>
	/// ダメージ処理
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	/// <param name="pos">ダメージを受けた位置</param>
	void OnDamage(const int damage, const Vector3& pos) override final;

private:

	//// ステートマシンの初期化 ////

	/// <summary>
	/// 移動しながらホーミングレーザー攻撃の初期化
	/// </summary>
	void EnterMoveHormingLaserAttack(); 

	/// <summary>
	/// キューブレーザー攻撃の初期化
	/// </summary>
	void EnterCubeLaserAttack();	

	/// <summary>
	/// 登場時の初期化
	/// </summary>
	void EnterDie();	


	//// ステートの更新 ////

	/// <summary>
	/// 登場時の更新
	/// </summary>
	void UpdateEntry();

	/// <summary>
	/// 待機時の更新
	/// </summary>
	void UpdateIdle();

	/// <summary>
	/// 死亡時の更新
	/// </summary>
	void UpdateDie();		

	/// <summary>
	/// ゲームオーバー時の更新
	/// </summary>
	void UpdateGameOver();

	/// <summary>
	/// 移動しながらホーミングレーザー攻撃の更新
	/// </summary>
	void UpdateMoveHomingLaserAttack();	

	/// <summary>
	/// キューブレーザー攻撃の更新
	/// </summary>
	void UpdateCubeLaserAttack();		


	//// 移動 ////

	/// <summary>
	/// 移動の初期化
	/// </summary>
	void InitMove();		

	/// <summary>
	/// 目標座標の設定
	/// </summary>
	void SetGoalPos();

	/// <summary>
	/// 初期位置に移動
	/// </summary>
	void MoveInitPos();	

	/// <summary>
	/// 移動
	/// </summary>
	void Move();


	//// 攻撃 ////

	/// <summary>
	/// 攻撃順序のシャッフル
	/// </summary>
	void ShuffleAttackState();

	/// <summary>
	/// 攻撃ステートの設定
	/// </summary>
	void SetAttackState();


	//// ゲッター ////

	/// <summary>
	/// 死亡時の演出中か
	/// </summary>
	/// <returns>true : 演出中、false : 演出中ではない</returns>
	bool IsDeadAnim() override;	

private:
	/// <summary>
	/// プレイヤーの方向を見る
	/// </summary>
	void LookPlayerDir();

private:
	// ステート
	enum class State
	{
		// 基本
		ENTRY,		// 登場
		IDLE,		// 待機
		BARIER,		// バリア
		DIE,		// 死亡
		GAME_OVER,	// ゲームオーバー

		// 攻撃
		MOVE_HOMING_LASER_ATTACK,	// 移動しながらホーミングレーザー攻撃
		CUBE_LASER_ATTACK,			// キューブレーザー攻撃
	};

private:
	// ステートマシン
	StateMachine<State> m_stateMachine;

	// ポインタ
	std::shared_ptr<Gauge>        m_pHpGauge;		// HPゲージ
	std::shared_ptr<StringUI>     m_pBossName;		// ボスの名前
	std::shared_ptr<LaserManager> m_pLaserManager;	// レーザーマネージャー
	std::shared_ptr<ScreenShaker> m_pScreenShaker;	// 画面揺れ
	std::shared_ptr<Flash>        m_pFlash;			// フラッシュ

	// 移動
	std::vector<Vector3> m_movePointTable;	// 移動ポイントテーブル
	int m_movePointIndex;					// 移動ポイントのインデックス
	Vector3 m_goalPos;						// 目標座標
	bool m_isGoal;							// 目標座標に到達したか
	bool m_isMoveEnd;						// 移動が終了したか

	// 攻撃ステート
	std::vector<State> m_attackStateTable;	// 攻撃ステートテーブル
	int m_attackStateIndex;					// 攻撃ステートのインデックス

	// フレーム
	int m_idleFrame;						// 待機フレーム
	int m_laserFrame;						// レーザー発射フレーム
	int m_cubeLaserIntervalFrame;			// キューブレーザーの発射間隔フレーム
	int m_dieIdleFrame;						// 死亡時の待機フレーム	
	int m_dieDrawStopFrame;					// 死亡時の描画を止めるフレーム

	// レーザーのKey
	int m_laserKey;

	// エフェクトハンドル
	int m_damageEffectHandle;
	int m_dieEffectHandle;

	// 徐々にプレイヤーの方向に向ける時に使用するラープの値
	int m_playerDirLerp;

	int  m_timer;
	bool m_isExplosionSound;
};