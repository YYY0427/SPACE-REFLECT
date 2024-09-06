#pragma once
#include "SceneBase.h"
#include "PuseScene.h"

namespace Scene
{
	/// <summary>
	/// 確認画面
	/// </summary>
	class Confirm final : public Scene::Base
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="manager">シーンマネージャ</param>
		/// <param name="state">ポーズシーンの状態</param>
		Confirm(const std::shared_ptr<Scene::Manager>& pSceneManager, const Puse::State state);

		/// <summary>
		/// デストラクタko
		/// </summary>
		~Confirm();

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
		// 確認の状態
		enum class State
		{
			NO,		// いいえ
			YES,	// はい
			NUM		// 状態数
		};

	private:
		// 項目の描画色データ
		std::vector<unsigned int> m_itemColorTable;

		// 現在の選択項目
		int m_currentSelectItem;

		// どこに戻るのかを確認するポーズシーンの状態
		Puse::State m_state;

		// モザイク処理用スクリーン
		int m_gaussScreen;
	};
}