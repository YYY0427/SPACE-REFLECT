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
	 
	 /// <summary>
	 /// デストラクタ
	 /// </summary>
	 ~TestScene();
	 
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
	std::unique_ptr<Warning> m_pWarning;
	std::unique_ptr<ResultWindow> m_pResultWindow;
	std::vector<std::vector<std::string>> m_fileNames;
};