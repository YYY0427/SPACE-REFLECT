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

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneBase();

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
	// シーンマネーシャーの参照
	SceneManager& m_manager;

	// 画面切り替え演出のインスタンス
	std::unique_ptr<TransitorBase> m_pTransitor;

	// フェード
	std::unique_ptr<Fade> m_pFade;
};