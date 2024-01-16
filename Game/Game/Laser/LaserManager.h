#pragma once
#include <memory>
#include <list>
#include <map>

// プロトタイプ宣言
class LaserBase;
class Player;
class EnemyBase;

// レーザーの種類
enum class LaserType
{
	NORMAL,		// 通常レーザー
	REFLECT,	// 反射レーザー
	NUM
};

// レーザーのデータ
struct LaserData
{
	LaserType type;	// レーザーの種類
	std::shared_ptr<LaserBase> pLaser;	// 発射元のポインタ
};

/// <summary>
/// レーザーの管理クラス
/// </summary>
class LaserManager
{
public:
	// コンストラクタ
	LaserManager(std::shared_ptr<Player> pPlayer);

	// デストラクタ
	~LaserManager();

	// 更新
	void Update();

	// 描画
	void Draw();

	/// <summary>
	/// レーザーの追加
	/// </summary>
	/// <param name="type">レーザーの種類</param>
	/// <param name="pEnemy">発射元のポインタ</param>
	/// <param name="laserFireFrame">レーザーの発射時間フレーム</param>
	void AddLaser(LaserType type, std::shared_ptr<EnemyBase> pEnemy, int laserFireFrame, float laserSpeed, bool isPlayerFollowing);
	
	// ゲッター
	const std::list<LaserData>& GetLaserList() const;	// レーザーリスト

private:
	// ポインタ
	std::list<LaserData> m_pLaserList;	// レーザーリスト
	std::shared_ptr<Player> m_pPlayer;	// プレイヤーのポインタ

	// モデルハンドルテーブル
	std::map<LaserType, int> m_modelHandleTable;	
};

