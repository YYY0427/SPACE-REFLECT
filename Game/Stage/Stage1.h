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
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	Stage1(SceneManager& manager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Stage1();

	/// <summary>
	/// プレイステートの開始
	/// </summary>
	void EnterPlay();

	/// <summary>
	/// リザルトステートの開始
	/// </summary>
	void EnterResult();

	/// <summary>
	/// ゲームクリアステートの開始
	/// </summary>
	void EnterGameClear();

	/// <summary>
	/// ゲームオーバーステートの開始
	/// </summary>
	void EnterGameOver();

	/// <summary>
	/// 通常の更新
	/// </summary>
	void Update() override final;

	/// <summary>
	/// スタート演出ステートの更新
	/// </summary>
	void UpdateStartAnimation();

	/// <summary>
	/// プレイステートの更新
	/// </summary>
	void UpdatePlay();

	/// <summary>
	/// ゲームクリアステートの更新
	/// </summary>
	void UpdateGameClear();

	/// <summary>
	/// ゲームオーバーステートの更新
	/// </summary>
	void UpdateGameOver();

	/// <summary>
	/// リザルトステートの更新
	/// </summary>
	void UpdateResult();

	/// <summary>
	/// 描画
	/// </summary>
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