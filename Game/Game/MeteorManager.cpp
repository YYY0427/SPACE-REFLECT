#include "MeteorManager.h"
#include "../Util/DrawFunctions.h"
#include "../Editor/DataReaderFromUnity.h"
#include <string>

namespace
{
	// 小さい隕石の生成間隔
	const int small_create_interval_frame = 5;
}

// コンストラクタ
MeteorManager::MeteorManager(const std::string& objectDataFileName, const std::shared_ptr<Player>& pPlayer) :
	m_smallCreateIntervalFrame(0),
	m_pPlayer(pPlayer)
{
	// 配置データが存在する場合は配置データから隕石を生成
	auto data = DataReaderFromUnity::GetInstance().GetData(objectDataFileName, "Meteor2");
	for (auto& meteorData : data)
	{
		m_pMeteorList.push_back(std::make_shared<Meteor>(meteorData, m_pPlayer));
	}
}

// デストラクタ
MeteorManager::~MeteorManager()
{
	// 隕石の削除
	m_pMeteorList.clear();
}

// スタート演出時の更新
void MeteorManager::UpdateStart()
{
	// 隕石の更新
	for (auto& meteor : m_pMeteorList)
	{
		meteor->UpdateStart();
	}
}

// 更新
void MeteorManager::Update(const Vector3& cameraPos)
{
	// 存在しない隕石を削除
	m_pMeteorList.remove_if([](std::shared_ptr<Meteor> meteor) { return !meteor->IsEnabled(); });

	// 隕石の更新
	for (auto& meteor : m_pMeteorList)
	{
		meteor->Update(cameraPos);
	}
}

// 描画
void MeteorManager::Draw()
{
	for (auto& meteor : m_pMeteorList)
	{
		meteor->Draw();
	}
}

// 小さい隕石の生成
void MeteorManager::CreateSmallMeteor(const float playerRelativePos)
{
	m_smallCreateIntervalFrame++;
	if (small_create_interval_frame < m_smallCreateIntervalFrame)
	{
		// 小さい隕石の生成
		m_pMeteorList.push_back(std::make_shared<Meteor>(MeteorType::SMALL, m_pPlayer, playerRelativePos));
		m_smallCreateIntervalFrame = 0;
	}
}

// 隕石の削除
void MeteorManager::DeleteAllMeteor()
{
	m_pMeteorList.clear();
}

// 隕石の取得
const std::list<std::shared_ptr<Meteor>>& MeteorManager::GetMeteor() const
{
	return m_pMeteorList;
}
