#pragma once
#include <memory>
#include "../Transitor/TransitorBase.h"

// プロトタイプ宣言
class SceneManager;
class Fade;

/// <summary>
/// シーンの基底クラス
/// </summary>
class SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	SceneBase(SceneManager& manager);

	// デストラクタ
	virtual ~SceneBase();

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

protected:
	// シーンマネーシャーの参照
	SceneManager& m_manager;

	// 画面切り替え演出のインスタンス
	std::unique_ptr<TransitorBase> m_pTransitor;

	// フェード
	std::shared_ptr<Fade> m_pFade;
};