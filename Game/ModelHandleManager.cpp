#include "ModelHandleManager.h"
#include "Util/DrawFunctions.h"
#include "Util/FileUtil.h"
#include <DxLib.h>
#include <cassert>

namespace
{
	// モデルのファイルパス
	const std::string model_list_file_path = "Data/Csv/Model.csv";		
}

// インスタンスの取得
ModelHandleManager& ModelHandleManager::GetInstance()
{
	static ModelHandleManager instance;
	return instance;
}

// コンストラクタ
ModelHandleManager::ModelHandleManager()
{
	// モデルのファイルパスの読み込み
	auto data = FileUtil::LoadCsvFile(model_list_file_path);
	for (auto& line : data)
	{
		// モデルの読み込み
		m_handleMap[line[static_cast<int>(ModelFileType::ID)]].modelHandle = LoadModel(line[static_cast<int>(ModelFileType::FILE_PATH)]);

		// ファイルパスの登録
		m_handleMap[line[static_cast<int>(ModelFileType::ID)]].filePath = line[static_cast<int>(ModelFileType::FILE_PATH)];
	}
}

// デストラクタ
ModelHandleManager::~ModelHandleManager()
{
	// ハンドルの解放
	ReleaseHandle();
}

// ハンドルの解放
void ModelHandleManager::ReleaseHandle()
{
	// モデルのハンドルを解放
	for (auto& handle : m_handleMap)
	{
		MV1DeleteModel(handle.second.modelHandle);
	}
}

// モデルの読み込み
int ModelHandleManager::LoadModel(const std::string& filePath)
{
	// 既にファイルパスが登録されている場合はハンドル返す
	for (auto& handle : m_handleMap)
	{
		if (handle.second.filePath == filePath)
		{
			return handle.second.modelHandle;
		}
	}

	// モデルの読み込み
	return my::MyLoadModel(filePath.c_str());
}

// ハンドルの取得
int ModelHandleManager::GetHandle(const std::string& id)
{
	// ハンドルが存在していたら
	if (m_handleMap.find(id) != m_handleMap.end())
	{
		return m_handleMap[id].modelHandle;
	}
	// 存在しなかったら
	assert(!"モデルのIDが見つかりません");
	return -1;
}

// ハンドルの削除
void ModelHandleManager::DeleteHandle(const std::string& id)
{
	// ハンドルが存在していたら
	if (m_handleMap.find(id) != m_handleMap.end())
	{
		// モデルのハンドルを解放
		MV1DeleteModel(m_handleMap[id].modelHandle);

		// マップから削除
		m_handleMap.erase(id);
	}
	// 存在しなかったら
	else
	{
		assert(!"モデルのIDが見つかりません");
	}
}