#pragma once
#include "SceneBase.h"

namespace Scene
{
	/// <summary>
	/// ロードシーン
	/// </summary>
	class Load final : public Scene::Base
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="manager">シーンマネージャのインスタンス</param>
		/// <param name="isEternal">永続表示か(デバッグ用)</param>
		Load(const std::shared_ptr<Scene::Manager>& pSceneManager, const bool isDrawEternal = false);

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Load();

		/// <summary>
		/// 初期化
		/// </summary>
		void Init() override final;

		/// <summary>
		/// 終了処理
		/// </summary>
		void End() override final;

		/// <summary>
		/// 更新
		/// </summary>
		void Update() override final;

		/// <summary>
		/// 描画
		/// </summary>
		void Draw() override final;

	private:
		// ロード画面が表示されている時間
		int m_time;

		// ロード画面が永続表示か
		bool m_isDrawEternal;
	};
}