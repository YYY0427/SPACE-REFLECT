#pragma once
#include "../EnemyBase.h"

/// <summary>
/// 蚊のボスクラス
/// </summary>
class BossMosquito final : public EnemyBase
{
public:
	// コンストラクタ
	BossMosquito(std::shared_ptr<Player> pPlayer, std::shared_ptr<LaserManager> pLaserManager);

	// デストラクタ
	~BossMosquito();

	// 更新
	void Update() override final;

	// 描画
	void Draw() override final;

private:
};