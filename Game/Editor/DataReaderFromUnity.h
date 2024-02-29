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
	/// <summary>
	/// デストラクタ
	/// </summary>
	~DataReaderFromUnity();

	/// <summary>
	/// Unityで配置したオブジェクトのデータを読み取る 
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	void LoadUnityGameObjectData(const std::string& fileName);

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static DataReaderFromUnity& GetInstance();

	/// <summary>
	/// Unityで配置したオブジェクトのデータを取得
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="objectName">オブジェクト名</param>
	/// <returns>データ</returns>
	std::vector<UnityGameObject> GetData(const std::string& fileName, const std::string& objectName) const;

	/// <summary>
	/// 全てのデータを削除
	/// </summary>
	void DeleteAllData();

private:
	/// <summary>
	/// デストラクタ
	/// </summary>
	DataReaderFromUnity();

	/// <summary>
	/// コピーコンストラクタ禁止
	/// </summary>
	/// <param name=""></param>
	DataReaderFromUnity(const DataReaderFromUnity&) = delete;	

	/// <summary>
	/// 代入演算子禁止
	/// </summary>
	/// <param name=""></param>
	void operator = (const DataReaderFromUnity&) = delete;		

private:
	// 読み取ったデータ
	// ここにデータを格納する
	std::map<std::string, std::map<std::string, std::vector<UnityGameObject>>> m_data;
};