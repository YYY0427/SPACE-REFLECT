#include "LaserManager.h"
#include "LaserBase.h"
#include "NormalLaser.h"
#include "../../Util/DrawFunctions.h"
#include "../Shield.h"
#include "ReflectLaser.h"
#include "CubeLaser.h"
#include "../Player.h"
#include "../Shield.h"
#include "../../Application.h"
#include <cassert>
#include <string>

// コンストラクタ
LaserManager::LaserManager(const std::shared_ptr<Player>& pPlayer, const std::shared_ptr<Camera>& pCamera) :
	m_pPlayer(pPlayer),
	m_pCamera(pCamera)
{
}

// デストラクタ
LaserManager::~LaserManager()
{
}

// 更新
void LaserManager::Update()
{
	// 存在フラグが下がっているのレーザーを削除
	m_pLaserList.remove_if([](const LaserData& laser) { return !laser.pLaser->IsEnabled(); });

	// レーザーの更新
	for (auto& laser : m_pLaserList)
	{
		laser.pLaser->Update();
	}
}

// 描画
void LaserManager::Draw()
{
	for (auto& laser : m_pLaserList)
	{
		laser.pLaser->Draw();
	}
}

// レーザーの追加
int LaserManager::AddLaser(LaserType type, 
						   const std::shared_ptr<EnemyBase>& pEnemy, 
						   const int laserChargeFrame, 
						   const int laserFireFrame, 
						   const float laserSpeed, 
						   const bool isPlayerFollowing)
{
	// レーザーのデータを作成
	LaserData laserData;

	// レーザーの種類を設定
	laserData.type = LaserType::NORMAL;

	// レーザーのインスタンスを作成
	laserData.pLaser = std::make_shared<NormalLaser>(
		pEnemy, m_pPlayer, laserChargeFrame, laserFireFrame, laserSpeed, isPlayerFollowing);

	// Keyの設定
	laserData.key = CreateLaserKey();

	// レーザーリストに追加
	m_pLaserList.push_back(laserData);

	// Keyを返す
	return laserData.key;
}

// 反射レーザーの追加
int LaserManager::AddReflectLaser(const std::shared_ptr<EnemyManager>& pEnemyManager, 
								  const std::shared_ptr<Shield>& pShield, 
								  const std::shared_ptr<LaserBase>& pLaser, 
								  const Vector3& firePos)
{
	// レーザーのデータを作成
	LaserData laserData;
	laserData.type = LaserType::REFLECT;

	// Keyの設定
	laserData.key = CreateLaserKey();

	// レーザーのポインタを設定
	laserData.pLaser = std::make_shared<ReflectLaser>(pEnemyManager, pShield, pLaser, firePos);

	// レーザーリストに追加
	m_pLaserList.push_back(laserData);

	return laserData.key;
}

// キューブレーザーの追加
int LaserManager::AddCubeLaser(const Vector3& firePos, const float laserSpeed)
{
	// レーザーのデータを作成
	LaserData laserData;
	laserData.type = LaserType::CUBE;

	// Keyの設定
	laserData.key = CreateLaserKey();

	// レーザーのポインタを設定
	laserData.pLaser = std::make_shared<CubeLaser>(firePos, laserSpeed, m_pPlayer, m_pCamera);

	// レーザーリストに追加
	m_pLaserList.push_back(laserData);

	// Keyを返す
	return laserData.key;
}

// レーザーの削除
void LaserManager::DeleteLaser(const int key)
{
	// キーと一致するレーザーを削除
	for (auto& laser : m_pLaserList)
	{
		if (laser.key == key)
		{
			laser.pLaser->Delete();
		}
	}
}

// 全てのレーザーの削除
void LaserManager::DeleteAllLaser()
{
	// レーザーの削除
	for (auto& laser : m_pLaserList)
	{
		laser.pLaser->Delete();
	}
	m_pLaserList.clear();
}

// レーザーの取得	
const std::list<LaserData>& LaserManager::GetLaserList() const
{
	return m_pLaserList;
}

// レーザーの取得
LaserData LaserManager::GetLaserData(const int key) const
{
	// キーと一致するレーザーを取得
	for (auto& laser : m_pLaserList)
	{
		if (laser.key == key)
		{
			return laser;
		}
	}

	// エラー
	assert(!"レーザーが見つかりませんでした");
	return m_pLaserList.front();
}

// レーザーの位置の設定
void LaserManager::SetLaserPos(const int key, const Vector3& pos)
{
	// キーと一致するレーザーに位置を設定
	for (auto& laser : m_pLaserList)
	{
		if (laser.key == key)
		{
			laser.pLaser->SetPos(pos);
		}
	}
}

// レーザーのキーの作成
int LaserManager::CreateLaserKey()
{
	// キーの重複を避けるために、最大のキーに+1をする
	int key = 0;
	for (auto& laser : m_pLaserList)
	{
		if (key <= laser.key)
		{
			key = laser.key + 1;
		}
	}
	return key;
}