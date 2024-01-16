#pragma once
#include "../../../Math/Vector3.h"
#include <memory>

// プロトタイプ宣言
class Model;
class Player;
class LaserManager;

/// <summary>
/// ボス敵の基底クラス
/// </summary>
class BossEnemyBase
{
public:
	// コンストラクタ
	BossEnemyBase();

	// デストラクタ
	virtual ~BossEnemyBase();

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

	// ダメージ処理
	void OnDamage(int damage, Vector3 pos);

	// ゲッター
	Vector3 GetPos() const;		// 位置情報
	bool IsEnabled() const;		// 存在フラグ

protected:
	// ポインタ
	std::shared_ptr<Model> m_pModel;
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<LaserManager> m_pLaserManager;

	// 位置情報
	Vector3 m_pos;

	// 回転情報
	Vector3 m_rot;

	// 移動ベクトル
	Vector3 m_moveVec;

	// 不透明度
	float m_opacity;

	// HP
	int m_hp;

	// 存在フラグ
	bool m_isEnabled;

	// ダメージエフェクトのハンドル
	int m_damageEffectHandle;
};