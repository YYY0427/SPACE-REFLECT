#pragma once
#include "SceneBase.h"

/// <summary>
/// デバッグシーン
/// このシーンから全てのシーンに遷移できる
/// </summary>
class DebugScene final : public SceneBase
{
public:
	 /// </summary>
	 /// コンストラクタ
	 /// </summary>
	 /// <param name="manager">シーンマネージャーの参照</param>
	 DebugScene(SceneManager& manager);
	 
	 // デストラクタ
	 ~DebugScene();
	 
	 // 更新
	 void Update() override final;
	 
	 // 描画
	 void Draw() override final;

private:
	// ここから飛べるシーン項目
	enum class SceneItem
	{
		TITLE,	// タイトル
		GAME,	// ゲーム
		STAGE_SELECT,	// ステージセレクト
		PAUSE,	// ポーズ
		OPTION,	// オプション
		RESULT,	// リザルト
		TEST,	// テスト
		EXIT,	// 終了
		NUM		// 項目数
	};

private:
	// 現在選択しているシーン項目
	int m_currentSelectSceneItem;
};