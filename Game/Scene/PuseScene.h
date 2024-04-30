#pragma once
#include "SceneBase.h"
#include "StageSelectScene.h"

/// <summary>
/// ポーズシーン
/// </summary>
class PuseScene final :  public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャ</param>
	PuseScene(SceneManager& manager, const Stage stage);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PuseScene();

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

	// ポーズの状態
	enum class State
	{
		CONTINUE,		// 続ける
		OPTION,			// オプション
		STAGE_SELECT,	// ステージセレクト
		TITLE,			// タイトル
		NUM				// 状態数
	};

private:
	// 現在の状態
	int m_currentSelectItem;

	// 項目の描画色データ
	std::vector<int> m_itemColorTable;

	// 現在のステージ
	Stage m_stage;

	// モザイク処理用スクリーン
	int m_gaussScreen;
};

