#include "DataReaderFromUnity.h"
#include <cassert>
#include "../Math/MathUtil.h"
#include "DxLib.h"

namespace
{
	// ファイルの階層
	const std::string file_hierarchy = "Data/Stage/";	

	// 拡張子
	const std::string extension = ".dat";
}

// コンストラクタ
DataReaderFromUnity::DataReaderFromUnity()
{
}

// デストラクタ
DataReaderFromUnity::~DataReaderFromUnity()
{
}

// Unityで配置したオブジェクトのデータを読み取る
void DataReaderFromUnity::LoadUnityGameObjectData(const std::string& fileName)
{
	// ファイルパスの作成
	std::string filePath = file_hierarchy + fileName + extension;

	// ファイルにアクセス
	auto dataHandle = FileRead_open(filePath.c_str());

	// データ数を得る
	int dataNum = 0;
	int result = FileRead_read(&dataNum, sizeof(dataNum), dataHandle);
	assert(result != -1);

	// データの読み取り
	for (int i = 0; i < dataNum; i++)
	{
		UnityGameObject data;

		// 名前の文字列数の読み取り
		uint8_t nameSize = 0;
		result = FileRead_read(&nameSize, sizeof(nameSize), dataHandle);
		assert(result != -1);

		// 名前そのものを読み取る
		data.name.resize(nameSize);
		result = FileRead_read(data.name.data(), sizeof(char) * nameSize, dataHandle);
		assert(result != -1);

		// 座標データxyzを読む
		result = FileRead_read(&data.pos, sizeof(data.pos), dataHandle);
		assert(result != -1);

		// 回転データxyzを読む
		result = FileRead_read(&data.rot, sizeof(data.rot), dataHandle);
		assert(result != -1);

		// 度数法を弧度法に変換
		data.rot.x = MathUtil::ToRadian(data.rot.x);
		data.rot.y = MathUtil::ToRadian(data.rot.y);
		data.rot.z = MathUtil::ToRadian(data.rot.z);

		// UnityとDxLibでは回転がずれるため合わせる
		data.rot.y += DX_PI_F;

		// 拡大データxyzを読む
		result = FileRead_read(&data.scale, sizeof(data.scale), dataHandle);
		assert(result != -1);

		// ファイル名ごとに保存
		// オブジェクトの名前ごとに保存
		m_data[fileName][data.name].push_back(data);
	}

	// ファイルを閉じる
	FileRead_close(dataHandle);
}

// インスタンスの取得
DataReaderFromUnity& DataReaderFromUnity::GetInstance()
{
	// 唯一のインスタンスを返す
	static DataReaderFromUnity instance;
	return instance;
}

// オブジェクトの名前からデータを取得
std::vector<UnityGameObject> DataReaderFromUnity::GetData(const std::string& fileName, const std::string& objectName) const
{
	// ファイル名からデータを取得
	auto it = m_data.find(fileName);
	assert(it != m_data.end() && "ファイルデータがないです");

	// オブジェクトの名前からデータを取得
	auto it2 = it->second.find(objectName);
	if (it2 == it->second.end())
	{
		// データがない場合は空のデータを返す
		static std::vector<UnityGameObject> empty;
		return empty;
	}

	// データを返す
	return it2->second;
}

// すべてのデータの削除
void DataReaderFromUnity::DeleteAllData()
{
	m_data.clear();
}