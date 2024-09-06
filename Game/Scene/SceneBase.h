#pragma once
#include <memory>
#include "../Transitor/TransitorBase.h"
#include "SceneManager.h"
#include "../Transitor/Fade.h"

namespace Scene
{
	// プロトタイプ宣言
	class Manager;

	/// <summary>
	/// シーンの基底クラス
	/// </summary>
	class Base
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="manager">シーンマネージャのインスタンス</param>
		Base(const std::shared_ptr<Scene::Manager>& pSceneManager) :
			m_pSceneManager(pSceneManager)
		{
		}

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~Base() {}

		/// <summary>
		/// 初期化
		/// コンストラクタでは好きな場所で初期化を行えないため作成
		/// </summary>
		virtual void Init() = 0;

		/// <summary>
		/// 終了処理
		/// デストラクタでは好きな場所で終了処理を行えないため作成
		/// </summary>
		virtual void End() = 0;

		/// <summary>
		/// 更新
		/// </summary>
		virtual void Update() = 0;

		/// <summary>
		/// 描画
		/// </summary>
		virtual void Draw() = 0;

	protected:
		// シーンのマネージャー
		std::shared_ptr<Scene::Manager> m_pSceneManager;

		// 画面切り替え演出のインスタンス
		std::unique_ptr<TransitorBase> m_pTransitor;

		// フェード
		std::unique_ptr<Fade> m_pFade;
	};
}