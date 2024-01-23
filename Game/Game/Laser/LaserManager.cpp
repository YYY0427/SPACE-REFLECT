#include "LaserManager.h"
#include "LaserBase.h"
#include "NormalLaser.h"
#include "../../Util/DrawFunctions.h"
#include "../Shield.h"
#include "ReflectLaser.h"
#include "../Player.h"
#include "../Shield.h"
#include <cassert>
#include <string>

namespace
{
	// 通常レーザーのモデルファイルパス
	const std::string normal_laser_model_file_path = "Data/Model/Laser.mv1";	
} 

// コンストラクタ
LaserManager::LaserManager(std::shared_ptr<Player> pPlayer) :
	m_pPlayer(pPlayer)
{
	// モデルの読み込み
	m_modelHandleTable[LaserType::NORMAL] = my::MyLoadModel(normal_laser_model_file_path.c_str());
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

	if (m_pPlayer->GetShield())
	{
		if (!m_pPlayer->GetShield()->IsShield())
		{
			// 反射レーザーの削除
			m_pLaserList.remove_if([](const LaserData& laser) { return laser.type == LaserType::REFLECT; });
		}
	}

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
void LaserManager::AddReflectLaser(std::shared_ptr<Shield> pShield, std::shared_ptr<LaserBase> pLaser)
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
	laserData.pLaser = std::make_shared<ReflectLaser>(m_modelHandleTable[LaserType::NORMAL], pShield, pLaser);

	// レーザーリストに追加
	m_pLaserList.push_back(laserData);
}

// レーザーの削除
void LaserManager::DeleteLaser(int key)
{
	// レーザーの削除
	m_pLaserList.remove_if([key](const LaserData& laser) { return laser.key == key; });
}

// レーザーの取得	
const std::list<LaserData>& LaserManager::GetLaserList() const
{
	return m_pLaserList;
}