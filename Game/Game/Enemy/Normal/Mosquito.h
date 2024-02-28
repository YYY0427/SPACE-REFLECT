#pragma once
#include "../EnemyBase.h"
#include "../../../Util/Timer.h"
#include "../../../Game/Laser/LaserManager.h"
#include <memory>

/// <summary>
/// 雑魚敵クラス
/// </summary>
class Mosquito final : public EnemyBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="data">敵のデータ</param>
	/// <param name="pPlayer">プレイヤーのポインタ</param>
	/// <param name="pLaserManager">レーザーマネージャーのポインタ</param>
	Mosquito(const EnemyData& data, 
		     const std::shared_ptr<Player>& pPlayer, 
		     const std::shared_ptr<LaserManager>& pLaserManager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Mosquito();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override final;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override final;

	/// <summary>
	/// 輪郭線の描画
	/// </summary>
	void DrawOutLine();

	//// ゲッター ////

	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetPos() const;			

private:

	//// 初期化 ////

	/// <summary>
	/// 移動の初期化
	/// </summary>
	void EntarMove();	


	//// 更新 ////

	/// <summary>
	/// 待機状態の更新
	/// </summary>
	void UpdateIdle();

	/// <summary>
	/// 移動状態の更新
	/// </summary>
	void UpdateMove();

	/// <summary>
	/// 攻撃状態の更新
	/// </summary>
	void UpdateAttack();



	/// <summary>
	/// ゴール座標を設定
	/// </summary>
	void SetGoalPos();	

private:
	// 状態
	enum class State
	{
		IDLE,		// 待機
		MOVE,		// 移動
		ATTACK,		// 攻撃
	};

private:
	// 状態マシン
	StateMachine<State> m_state;	

	// 行動データ
	EnemyActionData m_actionData;	

	// レーザーのKey
	int m_laserKey;

	// 待機時間フレーム
	int m_idleFrame;
};