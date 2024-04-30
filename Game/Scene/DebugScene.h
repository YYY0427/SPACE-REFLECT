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
	 
	 /// <summary>
	 /// デストラクタ
	 /// </summary>
	 ~DebugScene();
	 
	 /// <summary>
	 /// 初期化
	 /// </summary>
	 void Init()  override final;

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
	// ここから飛べるシーン項目
	enum class SceneItem
	{
		TITLE,			// タイトル
		STAGE_SELECT,	// ステージセレクト
		PAUSE,			// ポーズ
		OPTION,			// オプション
		TEST,			// テスト
		EXIT,			// 終了
		NUM				// 項目数
	};

private:
	// 現在選択しているシーン項目
	int m_currentSelectSceneItem;
};