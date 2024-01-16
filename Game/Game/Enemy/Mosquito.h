#pragma once
#include "EnemyBase.h"
#include "../../Util/Timer.h"
#include "../../Game/Laser/LaserManager.h"

/// <summary>
/// 雑魚敵クラス
/// </summary>
class Mosquito final : public EnemyBase
{
public:
	// コンストラクタ
	Mosquito(int modelHandle, EnemyData data, std::shared_ptr<Player> pPlayer, std::shared_ptr<LaserManager> pLaserManager);

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

	// 待機時間フレーム
	int m_idleFrame;		

	// レーザー
	LaserType m_laserType;		// レーザーの種類
	int m_laserFireIdleFrame;	// レーザー発射までの待機時間
	int m_laserFireFrame;		// レーザー発射時間
	float m_laserSpeed;			// レーザーの速度
};

