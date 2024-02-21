#pragma once
#include <string>
#include <map>

/// <summary>
/// モデルのハンドルを管理するシングルトンクラス
/// </summary>
class ModelHandleManager
{
public:
	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static ModelHandleManager& GetInstance();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ModelHandleManager();

	/// <summary>
	/// ハンドルの解放
	/// </summary>
	void ReleaseHandle();

	/// <summary>
	/// モデルの読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns>モデルハンドル</returns>
	int LoadModel(const std::string& filePath);

	/// <summary>
	/// ハンドルの取得
	/// </summary>
	/// <param name="id">外部ファイルで設定したID</param>
	/// <returns>モデルハンドル</returns>
	int GetHandle(const std::string& id);

	/// <summary>
	/// ハンドルの削除
	/// </summary>
	/// <param name="id">外部ファイルで設定したID</param>
	void DeleteHandle(const std::string& id);

private:
	/// <summary>
	/// コンストラクタ
	/// シングルトンのためprivate
	/// </summary>
	ModelHandleManager();

	/// <summary>
	/// コピーコンストラクタ禁止
	/// </summary>
	/// <param name=""></param>
	ModelHandleManager(const ModelHandleManager&) = delete;

	/// <summary>
	/// 代入演算子禁止
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	ModelHandleManager& operator=(const ModelHandleManager&) = delete;

private:
	// モデルのファイルタイプ
	enum class ModelFileType
	{
		ID,			// ID
		FILE_PATH,	// ファイルパス
	};

private:
	// モデルデータ
	struct ModelData
	{
		int modelHandle = -1; // モデルハンドル
		std::string filePath; // ファイルパス
	};

private:
	// ハンドルのマップ
	std::map<std::string, ModelData> m_handleMap;
};