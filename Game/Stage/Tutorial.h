#pragma once
#include "StageBase.h"
#include "../StateMachine.h"
#include "../Util/Timer.h"
#include <string>
#include <memory>

// プロトタイプ宣言
class TutorialUI;

/// <summary>
/// チュートリアルステージクラス
/// </summary>
class Tutorial final : public StageBase
{
public:
	// コンストラクタ
	Tutorial(SceneManager& manager);

	// デストラクタ
	~Tutorial();

	// 開始
	void EnterStartAnimation();
	void EnterResult();

	// 更新
	void Update() override final;
	void UpdateStartAnimation();
	void UpdateMoveTutorial();
	void UpdateShieldTutorial();
	void UpdateReflectTutorial();
	void UpdateCubeTutorial();
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
		MOVE_TUTORIAL,		// 移動チュートリアル
		SHIELD_TUTORIAL,	// シールドチュートリアル
		REFLECT_TUTORIAL,	// 反射チュートリアル
		CUBE_TUTORIAL,		// キューブチュートリアル
		PLAY,				// プレイ中
		GAME_CLEAR,			// ゲームクリア
		GAME_OVER,			// ゲームオーバー
		RESULT,				// リザルト
		NUM					// 状態の数
	};

private:
	// ステートマシン
	StateMachine<State> m_stateMachine;

	// ポインタ
	std::shared_ptr<TutorialUI> m_pTutorialUI;
};