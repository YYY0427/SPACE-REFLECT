#include "ModelHandleManager.h"
#include "Util/DrawFunctions.h"
#include <DxLib.h>

namespace
{
	// モデルのファイルパス
	const std::string player_model_file_path = "Data/Model/Player.mv1";
	const std::string mosquito_model_file_path = "Data/Model/Mosquito.mv1";
	const std::string matrix_model_file_path = "Data/Model/Matrix.mv1";
	const std::string earth_model_file_path = "Data/Model/Earth.mv1";
	const std::string moon_model_file_path = "Data/Model/Moon.mv1";
	const std::string sky_dome_model_file_path = "Data/Model/SkyDome.mv1";
	const std::string meteorite_model_file_path = "Data/Model/Meteor2.mv1";
	const std::string laser_model_file_path = "Data/Model/Laser.mv1";
	const std::string cube_laser_model_file_path = "Data/Model/CubeLaser.mv1";
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
	// モデルの読み込み
	m_handleMap[ModelType::PLAYER].modelHandle = LoadModel(player_model_file_path.c_str());
	m_handleMap[ModelType::MOSQUITO].modelHandle = LoadModel(mosquito_model_file_path.c_str());
	m_handleMap[ModelType::MATRIX].modelHandle = LoadModel(matrix_model_file_path.c_str());
	m_handleMap[ModelType::EARTH].modelHandle = LoadModel(earth_model_file_path.c_str());
	m_handleMap[ModelType::MOON].modelHandle = LoadModel(moon_model_file_path.c_str());
	m_handleMap[ModelType::METEOR].modelHandle = LoadModel(meteorite_model_file_path.c_str());
	m_handleMap[ModelType::SKYDOME].modelHandle = LoadModel(sky_dome_model_file_path.c_str());
	m_handleMap[ModelType::LASER].modelHandle = LoadModel(laser_model_file_path.c_str());
	m_handleMap[ModelType::CUBE_LASER].modelHandle = LoadModel(cube_laser_model_file_path.c_str());
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
int ModelHandleManager::GetHandle(ModelType type)
{
	return m_handleMap[type].modelHandle;
}

// ハンドルの削除
void ModelHandleManager::DeleteHandle(ModelType type)
{
	MV1DeleteModel(m_handleMap[type].modelHandle);
	m_handleMap.erase(type);
}