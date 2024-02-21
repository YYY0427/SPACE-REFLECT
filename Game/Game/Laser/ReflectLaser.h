#pragma once
#include "LaserBase.h"
#include "../Shield.h"

// プロトタイプ宣言
class EnemyManager;

/// <summary>
/// 反射レーザー
/// </summary>
class ReflectLaser : public LaserBase
{
public:
	// コンストラクタ
	ReflectLaser(const std::shared_ptr<EnemyManager>& pEnemyManager, 
				 const std::shared_ptr<Shield>& pShield, 
				 const std::shared_ptr<LaserBase>& pLaser, 
				 const Vector3& firePos);

	// デストラクタ
	~ReflectLaser();

	// 更新
	void Update() override final;

	// 描画
	void Draw() override final;

private:
	// ポインタ
	std::shared_ptr<Shield> m_pShield;		// シールドのポインタ
	std::shared_ptr<LaserBase> m_pLaser;	// レーザーのポインタ
	std::shared_ptr<EnemyManager> m_pEnemyManager;	// 敵のポインタ

	// レーザーエフェクトのハンドル
	int m_laserEffectHandle;	

	Vector3 m_directionPos;
};
