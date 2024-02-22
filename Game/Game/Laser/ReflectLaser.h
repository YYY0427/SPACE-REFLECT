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
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pEnemyManager">pEnemymanagerのポインタ</param>
	/// <param name="pShield">pShieldのポインタ</param>
	/// <param name="pLaser">pLaserのポインタ</param>
	/// <param name="firePos">発射位置</param>
	ReflectLaser(const std::shared_ptr<EnemyManager>& pEnemyManager, 
				 const std::shared_ptr<Shield>& pShield, 
				 const std::shared_ptr<LaserBase>& pLaser, 
				 const Vector3& firePos);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ReflectLaser();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override final;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override final;

private:
	// ポインタ
	std::shared_ptr<Shield>       m_pShield;		// シールドのポインタ
	std::shared_ptr<LaserBase>    m_pLaser;			// レーザーのポインタ
	std::shared_ptr<EnemyManager> m_pEnemyManager;	// 敵のポインタ

	// レーザーエフェクトのハンドル
	int m_laserEffectHandle;	

	// レーザーの向いている方向の座標
	Vector3 m_directionPos;
};
