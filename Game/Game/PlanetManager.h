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
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="objectDataFileName">オブジェクトデータのファイル名</param>
	PlanetManager(const std::string& objectDataFileName);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlanetManager();

	/// <summary>
	/// ゲームプレイ時の更新
	/// </summary>
	/// <param name="playerVec">プレイヤーのベクトル</param>
	void UpdatePlay(const Vector3& playerVec);

	/// <summary>
	/// ステージセレクト時の更新
	/// </summary>
	void UpdateStageSelect();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 惑星の取得
	/// </summary>
	/// <param name="type">惑星の種類</param>
	/// <returns>惑星のポインタ</returns>
	const std::shared_ptr<Planet>& GetPlanet(PlanetType type) const;

private:
	// 惑星のデータ
	struct PlanetData
	{
		// モデルハンドル
		int modelHandle = -1;

		// ポインタ
		std::shared_ptr<Planet> pPlanet;
	};

private:
	// 惑星のデータ
	std::map<PlanetType, PlanetData> m_planetData;
};