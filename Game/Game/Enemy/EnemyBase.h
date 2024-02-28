#pragma once
#include "../../Math/Vector3.h"
#include "EnemyManager.h"
#include "../../StateMachine.h"
#include <memory>

// プロトタイプ宣言
class Model;
class Player;
class LaserManager;

/// <summary>
/// 敵の基底クラス
/// </summary>
class EnemyBase : public std::enable_shared_from_this<EnemyBase>
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EnemyBase();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// ダメージ処理
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	/// <param name="pos">ダメージを受けた場所</param>
	virtual void OnDamage(const int damage, const Vector3& pos);

	/// <summary>
	/// 死亡演出
	/// </summary>
	virtual void PerformDeathEffect() {};


	//// ゲッター ////

	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetPos() const;

	/// <summary>
	/// レーザーの発射位置を取得
	/// </summary>
	/// <returns>レーザーの発射位置</returns>
	const Vector3& GetLaserFirePos() const;	

	/// <summary>
	/// 存在フラグを取得
	/// </summary>
	/// <returns>true : 存在、false : 存在していない</returns>
	bool IsEnabled() const;			

	/// <summary>
	/// 当たり判定の半径を取得
	/// </summary>
	/// <returns>当たり判定の半径</returns>
	float GetCollisionRadius() const;	

	/// <summary>
	/// ボスの死亡演出中かどうか
	/// </summary>
	/// <returns></returns>
	virtual bool IsDeadAnim();

protected:
	// ポインタ
	std::shared_ptr<Model> m_pModel;
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<LaserManager> m_pLaserManager;

	// 行動データリスト
	std::vector<EnemyActionData> m_actionDataList;

	// 位置情報
	Vector3 m_pos;

	// 回転情報
	Vector3 m_rot;

	// 移動ベクトル
	Vector3 m_moveVec;

	// 目的地
	Vector3 m_goalPos;

	// レーザーの発射位置
	Vector3 m_laserFirePos;

	// 大きさ
	Vector3 m_scale;

	// 当たり判定の半径
	float m_collisionRadius;	

	// 移動速度
	float m_moveSpeed;

	// 不透明度
	float m_opacity;

	// 攻撃力
	int m_attackPower;

	// HP
	int m_hp;		

	// 存在フラグ
	bool m_isEnabled;

	// 現在の行動データのインデックス
	int m_movePointIndex;

	// 死亡エフェクトハンドル
	int m_deadEffectHandle;

	// 目的地に到達したかフラグ
	bool m_isGoal;
};