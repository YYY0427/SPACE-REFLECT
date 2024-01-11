#pragma once
#include <memory>
#include <list>

// プロトタイプ宣言
class EnemyBase;
class BossEnemyBase;

/// <summary>
/// 敵の管理クラス
/// </summary>
class EnemyManager
{
public:
	// コンストラクタ
	EnemyManager();

	// デストラクタ
	~EnemyManager();

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	// 敵のリスト
	std::list<std::shared_ptr<EnemyBase>> m_pEnemyList;

	// ボス敵
	std::shared_ptr<BossEnemyBase> m_pBossEnemy;
};