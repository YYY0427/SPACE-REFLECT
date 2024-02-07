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
	GameScene(SceneManager& manager, Stage stage);
	
	// デストラクタ
	~GameScene();
	
	// 更新
	void Update() override final;
	
	// 描画
	void Draw() override final;

private:
	// ステージ
	std::unique_ptr<StageBase> m_pStage;

	// いまのステージ
	Stage m_stage;
};