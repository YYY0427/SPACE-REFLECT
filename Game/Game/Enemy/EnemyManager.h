#pragma once
#include <memory>
#include <list>
#include <vector>
#include <string>
#include <map>
#include "../../Math/Vector3.h"

// プロトタイプ宣言
class EnemyBase;
class BossEnemyBase;
class Player;

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
	NONE,			// なし
};

// 敵の行動のデータ
struct EnemyActionData
{
	Vector3 goalPos;	// 目的地
	int idleFrame;		// 目的地に到達してから次の目的地に向かうまでの待機フレーム
	bool isLaser;		// 目的地に到達したらレーザーを撃つか
	int laserType;		// レーザーを撃つ場合、どのレーザーを撃つか
	int laserIdleFrame;	// レーザーを撃つ場合、目的地に到達してからレーザーを撃つまでの待機フレーム
	float laserSpeed;	// レーザーを撃つ場合、レーザーの移動速度
	int laserFireFrame; // レーザーを何フレーム発射し続けるか
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
	BossEnemyType bossType;						// ボスの種類
};

/// <summary>
/// 敵の管理クラス
/// </summary>
class EnemyManager
{
public:
	// コンストラクタ
	EnemyManager(std::shared_ptr<Player> pPlayer);

	// デストラクタ
	~EnemyManager();

	// 更新
	void Update();

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
	EnemyData LoadEnemyFileData(std::string filePath);	// 敵
	std::vector<EnemyActionData> LoadEnemyActionFileData(std::string filePath);	// 敵の行動

private:
	// ポインタ
	std::shared_ptr<Player> m_pPlayer;						// プレイヤー
	std::list<std::shared_ptr<EnemyBase>> m_pEnemyList;		// 雑魚敵リスト
	std::shared_ptr<BossEnemyBase> m_pBossEnemy;			// ボス敵

	// モデルハンドルテーブル
	std::map<EnemyType, int> m_modelHandleTable;			// 雑魚敵
	std::map<BossEnemyType, int> m_bossModelHandleTable;	// ボス敵

	// ウェーブ
	std::vector<WaveData> m_waveTable;	// ウェーブデータ
	int m_waveNow;		// 現在のウェーブ数
	bool m_isNextWave;	// 次のウェーブに移行するかどうか
	bool m_isLoadWave;	// ウェーブデータを読み込んだかどうか
};