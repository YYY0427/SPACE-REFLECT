#pragma once
#include "EnemyBase.h"

/// <summary>
/// 雑魚敵クラス
/// </summary>
class Mosquito final : public EnemyBase
{
public:
	// コンストラクタ
	Mosquito(int modelHandle, EnemyData data, std::shared_ptr<Player> m_pPlayer);

	// デストラクタ
	~Mosquito();

	// 初期化
	void EntarMove();	// 移動

	// 更新
	void Update() override final;
	void UpdateIdle();		// 待機
	void UpdateMove();		// 移動
	void UpdateAttack();	// 攻撃
	void UpdateDead();		// 死亡

	// 描画
	void Draw() override final;

private:
	// 状態
	enum class State
	{
		IDLE,		// 待機
		MOVE,		// 移動
		ATTACK,		// 攻撃
		DEAD,		// 死亡
	};

private:
	// 状態マシン
	StateMachine<State> m_state;	
};

