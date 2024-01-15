#pragma once
#include "LaserBase.h"
#include "../../StateMachine.h"
#include "../../Util/Timer.h"
#include "../../Math/Matrix.h"

// プロトタイプ宣言
class EnemyBase;
class Player;

/// <summary>
/// 通常レーザークラス
/// </summary>
class NormalLaser final : public LaserBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="modelHandle">モデルハンドル</param>
	/// <param name="pEnemy">発射元の敵のポインタ</param>
	/// <param name="pPlayer">プレイヤーのポインタ</param>
	/// <param name="laserFireFrame">レーザーの発射時間フレーム</param>
	/// <param name="laserSpeed">レーザーの移動速度</param>
	/// <param name="isPlayerFollowing">プレイヤーを追従するか</param>
	NormalLaser(int modelHandle, std::shared_ptr<EnemyBase> pEnemy, std::shared_ptr<Player> pPlayer, int laserFireFrame, float laserSpeed, bool isPlayerFollowing);

	// デストラクタ
	~NormalLaser();

	// 更新
	void Update() override final;
	void UpdateCharge();
	void UpdateNormalFire();
	void UpdateFirePlayerFollowing();

	// 描画
	void Draw() override final;

private:
	// 状態
	enum class State
	{
		CHARGE,					// チャージ
		FIRE_PLYER_FOLLOWING,	// プレイヤーを追従して発射
		NORMAL_FIRE,			// 通常発射
		NUM
	};

private:
	// ポインタ
	std::shared_ptr<EnemyBase> m_pEnemy;	// 発射元のポインタ
	std::shared_ptr<Player> m_pPlayer;		// プレイヤーのポインタ

	// 状態マシン
	StateMachine<State> m_stateMachine;	

	// 回転行列
	Matrix m_rotMtx;	

	// レーザーのエフェクトのハンドル
	int m_laserEffectHandle;

	// レーザーの発射時間フレーム
	int m_laserFireFrame;	

	// タイマー
	Timer<int> m_chargeEffectTimer;	// チャージエフェクトのタイマー

	// プレイヤーを追従するか
	bool m_isPlayerFollowing;	
};