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
class Camera;

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
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pPlayer">プレイヤーのポインタ</param>
	/// <param name="pCamera">カメラのポインタ</param>
	LaserManager(const std::shared_ptr<Player>& pPlayer, const std::shared_ptr<Camera>& pCamera);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~LaserManager();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
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
	int AddLaser(LaserType type, 
				 const std::shared_ptr<EnemyBase>& pEnemy, 
				 const int laserChargeFrame, 
				 const int laserFireFrame, 
				 const float laserSpeed, 
				 const bool isPlayerFollowing);

	/// <summary>
	/// 反射レーザーの追加
	/// </summary>
	/// <param name="pEnemyManager">エネミー管理クラスのポインタ</param>
	/// <param name="pShield">シールドのポインタ</param>
	/// <param name="pLaser">レーザーのポインタ</param>
	/// <param name="firePos">発射位置</param>
	/// <returns>レーザーのキー</returns>
	int AddReflectLaser(const std::shared_ptr<EnemyManager>& pEnemyManager,
						const std::shared_ptr<Shield>& pShield,
						const std::shared_ptr<LaserBase>& pLaser, 
					    const Vector3& firePos);

	/// <summary>
	/// キューブレーザーの追加
	/// </summary>
	/// <param name="firePos">発射位置</param>
	/// <param name="laserSpeed">レーザーの移動速度</param>
	/// <returns>レーザーのキー</returns>
	int AddCubeLaser(const Vector3& firePos, const float laserSpeed);

	/// <summary>
	/// レーザーの削除
	/// </summary>
	/// <param name="key">削除するレーザーのキー</param>
	void DeleteLaser(const int key);

	/// <summary>
	/// 全てのレーザーの削除
	/// </summary>
	void DeleteAllLaser();


	//// ゲッター /////

	/// <summary>
	/// レーザーリストの取得
	/// </summary>
	/// <returns></returns>
	const std::list<LaserData>& GetLaserList() const;	// レーザーリスト

	/// <summary>
	/// レーザーデータの取得
	/// </summary>
	/// <param name="key">レーザーのキー</param>
	/// <returns>レーザーデータ</returns>
	LaserData GetLaserData(const int key) const;

	///// セッター /////

	/// <summary>
	/// レーザーの位置を設定
	/// </summary>
	/// <param name="key">レーザーのキー</param>
	/// <param name="pos">位置情報</param>
	void SetLaserPos(const int key, const Vector3& pos);

private:
	/// <summary>
	/// レーザーキーの作成
	/// </summary>
	/// <returns>レーザーキー</returns>
	int CreateLaserKey();	

private:
	// ポインタ
	std::shared_ptr<Player> m_pPlayer;   	// プレイヤーのポインタ
	std::shared_ptr<Camera> m_pCamera;		// カメラのポインタ

	// レーザーリスト
	std::list<LaserData>    m_pLaserList;	
};

