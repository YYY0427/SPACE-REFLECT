#pragma once
#include "SceneBase.h"
#include <string>
#include <vector>

// プロトタイプ宣言
class Warning;
class ResultWindow;

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
	std::unique_ptr<Warning> m_pWarning;
	std::unique_ptr<ResultWindow> m_pResultWindow;
	std::vector<std::vector<std::string>> m_fileNames;
};