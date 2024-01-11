#include "BossEnemyBase.h"
#include "../../Model.h"
#include "../../Effect/Effekseer3DEffectManager.h"

namespace
{
	// ダメージエフェクトの再生速度
	constexpr float damage_effect_play_speed = 1.0f;

	// ダメージエフェクトの拡大率
	const Vector3 damage_effect_scale = { 200.0f, 200.0f, 200.0f };
}

// コンストラクタ
BossEnemyBase::BossEnemyBase() :
	m_isEnabled(true),
	m_pos({ 0, 0, 0 }),
	m_rot({ 0, 0, 0 }),
	m_hp(0),
	m_opacity(0.0f),
	m_damageEffectHandle(-1)
{
}

// デストラクタ
BossEnemyBase::~BossEnemyBase()
{
}

// ダメージ処理
void BossEnemyBase::OnDamage(int damage, Vector3 pos)
{
	// HPを減らす
	m_hp -= damage;

	// モデルのディフューズカラーを反転
	m_pModel->InversAllMaterialDifColor();

	// ダメージエフェクトを再生
	Effekseer3DEffectManager::GetInstance().PlayEffect(
		m_damageEffectHandle,
		EffectID::enemy_boss_hit_effect,
		pos,
		damage_effect_scale,
		damage_effect_play_speed);

	// HPが0以下になったら
	if (m_hp <= 0)
	{
		// 死亡フラグを立てる
		m_isEnabled = false;
	}
}

// 位置の取得
Vector3 BossEnemyBase::GetPos() const
{
	return m_pos;
}

// 存在フラグの取得
bool BossEnemyBase::IsEnabled() const
{
	return m_isEnabled;
}
