#pragma once
#include "../../Math/Vector3.h"
#include <memory>

// プロトタイプ宣言
class Model;

/// <summary>
/// 敵の基底クラス
/// </summary>
class EnemyBase
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
	void OnDamage(int damage, Vector3 pos);

	// ゲッター
	Vector3 GetPos() const;		// 位置情報
	bool IsEnabled() const;		// 存在フラグ

protected:
	// ポインタ
	std::shared_ptr<Model> m_pModel;

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

	// 死亡エフェクトハンドル
	int m_deadEffectHandle;
};