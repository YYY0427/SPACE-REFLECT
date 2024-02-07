#include "PlanetManager.h"
#include "Planet.h"
#include "../Editor/DataReaderFromUnity.h"
#include "../Util/DrawFunctions.h"
#include "../ModelHandleManager.h"
#include "../Math/MathUtil.h"
#include <string>

namespace
{
	// ステージセレクト時の回転速度
	const Vector3 stage_select_rotate_speed = { 0, MathUtil::ToRadian(0.5f), 0 };

	// プレイ画面の回転速度
	const Vector3 play_rotate_speed = { 0, MathUtil::ToRadian(0.01f), 0 };
}

// コンストラクタ
PlanetManager::PlanetManager(std::string objectDataFileName)
{
	// インスタンスの取得
	auto& dataReader = DataReaderFromUnity::GetInstance();

	// 地球のインスタンスの作成
	auto& earthData = dataReader.GetData(objectDataFileName, "Earth");
	for (auto& earth : earthData)
	{
		m_planetData[PlanetType::EARTH].modelHandle = ModelHandleManager::GetInstance().GetHandle(ModelType::EARTH);
		m_planetData[PlanetType::EARTH].pPlanet = std::make_shared<Planet>(m_planetData[PlanetType::EARTH].modelHandle, earth);
	}

	// 月のインスタンスの作成
	auto& moonData = dataReader.GetData(objectDataFileName, "Moon");
	for (auto& moon : moonData)
	{
		m_planetData[PlanetType::MOON].modelHandle = ModelHandleManager::GetInstance().GetHandle(ModelType::MOON);
		m_planetData[PlanetType::MOON].pPlanet = std::make_shared<Planet>(m_planetData[PlanetType::MOON].modelHandle, moon);
	}
}

// デストラクタ
PlanetManager::~PlanetManager()
{
	m_planetData.clear();
}

// スタート演出の更新
void PlanetManager::UpdateStart(Vector3 playerVec)
{
	for (auto& planet : m_planetData)
	{
		planet.second.pPlanet->UpdateStart(playerVec, play_rotate_speed);
	}
}

// プレイ時の更新
void PlanetManager::UpdatePlay()
{
	for (auto& planet : m_planetData)
	{
		planet.second.pPlanet->Update(play_rotate_speed);
	}
}

// ステージセレクト時の更新
void PlanetManager::UpdateStageSelect()
{
	for (auto& planet : m_planetData)
	{
		planet.second.pPlanet->Update(stage_select_rotate_speed);
	}
}

// 描画
void PlanetManager::Draw()
{
	for (auto& planet : m_planetData)
	{
		planet.second.pPlanet->Draw();
	}
}

// タイプから惑星のポインタを取得
std::shared_ptr<Planet> PlanetManager::GetPlanet(PlanetType type) const
{
	return m_planetData.at(type).pPlanet;
}