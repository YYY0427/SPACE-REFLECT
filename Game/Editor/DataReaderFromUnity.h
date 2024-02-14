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
	// デストラクタ
	~DataReaderFromUnity();

	// Unityで配置したオブジェクトのデータを読み取る
	void LoadUnityGameObjectData(std::string fileName);

	// インスタンスの取得
	static DataReaderFromUnity& GetInstance();

	// 読み取ったデータの取得
	const std::vector<UnityGameObject>& GetData(std::string fileName, std::string objectName) const;

	// すべてのデータの削除
	void DeleteAllData();

private:
	// コンストラクタ
	DataReaderFromUnity();

	// コピーも代入も禁止
	DataReaderFromUnity(const DataReaderFromUnity&) = delete;	// コピーコンストラクタ禁止
	void operator = (const DataReaderFromUnity&) = delete;		// 代入禁止

private:
	// 読み取ったデータ
	// ここにデータを格納する
	std::map<std::string, std::map<std::string, std::vector<UnityGameObject>>> m_data;
};