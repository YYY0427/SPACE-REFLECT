#include "MeteorManager.h"
#include "../Util/DrawFunctions.h"
#include "../Editor/DataReaderFromUnity.h"
#include <string>

// コンストラクタ
MeteorManager::MeteorManager(std::string objectDataFileName) :
	m_createIntervalFrameTimer(0)
{
	// 配置データが存在する場合は配置データから隕石を生成
	auto& data = DataReaderFromUnity::GetInstance().GetData(objectDataFileName, "Meteor2");
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

void MeteorManager::UpdateSmallMeteor()
{
	// 隕石の更新
	for (auto& meteor : m_pMeteorList)
	{
//		meteor->UpdateSmallMeteor();
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

// 隕石の生成
void MeteorManager::CreateMeteor(int createIntervalFrame, Vector3 playerPos)
{
	// タイマーの更新
	m_createIntervalFrameTimer++;

	// タイマーが指定フレームを超えたら
	if (m_createIntervalFrameTimer > createIntervalFrame)
	{
		// 隕石の生成
		m_pMeteorList.push_back(std::make_shared<Meteor>(playerPos));

		// タイマーのリセット
		m_createIntervalFrameTimer = 0;
	}
}

// スロー値の設定
void MeteorManager::SetSlowValue(float slowValue)
{
	for (auto& meteor : m_pMeteorList)
	{
		meteor->SetSlowValue(slowValue);
	}
}

// 隕石の取得
const std::list<std::shared_ptr<Meteor>>& MeteorManager::GetMeteor() const
{
	return m_pMeteorList;
}
