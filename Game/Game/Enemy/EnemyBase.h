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
	// コンストラクタ
	EnemyBase();

	// デストラクタ
	virtual ~EnemyBase();

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

	// ダメージ処理
	virtual void OnDamage(int damage, Vector3 pos);

	// 死亡演出
	virtual void PerformDeathEffect() {};

	// ゲッター
	Vector3 GetPos() const;				// 位置情報
	Vector3 GetLaserFirePos() const;	// レーザーの発射位置
	bool IsEnabled() const;				// 存在フラグ
	float GetCollisionRadius() const;	// 当たり判定の半径

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