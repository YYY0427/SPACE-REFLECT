#pragma once
#include "BossEnemyBase.h"

// プロトタイプ宣言
class LaserManager;

/// <summary>
/// ボス敵
/// マトリックス
/// </summary>
class BossMatrix : public BossEnemyBase
{
public:
	// コンストラクタ
	BossMatrix(int modelHandle, 
			   std::shared_ptr<Player> pPlayer, 
			   std::shared_ptr<LaserManager> pLaserManager);

	// デストラクタ
	~BossMatrix();

	// 更新
	void Update() override final;

	// 描画
	void Draw() override final;
};