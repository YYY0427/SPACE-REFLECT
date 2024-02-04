#pragma once
#include "../Editor/DataReaderFromUnity.h"
#include <memory>
#include <vector>
#include <map>

// プロトタイプ宣言
 class Planet;

 // 惑星の種類
 enum class PlanetType
 {
	 MOON,
	 EARTH,
	 MARS,
	 JUPITER,
	 SATURN,
	 URANUS,
	 NEPTUNE,
	 PLUTO,
	 NUM
 };	

/// <summary>
/// 惑星を管理するクラス
/// </summary>
class PlanetManager
{
public:
	// コンストラクタ
	PlanetManager(std::string objectDataFileName);

	// デストラクタ
	~PlanetManager();

	// 更新
	void UpdateStart(Vector3 playerVec);
	void Update();

	// 描画
	void Draw();

	// ゲッター
	std::shared_ptr<Planet> GetPlanet(PlanetType type) const;

private:
	// 惑星のデータ
	struct PlanetData
	{
		// モデルハンドル
		int modelHandle;

		// 名前
		std::string name;

		// ポインタ
		std::shared_ptr<Planet> pPlanet;
	};

private:
	// 惑星のデータ
	std::map<PlanetType, PlanetData> m_planetData;
};