#pragma once
#include "BossEnemyBase.h"
#include "../Normal/EnemyBase.h"
#include <vector>

// プロトタイプ宣言
class LaserManager;
class Gauge;
class StringUI;

/// <summary>
/// ボス敵
/// マトリックス
/// </summary>
class BossMatrix : public BossEnemyBase
{
public:
	// コンストラクタ
	BossMatrix(int modelHandle, 
			   std::shared_ptr<Player> pPlayer, 
			   std::shared_ptr<LaserManager> pLaserManager);

	// デストラクタ
	~BossMatrix();

	// 更新
	void Update() override final;

	// 描画
	void Draw() override final;

private:
	// 開始
	void EntarStopNormalLaserAttack();	// 通常レーザー攻撃

	// 更新
	void UpdateEntry();		// 登場時の更新
	void UpdateIdle();		// 待機時の更新
	void UpdateDie();		// 死亡時の更新
	void UpdateMoveNormalLaserAttack();	// 移動しながら通常レーザー攻撃
	void UpdateMoveHomingLaserAttack();	// 移動しながらホーミングレーザー攻撃
	void UpdateStopNormalLaserAttack();	// 通常レーザー攻撃
	void UpdateStopHomingLaserAttack();	// ホーミングレーザー攻撃
	void UpdateCubeLaserAttack();		// キューブレーザー攻撃

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
		STOP_HOMING_LASER_ATTACK,	// ホーミングレーザー攻撃
		CUBE_LASER_ATTACK,			// キューブレーザー攻撃
	};

private:
	// ステートマシン
	StateMachine<State> m_stateMachine;

	// ポインタ
	std::shared_ptr<Gauge> m_pHpGauge;
	std::shared_ptr<StringUI> m_pBossName;
	std::shared_ptr<LaserManager> m_pLaserManager;

	// 移動
	std::vector<Vector3> m_movePointTable;	// 移動ポイントテーブル
	int m_movePointIndex;					// 移動ポイントのインデックス
	Vector3 m_goalPos;						// 目標座標
	bool m_isGoal;							// 目標座標に到達したか
	bool m_isMoveEnd;						// 移動が終了したか

	// 攻撃ステート
	std::vector<State> m_attackStateTable;	//	攻撃ステートテーブル
	int m_attackStateIndex;					// 攻撃ステートのインデックス

	// 待機
	int m_idleFrame;						// 待機フレーム
};