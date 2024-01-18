#include "LaserManager.h"
#include "LaserBase.h"
#include "NormalLaser.h"
#include "../../Util/DrawFunctions.h"
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
	for (auto it = m_pLaserList.begin(); it != m_pLaserList.end();)
	{
		// レーザーが無効になったら削除
		if (!it->second.pLaser->IsEnabled())
		{
			// レーザーの削除
			it = m_pLaserList.erase(it);
		}
		else
		{
			// 次の要素へ
			it++;
		}
	}

	// レーザーの更新
	for (auto& laser : m_pLaserList)
	{
		laser.second.pLaser->Update();
	}
}

// 描画
void LaserManager::Draw()
{
	for (auto& laser : m_pLaserList)
	{
		laser.second.pLaser->Draw();
	}
}

// レーザーの追加
int LaserManager::AddLaser(LaserType type, std::shared_ptr<EnemyBase> pEnemy, int laserFireFrame, float laserSpeed, bool isPlayerFollowing)
{
	LaserData laserData;
	laserData.type = type;

	// レーザーの種類によって処理を分岐
	switch (laserData.type)
	{
	case LaserType::NORMAL:
		laserData.pLaser = std::make_shared<NormalLaser>(
			m_modelHandleTable[LaserType::NORMAL],
			pEnemy, m_pPlayer, laserFireFrame, laserSpeed, isPlayerFollowing);
		break;
	default:
		assert(!"レーザーの種類がありません");
	}

	// Keyの設定
	int key = 0;
	if (m_pLaserList.size() != 0)
	{
		key = m_pLaserList.rbegin()->first + 1;
	}

	// レーザーリストに追加
	m_pLaserList[key] = laserData;

	// Keyを返す
	return key;
}

// レーザーの削除
void LaserManager::DeleteLaser(int key)
{
	// レーザーの削除
	m_pLaserList.erase(key);
}

// レーザーの取得	
const std::map<int, LaserData>& LaserManager::GetLaserList() const
{
	return m_pLaserList;
}