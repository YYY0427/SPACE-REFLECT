#pragma once
#include "SceneBase.h"

/// <summary>
/// ポーズシーン
/// </summary>
class PuseScene final :  public SceneBase
{
public:
	// コンストラクタ
	PuseScene(SceneManager& manager);

	// デストラクタ
	~PuseScene();

	// 更新
	void Update() override final;

	// 描画
	void Draw() override final;

private:
	// ポーズの状態
	enum class State
	{
		CONTINUE,		// 続ける
		RESTART,		// 最初から
		STAGE_SELECT,	// ステージセレクト
		OPTION,			// オプション
		TITLE,			// タイトル
	};

private:
	// 現在の状態
	int m_currentState;
};

