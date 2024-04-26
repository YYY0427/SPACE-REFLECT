#pragma once
#include "SceneBase.h"
#include "PuseScene.h"

/// <summary>
/// 確認画面
/// </summary>
class ConfirmScene : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャ</param>
	/// <param name="state">ポーズシーンの状態</param>
	ConfirmScene(SceneManager& manager, PuseScene::State state);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ConfirmScene();
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override final;
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override final;

private:
	// 確認の状態
	enum class State
	{
		YES,	// はい
		NO,		// いいえ
		NUM		// 状態数
	};

private:
	// 項目の描画色データ
	std::vector<unsigned int> m_itemColorTable;	

	// 現在の選択項目
	int m_currentSelectItem;	

	// どこに戻るのかを確認するポーズシーンの状態
	PuseScene::State m_state;

	// モザイク処理用スクリーン
	int m_gaussScreen;
};

