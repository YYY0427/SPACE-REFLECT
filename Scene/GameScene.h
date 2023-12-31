#pragma once
#include "SceneBase.h"
#include <string>
#include <memory>

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
	/// <param name="fileName">ステージファイル名</param>
	GameScene(SceneManager& manager, std::string fileName);
	
	// デストラクタ
	~GameScene();
	
	// 更新
	void Update() override final;
	
	// 描画
	void Draw() override final;

private:
};

