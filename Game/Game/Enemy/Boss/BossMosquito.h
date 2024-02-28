#pragma once
#include "../EnemyBase.h"

/// <summary>
/// 蚊のボスクラス
/// </summary>
class BossMosquito final : public EnemyBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pPlayer">プレイヤーのポインタ</param>
	/// <param name="pLaserManager">レーザーのマネージャーのポインタ</param>
	BossMosquito(const std::shared_ptr<Player>& pPlayer, 
				 const std::shared_ptr<LaserManager>& pLaserManager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BossMosquito();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override final;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override final;

private:
};