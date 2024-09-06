#pragma once
#include <string>
#include <map>
#include "../../Util/SingletonBase.h"

namespace Resource::Model
{
	/// <summary>
	/// モデルのハンドルを管理するシングルトンクラス
	/// </summary>
	class Manager : public SingletonBase<Manager>
	{
	public:
		// SingletonBaseクラスでのみインスタンス生成を許可する
		friend class SingletonBase<Manager>;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Manager();

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
		Manager();

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
}