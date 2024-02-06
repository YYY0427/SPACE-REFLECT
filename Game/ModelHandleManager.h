#pragma once
#include <string>
#include <map>

// モデルの種類
enum class ModelType
{
	PLAYER,
	MOSQUITO,
	MATRIX,
	MOON,
	EARTH,
	METEOR,
	SKYDOME,
	LASER,
	CUBE_LASER,
	NUM
};

/// <summary>
/// モデルなどのハンドルを管理するシングルトンクラス
/// </summary>
class ModelHandleManager
{
public:
	// インスタンスの取得
	static ModelHandleManager& GetInstance();

	// デストラクタ
	~ModelHandleManager();

	// ハンドルの解放
	void ReleaseHandle();

	int LoadModel(const std::string& filePath);

	// ハンドルの取得
	int GetHandle(ModelType type);

	// ハンドルの削除
	void DeleteHandle(ModelType type);

private:
	// コンストラクタ
	ModelHandleManager();

	// コピー、代入禁止
	ModelHandleManager(const ModelHandleManager&) = delete;
	ModelHandleManager& operator=(const ModelHandleManager&) = delete;

private:
	struct ModelData
	{
		// モデルハンドル
		int modelHandle = -1;

		// ファイルパス
		std::string filePath;
	};

private:
	// ハンドルのマップ
	std::map<ModelType, ModelData> m_handleMap;
};