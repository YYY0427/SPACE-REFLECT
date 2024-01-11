#pragma once
#include "../Editor/DataReaderFromUnity.h"
#include <memory>
#include <vector>
#include <map>

// プロトタイプ宣言
 class Planet;

/// <summary>
/// 惑星を管理するクラス
/// </summary>
class PlanetManager
{
public:
	// コンストラクタ
	PlanetManager();

	// デストラクタ
	~PlanetManager();

	// 更新
	void UpdateStart(Vector3 playerVec);
	void Update();

	// 描画
	void Draw();

private:
	// 惑星
	std::vector<std::shared_ptr<Planet>> m_pPlanet;

	// モデルのデータ
	std::map<std::string, int> m_modelHandleTable;
};