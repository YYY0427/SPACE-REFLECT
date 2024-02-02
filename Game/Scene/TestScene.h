#pragma once
#include "SceneBase.h"
#include <string>
#include <vector>

// プロトタイプ宣言
class Warning;

/// <summary>
/// テスト用シーン
/// 色々試すときに使う
/// </summary>
class TestScene final : public SceneBase
{
public:
	/// </summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	 TestScene(SceneManager& manager);
	 
	 // デストラクタ
	 ~TestScene();
	 
	 // 更新
	 void Update() override final;
	 
	 // 描画
	 void Draw() override final;

private:
	// 警告UI
	std::unique_ptr<Warning> m_pWarning;

	std::vector<std::string> m_fileNames;
};