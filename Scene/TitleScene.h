#pragma once
#include "SceneBase.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene final : public SceneBase
{
public:
	/// </summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	TitleScene(SceneManager& manager);
	
	// デストラクタ
	~TitleScene();
	
	// 更新
	void Update() override final;
	
	// 描画
	void Draw() override final;
};