#pragma once
#include <memory>
#include <list>
#include <vector>
#include <string>
#include <map>
#include "../../Math/Vector3.h"
#include "../Laser/LaserManager.h"
#include "../../StateMachine.h"

// プロトタイプ宣言
class EnemyBase;
class Player;
class Warning;
class ScreenShaker;

// 敵の種類
enum class EnemyType
{
	// 通常の敵
	MOSQUITO,		// 蚊
};

// ボス敵の種類
enum class BossEnemyType
{
	// ボス敵
	MOSQUITO,		// 蚊
	MATRIX,			// マトリックス
	NONE,			// なし
};

// 敵の行動のデータ
struct EnemyActionData
{
	Vector3 goalPos;		// 目的地
	int idleFrame;			// 目的地に到達してから次の目的地に向かうまでの待機フレーム
	bool isLaser;			// 目的地に到達したらレーザーを撃つか
	LaserType laserType;	// レーザーを撃つ場合、どのレーザーを撃つか
	float laserChargeFrame;	// レーザーを撃つ場合、レーザーを撃つまでのチャージフレーム
	int laserIdleFrame;		// レーザーを撃つ場合、目的地に到達してからレーザーを撃つまでの待機フレーム
	float laserSpeed;		// レーザーを撃つ場合、レーザーの移動速度
	int laserFireFrame;		// レーザーを何フレーム発射し続けるか
	bool isPlayerFollowing;	// レーザーを撃つ場合、レーザーがプレイヤーを追従するかどうか
};

// 敵のデータ
struct EnemyData
{
	Vector3 pos;	// 敵の初期位置
	EnemyType type;	// 敵の種類
	int hp;			// 敵の体力
	int attack;		// 敵の攻撃力
	float speed;	// 敵の移動速度
	float scale;	// 敵の大きさ
	std::vector<EnemyActionData> actionDataList;	// 敵の行動データリスト
};

// ウェーブデータ
struct WaveData
{
	std::vector<EnemyData> enemyDataList;	// 敵のデータリスト
};

/// <summary>
/// 敵の管理クラス
/// </summary>
class EnemyManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pPlayer">プレイヤーのポインタ</param>
	/// <param name="pLaserManager">レーザー管理のポインタ</param>
	/// <param name="pScreenShaker">画面揺れのポインタ</param>
	EnemyManager(
		const std::shared_ptr<Player>& pPlayer, 
		const std::shared_ptr<LaserManager>& pLaserManager,
		const std::shared_ptr<ScreenShaker>& pScreenShaker);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyManager();

	/// <summary>
	/// ステートの更新
	/// </summary>
	void Update();

	/// <summary>
	/// 通常の更新
	/// </summary>
	void UpdateNormal();

	/// <summary>
	/// 警告時の更新
	/// </summary>
	void UpdateWarning();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ウェーブの開始
	/// </summary>
	void StartWave();

	/// <summary>
	/// 次のウェーブに移行
	/// </summary>
	void NextWave();

	/// <summary>
	/// 敵の追加
	/// </summary>
	/// <param name="data">敵のデータ</param>
	void AddEnemy(const EnemyData& data);

	/// <summary>
	/// ボス敵の追加
	/// </summary>
	/// <param name="type">ボス敵の種類</param>
	void AddBossEnemy(const BossEnemyType& type);

	/// <summary>
	/// 敵のステージデータを読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void LoadEnemyStageFileData(const std::string& filePath);		

	/// <summary>
	/// 敵のウェーブのデータを読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns>読み込んだデータ</returns>
	std::vector<EnemyData> LoadEnemyWaveFileData(const std::string& filePath);	

	/// <summary>
	/// 敵の行動データを読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns>読み込んだデータ</returns>
	std::vector<EnemyActionData> LoadEnemyActionFileData(const std::string& filePath);	

	/// <summary>
	/// 雑魚敵リストの取得
	/// </summary>
	/// <returns>雑魚敵リスト</returns>
	const std::list<std::shared_ptr<EnemyBase>>& GetEnemyList() const;	

	/// <summary>
	/// ボス敵の取得
	/// </summary>
	/// <returns>ボス敵</returns>
	const std::shared_ptr<EnemyBase>& GetBossEnemy() const;				

	/// <summary>
	/// ボス敵が生存しているかどうかの取得
	/// </summary>
	/// <returns></returns>
	bool IsBossAlive() const;

	/// <summary>
	/// ボスが倒され、死亡演出中かどうかの取得
	/// ボスが出現していない場合はfalseを返す
	/// </summary>
	/// <returns>true : 死亡演出中、false : 死亡演出中でない</returns>
	bool IsDeadBossAnim() const;

	/// <summary>
	/// ボスが倒されたか、死亡演出を終了したかの取得
	/// </summary>
	/// <returns>true : 倒された、false : まだ生存している</returns>
	bool IsDeadBoosEndAnim() const;

	/// <summary>
	/// 現在のウェーブが終了したかどうかの取得
	/// </summary>
	/// <returns>true : 終了、false : 途中</returns>
	bool IsEndWave() const;												

private:
	// ステート
	enum class State
	{
		NORMAL,
		WARNING,
	};

	// ウェーブのインデックス
	enum class EnemyWaveIndex
	{
		POS_X,
		POS_Y,
		POS_Z,
		TYPE,
		HP,
		ATTACK,
		SCALE,
		SPEED,
		ACTION,
	};

	// 敵の行動のインデックス
	enum class EnemyActionIndex
	{
		GOAL_POS_X,
		GOAL_POS_Y,
		GOAL_POS_Z,
		IDLE_FRAME,
		IS_LASER,
		LASER_TYPE,
		LASER_CHARGE_FRAME,
		LASER_IDLE_FRAME,
		LASER_SPEED,
		LASER_FIRE_FRAME,
		IS_PLAYER_FOLLOWING,
	};

private:
	// ステートマシン
	StateMachine<State> m_stateMachine;	

	// ポインタ
	std::shared_ptr<Player> m_pPlayer;						// プレイヤー
	std::shared_ptr<LaserManager> m_pLaserManager;			// レーザー管理
	std::list<std::shared_ptr<EnemyBase>> m_pEnemyList;		// 雑魚敵リスト
	std::shared_ptr<EnemyBase> m_pBossEnemy;				// ボス敵
	std::shared_ptr<Warning> m_pWarning;					// 警告
	std::shared_ptr<ScreenShaker> m_pScreenShaker;			// 画面揺れ

	// フラグ
	bool m_isDeadBoss;			// ボスが倒されたかどうか
	bool m_isBoss;				// ボスが出現したかどうか
	bool m_isNextWave;			// 次のウェーブに移行するかどうか
	bool m_isLoadWave;			// ウェーブデータを読み込んだかどうか
	bool m_isStartWave;			// ウェーブを開始したかどうか
	bool m_isDeadEffectSound;	// 死亡演出のサウンドを再生したかどうか

	// ウェーブ
	std::vector<WaveData> m_waveTable;	// ウェーブデータ
	BossEnemyType m_bossType;			// ボスの種類
	int m_waveNow;						// 現在のウェーブ数
};