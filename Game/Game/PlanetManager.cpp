#include "PlanetManager.h"
#include "Planet.h"
#include "../Editor/DataReaderFromUnity.h"
#include "../Util/DrawFunctions.h"
#include <string>

namespace
{
	// 太陽のモデルのファイルパス
	const std::string sun_model_file_path = "Data/Model/Sun.mv1";

	// 地球のモデルのファイルパス
	const std::string earth_model_file_path = "Data/Model/Earth.mv1";

	// 月のモデルのファイルパス
	const std::string moon_model_file_path = "Data/Model/Moon.mv1";
}

// コンストラクタ
PlanetManager::PlanetManager()
{
	// モデルのロード
	m_modelHandleTable["Earth"] = my::MyLoadModel(earth_model_file_path.c_str());
//	m_modelHandleTable["Sun"] = my::MyLoadModel(sun_model_file_path.c_str());
//	m_modelHandleTable["Moon"] = my::MyLoadModel(moon_model_file_path.c_str());

	// インスタンスの取得
	auto& dataReader = DataReaderFromUnity::GetInstance();

	// 太陽のインスタンスの作成
	/*auto& sunData = dataReader.GetData("Sun");
	for (auto& sun : sunData)
	{
		m_pPlanet.push_back(std::make_shared<Planet>(m_modelHandleTable["Sun"], sun));
	}*/

	// 地球のインスタンスの作成
	auto& earthData = dataReader.GetData("Earth");
	for (auto& earth : earthData)
	{
		m_pPlanet.push_back(std::make_shared<Planet>(m_modelHandleTable["Earth"], earth));
	}

	// 月のインスタンスの作成
	/*auto& moonData = dataReader.GetData("Moon");
	for (auto& moon : moonData)
	{
		m_pPlanet.push_back(std::make_shared<Planet>(m_modelHandleTable["Moon"], moon));
	}*/
}

// デストラクタ
PlanetManager::~PlanetManager()
{
}

// スタート演出の更新
void PlanetManager::UpdateStart(Vector3 playerVec)
{
	for (auto& planet : m_pPlanet)
	{
		planet->UpdateStart(playerVec);
	}
}

// 更新
void PlanetManager::Update()
{
	for (auto& planet : m_pPlanet)
	{
		planet->Update();
	}
}

// 描画
void PlanetManager::Draw()
{
	for (auto& planet : m_pPlanet)
	{
		planet->Draw();
	}
}