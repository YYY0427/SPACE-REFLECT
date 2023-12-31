#pragma once
#include <string>
#include <map>
#include <vector>
#include "../Math/Vector3.h"

// Unityで配置したオブジェクトのデータ
struct UnityGameObject
{
	std::string name;
	Vector3 pos;
	Vector3 rot;
	Vector3 scale;
};

/// <summary>
/// Unityから配置データを読み込むクラス
/// </summary>
class DataReaderFromUnity
{
public:
	// コンストラクタ
	DataReaderFromUnity();

	// デストラクタ
	~DataReaderFromUnity();

	// Unityで配置したオブジェクトのデータを読み取る
	void LoadUnityGameObjectData(std::string fileName);

	// 読み取ったデータの取得
	const std::vector<UnityGameObject>& GetData(std::string objectName) const;

private:
	// 読み取ったデータ
	// ここにデータを格納する
	std::map<std::string, std::vector<UnityGameObject>> m_data;
};