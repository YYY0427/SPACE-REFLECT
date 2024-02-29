#include "MeteorManager.h"
#include "../Util/DrawFunctions.h"
#include "../Editor/DataReaderFromUnity.h"
#include <string>

namespace
{
	// 小さい隕石の生成間隔
	const int small_create_interval_frame = 10;
}

// コンストラクタ
MeteorManager::MeteorManager(std::string objectDataFileName) :
	m_createIntervalFrame(0),
	m_smallCreateIntervalFrame(0)
{
	// 配置データが存在する場合は配置データから隕石を生成
	auto data = DataReaderFromUnity::GetInstance().GetData(objectDataFileName, "Meteor2");
	for (auto& meteorData : data)
	{
		m_pMeteorList.push_back(std::make_shared<Meteor>(meteorData));
	}
}

// デストラクタ
MeteorManager::~MeteorManager()
{
}

// スタート演出時の更新
void MeteorManager::UpdateStart(Vector3 playerVec)
{
	// 隕石の更新
	for (auto& meteor : m_pMeteorList)
	{
		meteor->UpdateStart(playerVec);
	}
}

// 更新
void MeteorManager::Update(Vector3 cameraPos)
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
void MeteorManager::SmallMeteorCreate(Vector3 playerPos)
{
	m_smallCreateIntervalFrame++;
	if (small_create_interval_frame < m_smallCreateIntervalFrame)
	{
		// 小さい隕石の生成
		m_pMeteorList.push_back(std::make_shared<Meteor>(MeteorType::SMALL, playerPos));
		m_smallCreateIntervalFrame = 0;
	}
}

// 隕石の生成
void MeteorManager::CreateMeteor(int createIntervalFrame, Vector3 playerPos)
{
	// タイマーの更新
	m_createIntervalFrame++;

	// タイマーが指定フレームを超えたら
	if (m_createIntervalFrame > createIntervalFrame)
	{
		// 隕石の生成
		m_pMeteorList.push_back(std::make_shared<Meteor>(MeteorType::NORMAL, playerPos));

		// タイマーのリセット
		m_createIntervalFrame = 0;
	}
}

// 隕石の削除
void MeteorManager::DeleteMeteor()
{
	m_pMeteorList.clear();
}

// 隕石の取得
const std::list<std::shared_ptr<Meteor>>& MeteorManager::GetMeteor() const
{
	return m_pMeteorList;
}
