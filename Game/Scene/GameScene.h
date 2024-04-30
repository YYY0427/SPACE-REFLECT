#pragma once
#include "StageSelectScene.h"
#include "SceneBase.h"
#include <memory>

// プロトタイプ宣言
class StageBase;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene final : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	/// <param name="fileName">どのステージか</param>
	GameScene(SceneManager& manager, const Stage stage);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();
	
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
	// ステージ
	std::unique_ptr<StageBase> m_pStage;

	// いまのステージ
	Stage m_stage;
};