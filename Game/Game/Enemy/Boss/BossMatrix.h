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
	// コンストラクタ
	BossMatrix(int modelHandle, 
			   std::shared_ptr<Player> pPlayer, 
			   std::shared_ptr<LaserManager> pLaserManager,
			   std::shared_ptr<ScreenShaker> pScreenShaker);

	// デストラクタ
	~BossMatrix();

	// 更新
	void Update() override final;

	// 描画
	void Draw() override final;

	// ダメージ処理
	void OnDamage(int damage, Vector3 pos) override final;

	// 死亡演出
	void PerformDeathEffect() override final;

private:
	// 開始
	void EntarStopNormalLaserAttack();	// 通常レーザー攻撃
	void EntarMoveNormalLaserAttack();	// 移動しながら通常レーザー攻撃
	void EntarDie();					// 死亡

	// 更新
	void UpdateEntry();		// 登場時の更新
	void UpdateIdle();		// 待機時の更新
	void UpdateDie();		// 死亡時の更新
	void UpdateMoveNormalLaserAttack();	// 移動しながら通常レーザー攻撃
	void UpdateMoveHomingLaserAttack();	// 移動しながらホーミングレーザー攻撃
	void UpdateStopNormalLaserAttack();	// 通常レーザー攻撃
	void UpdateCubeLaserAttack();		// キューブレーザー攻撃

	// 移動
	void InitMove();		// 移動の初期化
	void SetGoalPos();		// 目標座標の設定
	void MoveInitPos();		// 初期位置に移動
	void Move();			// 移動

	// 攻撃ステートの順序をシャッフル
	void ShuffleAttackState();

	// 攻撃ステートの設定
	void SetAttackState();

private:
	// ステート
	enum class State
	{
		ENTRY,		// 登場
		IDLE,		// 待機
		BARIER,		// バリア
		DIE,		// 死亡

		// 攻撃
		MOVE_NORMAL_LASER_ATTACK,	// 移動しながら通常レーザー攻撃
		MOVE_HOMING_LASER_ATTACK,	// 移動しながらホーミングレーザー攻撃
		STOP_NORMAL_LASER_ATTACK,	// 通常レーザー攻撃
		CUBE_LASER_ATTACK,			// キューブレーザー攻撃
	};

private:
	// 死亡時のエフェクトのデータ
	struct DieEffectData
	{
		int effectHandle;
		Vector3 pos;
		Vector3 vec;
		float scale;
	};

private:
	// ステートマシン
	StateMachine<State> m_stateMachine;

	// ポインタ
	std::shared_ptr<Gauge> m_pHpGauge;
	std::shared_ptr<StringUI> m_pBossName;
	std::shared_ptr<LaserManager> m_pLaserManager;
	std::shared_ptr<ScreenShaker> m_pScreenShaker;
	std::unique_ptr<Flash> m_pFlash;
	std::unique_ptr<Triangle> m_pTriangle;

	// 死亡時のエフェクトテーブル
	std::vector<DieEffectData> m_dieEffectTable;

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
	int m_dieIdleFrame;						// 死亡時の待機フレーム	
	int m_dieShakeFrame;					// 死亡時の横揺れ演出を行うフレーム
	int m_dieEffectIntervalFrame;			// 死亡時のエフェクトの発生間隔
	int m_dieDrawStopFrame;					// 死亡時の描画を止めるフレーム

	// レーザーのKey
	int m_laserKey;

	// エフェクトハンドル
	int m_damageEffectHandle;
	int m_dieEffectHandle;
};