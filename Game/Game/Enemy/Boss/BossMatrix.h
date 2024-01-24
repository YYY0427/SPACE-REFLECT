#pragma once
#include "BossEnemyBase.h"
#include <vector>

// プロトタイプ宣言
class LaserManager;
class Gauge;

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
	void UpdateEntry();		// 登場時の更新
	void UpdateIdle();		// 待機時の更新
	void UpdateMove();		// 移動時の更新
	void UpdateDie();		// 死亡時の更新
	void UpdateMoveNormalLaserAttack();	// 移動しながら通常レーザー攻撃
	void UpdateMoveHomingLaserAttack();	// 移動しながらホーミングレーザー攻撃
	void UpdateCubeLaserAttack();	// キューブレーザー攻撃

	// 攻撃ステートの順序をシャッフル
	void ShuffleAttackState();

private:
	// ステート
	enum class State
	{
		ENTRY,		// 登場
		IDLE,		// 待機
		MOVE,		// 移動
		DIE,		// 死亡

		// 攻撃
		MOVE_NORMAL_LASER_ATTACK,	// 移動しながら通常レーザー攻撃
		MOVE_HOMING_LASER_ATTACK,	// 移動しながらホーミングレーザー攻撃
		CUBE_LASER_ATTACK,			// キューブレーザー攻撃
	};

private:
	// ステートマシン
	StateMachine<State> m_stateMachine;

	// ポインタ
	std::unique_ptr<Gauge> m_pHpGauge;

	// 移動ポイントテーブル
	std::vector<Vector3> m_movePointTable;

	// 攻撃ステート
	std::vector<State> m_attackStateTable;	//	攻撃ステートテーブル
	int m_attackStateIndex;					// 攻撃ステートのインデックス
};