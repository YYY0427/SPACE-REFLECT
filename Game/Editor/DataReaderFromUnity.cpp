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
void DataReaderFromUnity::LoadUnityGameObjectData(std::string fileName)
{
	// ファイル名の階層を付ける
	fileName = file_hierarchy + fileName;

	// 拡張子を付ける
	fileName += extension;

	// ファイルにアクセス
	auto dataHandle = FileRead_open(fileName.c_str());

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

		// Unityとの座標データを合わせるために100倍
		data.pos *= 100.0f;

		// 回転データxyzを読む
		result = FileRead_read(&data.rot, sizeof(data.rot), dataHandle);
		assert(result != -1);

		// 度数法を弧度法に変換
		data.rot.x = MathUtil::ToDegree(data.rot.x);
		data.rot.y = MathUtil::ToDegree(data.rot.y);
		data.rot.z = MathUtil::ToDegree(data.rot.z);

		// UnityとDxLibでは回転がずれるため合わせる
		data.rot.y += DX_PI_F;

		// 拡大データxyzを読む
		result = FileRead_read(&data.scale, sizeof(data.scale), dataHandle);
		assert(result != -1);

		// オブジェクトの名前ごとに保存
		m_data[data.name].push_back(data);
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
const std::vector<UnityGameObject>& DataReaderFromUnity::GetData(std::string objectName) const
{
	// データが見つかったか
	if (m_data.find(objectName) != m_data.end())
	{
		return m_data.find(objectName)->second;
	}
	// データが見つからなかった
	return std::vector<UnityGameObject>();
}
