#include "LaserManager.h"
#include "LaserBase.h"
#include "NormalLaser.h"
#include "../../Util/DrawFunctions.h"
#include "../Shield.h"
#include "ReflectLaser.h"
#include "CubeLaser.h"
#include "../Player.h"
#include "../Shield.h"
#include <cassert>
#include <string>

namespace
{
	// 通常レーザーのモデルファイルパス
	const std::string normal_laser_model_file_path = "Data/Model/Laser.mv1";

	// キューブレーザーのモデルファイルパス
	const std::string cube_laser_model_file_path = "Data/Model/CubeLaser.mv1";
} 

// コンストラクタ
LaserManager::LaserManager(std::shared_ptr<Player> pPlayer) :
	m_pPlayer(pPlayer)
{
	// モデルの読み込み
	m_modelHandleTable[LaserType::NORMAL] = my::MyLoadModel(normal_laser_model_file_path.c_str());
	m_modelHandleTable[LaserType::CUBE] = my::MyLoadModel(cube_laser_model_file_path.c_str());
}

// デストラクタ
LaserManager::~LaserManager()
{
}

// 更新
void LaserManager::Update()
{
	// 不要になったレーザーの削除
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
int LaserManager::AddLaser(LaserType type, std::shared_ptr<EnemyBase> pEnemy, int laserChargeFrame, int laserFireFrame, float laserSpeed, bool isPlayerFollowing)
{
	LaserData laserData;
	laserData.type = type;

	// レーザーの種類によって処理を分岐
	switch (laserData.type)
	{
	case LaserType::NORMAL:
		laserData.pLaser = std::make_shared<NormalLaser>(
			m_modelHandleTable[LaserType::NORMAL],
			pEnemy, m_pPlayer, laserChargeFrame, laserFireFrame, laserSpeed, isPlayerFollowing);
		break;

	default:
		assert(!"レーザーの種類がありません");
	}

	// Keyの設定
	laserData.key = 0;
	for (auto& laser : m_pLaserList)
	{
		if (laserData.key <= laser.key)
		{
			laserData.key = laser.key + 1;
		}
	}

	// レーザーリストに追加
	m_pLaserList.push_back(laserData);

	// Keyを返す
	return laserData.key;
}

// 反射レーザーの追加
int LaserManager::AddReflectLaser(std::shared_ptr<Shield> pShield, std::shared_ptr<LaserBase> pLaser, Vector3 firePos)
{
	// レーザーのデータを作成
	LaserData laserData;
	laserData.type = LaserType::REFLECT;

	// Keyの設定
	laserData.key = 0;
	for (auto& laser : m_pLaserList)
	{
		if (laserData.key <= laser.key)
		{
			laserData.key = laser.key + 1;
		}
	}

	// レーザーのポインタを設定
	laserData.pLaser = std::make_shared<ReflectLaser>(m_modelHandleTable[LaserType::NORMAL], pShield, pLaser, firePos);

	// レーザーリストに追加
	m_pLaserList.push_back(laserData);

	return laserData.key;
}

// キューブレーザーの追加
int LaserManager::AddCubeLaser(Vector3 firePos)
{
	// レーザーのデータを作成
	LaserData laserData;
	laserData.type = LaserType::CUBE;

	// Keyの設定
	laserData.key = 0;
	for (auto& laser : m_pLaserList)
	{
		if (laserData.key <= laser.key)
		{
			laserData.key = laser.key + 1;
		}
	}

	// レーザーのポインタを設定
	laserData.pLaser = std::make_shared<CubeLaser>(m_modelHandleTable[LaserType::CUBE], firePos, m_pPlayer);

	// レーザーリストに追加
	m_pLaserList.push_back(laserData);

	// Keyを返す
	return laserData.key;
}

// レーザーの削除
void LaserManager::DeleteLaser(int key)
{
	// レーザーの削除
	for (auto& laser : m_pLaserList)
	{
		if (laser.key == key)
		{
			laser.pLaser->Delete();
		}
	}
}

// レーザーの取得	
const std::list<LaserData>& LaserManager::GetLaserList() const
{
	return m_pLaserList;
}

// レーザーの取得
const LaserData& LaserManager::GetLaserData(int key) const
{
	for (auto& laser : m_pLaserList)
	{
		if (laser.key == key)
		{
			return laser;
		}
	}

	assert(!"レーザーが見つかりませんでした");
	return m_pLaserList.front();
}

// レーザーの位置の設定
void LaserManager::SetLaserPosition(int key, Vector3 pos)
{
	for (auto& laser : m_pLaserList)
	{
		if (laser.key == key)
		{
			laser.pLaser->SetPos(pos);
		}
	}
}
