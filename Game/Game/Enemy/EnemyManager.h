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
	float speed;		// 敵の移動速度
	float scale;		// 敵の大きさ
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
	// コンストラクタ
	EnemyManager(
		std::shared_ptr<Player> pPlayer, 
		std::shared_ptr<LaserManager> pLaserManager,
		std::shared_ptr<ScreenShaker> pScreenShaker);

	// デストラクタ
	~EnemyManager();

	// 更新
	void Update();
	void UpdateNormal();
	void UpdateWarning();

	// 描画
	void Draw();

	// ウェーブスタート
	void StartWave();

	// 次のウェーブへ
	void NextWave();

	// 雑魚敵の追加
	void AddEnemy(EnemyData data);

	// ボス敵の追加
	void AddBossEnemy(BossEnemyType type);

	// ファイルデータの読み込み
	void LoadWaveFileData(std::string filePath);		// ウェーブ
	std::vector<EnemyData> LoadEnemyFileData(std::string filePath);	// 敵
	std::vector<EnemyActionData> LoadEnemyActionFileData(std::string filePath);	// 敵の行動

	// ゲッター
	const std::list<std::shared_ptr<EnemyBase>>& GetEnemyList() const;	// 雑魚敵リスト
	const std::shared_ptr<EnemyBase>& GetBossEnemy() const;				// ボス敵
	bool IsDeadBoss() const;											// ボスが倒されたかどうか
	bool IsEndWave() const;												// 現在のウェーブが終了したかどうか

private:
	// ステート
	enum class State
	{
		NORMAL,
		WARNING,
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
	bool m_isDeadBoss;	// ボスが倒されたかどうか
	bool m_isBoss;		// ボスが出現したかどうか
	bool m_isNextWave;	// 次のウェーブに移行するかどうか
	bool m_isLoadWave;	// ウェーブデータを読み込んだかどうか
	bool m_isStartWave;	// ウェーブを開始したかどうか

	// ウェーブ
	std::vector<WaveData> m_waveTable;	// ウェーブデータ
	BossEnemyType m_bossType;			// ボスの種類
	int m_waveNow;						// 現在のウェーブ数
};