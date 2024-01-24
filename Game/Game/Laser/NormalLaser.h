#pragma once
#include "LaserBase.h"
#include "../../StateMachine.h"
#include "../../Util/Timer.h"
#include "../../Math/Matrix.h"
#include "../../Math/Vector2.h"
#include <vector>

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
	/// <param name="laserChargeFrame">レーザーのチャージフレーム</param>
	/// <param name="laserFireFrame">レーザーの発射時間フレーム</param>
	/// <param name="laserSpeed">レーザーの移動速度</param>
	/// <param name="isPlayerFollowing">プレイヤーを追従するか</param>
	NormalLaser(int modelHandle, std::shared_ptr<EnemyBase> pEnemy, std::shared_ptr<Player> pPlayer, int laserChargeFrame, int laserFireFrame, float laserSpeed, bool isPlayerFollowing);

	// デストラクタ
	~NormalLaser();

	// 更新
	void Update() override final;

	// 描画
	void Draw() override final;

	// レーザーを止める
	void Stop(Vector3 pos) override final;

	// 反射された状態から元に戻す
	void UndoReflect() override final;

	// 方向ベクトルのゲッター
	Vector3 GetDirection() const override final;

private:
	// 更新
	void UpdateCharge();				// チャージ
	void UpdateNormalFire();			// 通常発射
	void UpdateFirePlayerFollowing();	// プレイヤーを追従して発射

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

	// 向く方向
	Vector3 m_directionPos;	// 向く座標
	Vector3 m_directionVec; // ベクトル

	// 状態マシン
	StateMachine<State> m_stateMachine;	

	// 回転行列
	Matrix m_rotMtx;	

	// レーザーのエフェクトのハンドル
	int m_laserEffectHandle;

	// フレーム
	int m_laserFireFrame;		// レーザーの発射フレーム
	int m_chargeEffectFrame;	// エフェクトチャージフレーム

	// プレイヤーを追従するか
	bool m_isPlayerFollowing;

	// 通常発射時
	int m_normalFireMovePointIndex;		// 移動ポイントインデックス
	Vector3 m_normalFireGoalPos;		// 移動ポイントの座標
	std::vector<Vector2> m_normalFireMovePointList;	// 移動ポイントの座標のリスト
};