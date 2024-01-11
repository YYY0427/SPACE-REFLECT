#pragma once
#include "EnemyBase.h"

/// <summary>
/// 雑魚敵クラス
/// </summary>
class Mosquito final : public EnemyBase
{
public:
	// コンストラクタ
	Mosquito(int modelHandle);

	// デストラクタ
	~Mosquito();

	// 更新
	void Update() override final;

	// 描画
	void Draw() override final;

private:
};

