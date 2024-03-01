#pragma once
#include "StageBase.h"
#include "../StateMachine.h"
#include <memory>

/// <summary>
/// ステージ1
/// </summary>
class Stage1 final : public StageBase
{
public:
	// コンストラクタ
	Stage1(SceneManager& manager);

	// デストラクタ
	~Stage1();

	// 開始
	void EnterResult();
	void EnterGameOver();

	// 更新
	void Update() override final;
	void UpdateStartAnimation();
	void UpdatePlay();
	void UpdateGameClear();
	void UpdateGameOver();
	void UpdateResult();

	// 描画
	void Draw() override final;

private:
	// 状態
	enum class State
	{
		START_ANIMATION,	// スタート演出
		PLAY,				// プレイ中
		GAME_OVER,			// ゲームオーバー
		GAME_CLEAR,			// ゲームクリア
		RESULT,				// リザルト
		NUM					// 状態の数
	};

private:
	// ステートマシン
	StateMachine<State> m_stateMachine;
};