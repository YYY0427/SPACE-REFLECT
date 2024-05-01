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
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	Tutorial(SceneManager& manager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Tutorial();

	/// <summary>
	/// スタート演出ステートの開始
	/// </summary>
	void EnterStartAnimation();

	/// <summary>
	/// ゲームクリアステートの開始
	/// </summary>
	void EnterGameClear();

	/// <summary>
	/// ゲームオーバーステートの開始
	/// </summary>
	void EnterGameOver();

	/// <summary>
	/// リザルトステートの開始
	/// </summary>
	void EnterResult();

	/// <summary>
	/// 通常の更新
	/// </summary>
	void Update() override final;

	/// <summary>
	/// スタート演出ステートの更新
	/// </summary>
	void UpdateStartAnimation();

	/// <summary>
	/// 移動チュートリアルステートの更新
	/// </summary>
	void UpdateMoveTutorial();

	/// <summary>
	/// シールドチュートリアルステートの更新
	/// </summary>
	void UpdateShieldTutorial();

	/// <summary>
	/// 反射チュートリアルステートの更新
	/// </summary>
	void UpdateReflectTutorial();

	/// <summary>
	/// キューブチュートリアルステートの更新
	/// </summary>
	void UpdateCubeTutorial();

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