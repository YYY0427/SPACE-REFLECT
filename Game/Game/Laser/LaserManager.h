#pragma once
#include <memory>
#include <list>
#include <map>
#include "../../Math/Vector3.h"

// プロトタイプ宣言
class LaserBase;
class Player;
class EnemyBase;
class EnemyManager;
class Shield;

// レーザーの種類
enum class LaserType
{
	NORMAL,		// 通常レーザー
	REFLECT,	// 反射レーザー
	CUBE,		// キューブレーザー
	NUM
};

// レーザーのデータ
struct LaserData
{
	int key;		// レーザーのキー
	LaserType type;	// レーザーの種類
	std::shared_ptr<LaserBase> pLaser;	// レーザーのポインタ
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
	/// <param name="laserChargeFrame">レーザーのチャージフレーム</param>
	/// <param name="laserFireFrame">レーザーの発射時間フレーム</param>
	/// <param name="laserSpeed">レーザーの移動速度</param>
	/// <param name="isPlayerFollowing">プレイヤーを追従するか</param>
	int AddLaser(LaserType type, std::shared_ptr<EnemyBase> pEnemy, int laserChargeFrame, int laserFireFrame, float laserSpeed, bool isPlayerFollowing);

	// 反射レーザーの追加
	int AddReflectLaser(const std::shared_ptr<EnemyManager>& pEnemyManager,
						const std::shared_ptr<Shield>&    pShield,
						const std::shared_ptr<LaserBase>& pLaser, 
					    const Vector3& firePos);

	// キューブレーザーの追加
	int AddCubeLaser(Vector3 firePos, float laserSpeed);

	// レーザーの削除
	void DeleteLaser(int key);

	// レーザーの全削除
	void DeleteAllLaser();

	// ゲッター
	const std::list<LaserData>& GetLaserList() const;	// レーザーリスト
	const LaserData& GetLaserData(int key) const;		// レーザーデータ

	// セッター
	void SetLaserPosition(int key, Vector3 pos);		// レーザーの位置を設定

private:
	// ポインタ
	std::list<LaserData> m_pLaserList;	// レーザーリスト
	std::shared_ptr<Player> m_pPlayer;	// プレイヤーのポインタ
};

