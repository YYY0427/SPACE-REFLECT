#pragma once
#include "SceneBase.h"
#include "../Util/Timer.h"
#include "../StateMachine.h"
#include "../Vector2.h"
#include <memory>

// プロトタイプ宣言
class RockManager;
class PlanetManager;
class Camera;
class Player;
class DataReaderFromUnity;
class EnemyManager;
class LaserManager;
class Background;
class ScreenEffect;
class UIManager;

// メインシーン
// ゲームのメインの処理を行うシーン
class GameMainScene : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	GameMainScene(SceneManager& manager);

	// デストラクタ
	~GameMainScene();

	// メンバ関数ポインタの更新
	void Update() override;

	// 描画
	void Draw() override;

private:
	void EntarNormalState();
	void EntarGameClearState();
	void EntarGameOverState();

	void UpdateNormalState();
	void UpdateGameClearState();
	void UpdateGameOverState();

	void ExitNormalState();
	void ExitGameClearState();
	void ExitGameOverState();

	// 当たり判定
	void Collision();

private:
	enum class SceneItem
	{
		PAUSE,
		RESULT, 
		TITLE
	};

	enum class State
	{
		NORMAL,
		GAME_CLEAR,
		GAME_OVER,
	};

private:
	// シーンアイテム
	SceneItem item_;

	// ステートマシン
	StateMachine<State> stateMachine_;

	// ゲーム開始からの経過時間
	Timer<int> gameTimer_;

	// ボスの死亡演出のフレーム
	Timer<int> bossDiedEffectFrame_;

	Timer<int> waitTimer_;

	int screenHandle_;

	// ポインタ
	std::shared_ptr<Background> pBackground_;
	std::shared_ptr<Camera> pCamera_;
	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<RockManager> pRockManager_;
	std::shared_ptr<DataReaderFromUnity> pDataReader_;
	std::shared_ptr<PlanetManager> pPlanetManager_;
	std::shared_ptr<EnemyManager> pEnemyManager_;
	std::shared_ptr<LaserManager> pLaserManager_;
	std::shared_ptr<ScreenEffect> pScreenEffect_;
	std::shared_ptr<UIManager> pUIManager_;
};

