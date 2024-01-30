#pragma once
#include "LaserBase.h"
#include "../Shield.h"

/// <summary>
/// 反射レーザー
/// </summary>
class ReflectLaser : public LaserBase
{
public:
	// コンストラクタ
	ReflectLaser(int modelHandle, std::shared_ptr<Shield> pShield, std::shared_ptr<LaserBase> pLaser, Vector3 firePos);

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

	// レーザーエフェクトのハンドル
	int m_laserEffectHandle;	

	Vector3 m_directionVec;
};
