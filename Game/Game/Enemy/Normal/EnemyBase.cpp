#include "EnemyBase.h"
#include "../../../Effect/Effekseer3DEffectManager.h"
#include "../../../Model.h"

namespace
{
	// 死亡エフェクトの再生速度
	constexpr float dead_effect_play_speed = 0.5f;

	// 死亡エフェクトの拡大率
	const Vector3 dead_effect_scale = { 200.0f, 200.0f, 200.0f };
}

// コンストラクタ
EnemyBase::EnemyBase() :
	m_isEnabled(true),
	m_pos({ 0, 0, 0 }),
	m_rot({ 0, 0, 0 }),
	m_hp(0),
	m_opacity(0.0f),
	m_attackPower(0),
	m_moveSpeed(0.0f),
	m_deadEffectHandle(-1),
	m_movePointIndex(0),
	m_isGoal(false)
{
}

// デストラクタ
EnemyBase::~EnemyBase()
{
}

// ダメージ処理
void EnemyBase::OnDamage(int damage, Vector3 pos)
{
	// HPを減らす
	m_hp -= damage;

	// HPが0以下になったら死亡
	if (m_hp <= 0)
	{
		// 死亡エフェクトの再生
		Effekseer3DEffectManager::GetInstance().PlayEffect(
			m_deadEffectHandle,
			EffectID::enemy_died,
			m_pos,
			dead_effect_scale,
			dead_effect_play_speed);

		// 死亡フラグを立てる
		m_isEnabled = false;
	}
	else
	{
		// 全てのマテリアルのディフューズカラーを反転
		m_pModel->InversAllMaterialDifColor();
	}
}

// 位置の取得
Vector3 EnemyBase::GetPos() const
{
	return m_pos;
}

// レーザーの発射位置の取得
Vector3 EnemyBase::GetLaserFirePos() const
{
	return m_laserFirePos;
}

// 存在フラグの取得
bool EnemyBase::IsEnabled() const
{
	return m_isEnabled;
}