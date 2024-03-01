#pragma once
#include "LaserBase.h"
#include "../../StateMachine.h"
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
	/// <param name="pEnemy">発射元の敵のポインタ</param>
	/// <param name="pPlayer">プレイヤーのポインタ</param>
	/// <param name="laserChargeFrame">レーザーのチャージフレーム</param>
	/// <param name="laserFireFrame">レーザーの発射時間フレーム</param>
	/// <param name="laserSpeed">レーザーの移動速度</param>
	/// <param name="isPlayerFollowing">プレイヤーを追従するか</param>
	NormalLaser(const std::shared_ptr<EnemyBase>& pEnemy, 
				const std::shared_ptr<Player>& pPlayer, 
				const int laserChargeFrame, 
				const int laserFireFrame, 
				const float laserSpeed, 
				const bool isPlayerFollowing);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~NormalLaser();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override final;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override final;

	/// <summary>
	/// レーザーを止める
	/// </summary>
	/// <param name="pos">止める座標</param>
	void Stop(const Vector3& pos) override final;

	/// <summary>
	/// レーザーの反射
	/// </summary>
	/// <param name="pos">反射する座標</param>
	void Reflect(const Vector3& pos) override final;

	/// <summary>
	/// 反射を元に戻す
	/// </summary>
	void UndoReflect() override final;


	//// ゲッター ///////////////////////////////////////

	/// <summary>
	/// レーザーの向きを取得
	/// </summary>
	/// <returns>レーザーの向き</returns>
	Vector3 GetDirection() const override final;

private:
	//// 初期化 ///////////////////////////////////////

	/// <summary>
	/// レーザーのチャージの初期化
	/// </summary>
	void EnterCharge();					


	//// 更新 //////////////////////////////////////////

	/// <summary>
	/// レーザーのチャージの更新
	/// </summary>
	void UpdateCharge();		

	/// <summary>
	/// レーザーの通常発射の更新
	/// </summary>
	void UpdateNormalFire();	

	/// <summary>
	/// レーザーのプレイヤー追従発射の更新
	/// </summary>
	void UpdateFirePlayerFollowing();	

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
	std::shared_ptr<Player>    m_pPlayer;	// プレイヤーのポインタ

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
	int m_reflectFrame;			// 反射フレーム	

	// プレイヤーを追従するか
	bool m_isPlayerFollowing;

	// 通常発射時
	int     m_normalFireMovePointIndex;				// 移動ポイントインデックス
	Vector3 m_normalFireGoalPos;					// 移動ポイントの座標
	std::vector<Vector2> m_normalFireMovePointList;	// 移動ポイントの座標のリスト
};