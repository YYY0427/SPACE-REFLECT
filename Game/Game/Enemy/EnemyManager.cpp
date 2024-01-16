#include "EnemyManager.h"
#include "BossEnemyBase.h"
#include "EnemyBase.h"
#include "Mosquito.h"
#include "BossMosquito.h"
#include "../../StringManager.h"
#include "../../Util/DrawFunctions.h"
#include <fstream>
#include <sstream>
#include <cassert>

namespace
{
	// ファイルの階層
	const std::string wave_file_hierarchy = "Data/Wave/";					// ウェーブ
	const std::string enemy_file_hierarchy = "Data/Enemy/";					// 敵
	const std::string enemy_action_file_hierarchy = "Data/EnemyAction/";	// 敵の行動

	// ファイルの拡張子
	const std::string file_extension = ".csv";				

	// 敵のモデルのファイルパス
	const std::string mosquito_model_file_path = "Data/Model/Mosquito.mv1";	// 蚊
}

// コンストラクタ
EnemyManager::EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<LaserManager> pLaserManager) :
	m_waveNow(0),
	m_isNextWave(false),
	m_isLoadWave(false),
	m_pPlayer(pPlayer),
	m_pLaserManager(pLaserManager)
{
	// 雑魚敵モデルハンドルの読み込み
	m_modelHandleTable[EnemyType::MOSQUITO] = my::MyLoadModel(mosquito_model_file_path.c_str());
	
	// ボス敵モデルハンドルの読み込み
	m_bossModelHandleTable[BossEnemyType::NONE] = -1;
	m_bossModelHandleTable[BossEnemyType::MOSQUITO] = m_modelHandleTable[EnemyType::MOSQUITO];
}

// デストラクタ
EnemyManager::~EnemyManager()
{
}

// 更新
void EnemyManager::Update()
{
	// 雑魚敵の更新
	for (auto& enemy : m_pEnemyList)
	{
		enemy->Update();
	}

	// ボス敵が出現していたら更新
	if(m_pBossEnemy)
	{
		m_pBossEnemy->Update();
	}
}

// 描画
void EnemyManager::Draw()
{
	// 雑魚敵の描画
	for (auto& enemy : m_pEnemyList)
	{
		enemy->Draw();
	}

	// ボス敵が出現していたら描画
	if (m_pBossEnemy)
	{
		m_pBossEnemy->Draw();
	}
}

// ウェーブのスタート
void EnemyManager::StartWave()
{
	// ウェーブデータを読み込んでいなかったら止める
	if (!m_isLoadWave)
	{
		assert(!"ウェーブデータを読み込んでください");
	}

	// 最初のウェーブデータを実行
	auto& waveData = m_waveTable.front();

	// 雑魚敵の生成
	for (auto& data : waveData.enemyDataList)
	{
		AddEnemy(data);
	}
}

// 次のウェーブへ
void EnemyManager::NextWave()
{
	// まだ敵が残っていたらなにもしない
	for (auto& enemy : m_pEnemyList)
	{
		if (enemy->IsEnabled())
		{
			return;
		}
	};

	// ボス敵が出現していたらなにもしない
	if (m_pBossEnemy)
	{
		return;
	};

	// ウェーブを進める
	m_waveNow++;
	auto& waveData = m_waveTable[m_waveNow];

	// 最後のウェーブだったらボス敵を生成
	if(m_waveNow == m_waveTable.size() - 1)
	{
		// ボス敵の生成
		AddBossEnemy(waveData.bossType);
	}
	else
	{
		// 雑魚敵の生成
		for (auto& data : waveData.enemyDataList)
		{
			AddEnemy(data);
		}
	}
}

// 敵の追加
void EnemyManager::AddEnemy(EnemyData data)
{
	// 敵の種類によって生成する敵を変える
	switch (data.type)
	{
	// 蚊
	case EnemyType::MOSQUITO:
		m_pEnemyList.push_back(std::make_shared<Mosquito>(
			m_modelHandleTable[data.type], 
			data,
			m_pPlayer,
			m_pLaserManager));
		break;

	default:
		// ここに来たら敵の種類が追加されていない
		assert(!"敵の種類がありません");
	}
}

// ボス敵の追加
void EnemyManager::AddBossEnemy(BossEnemyType type)
{
	// 既にボス敵が生成されていたら
	if(m_pBossEnemy)
	{
		return;
	}

	// 種類によってボス敵の生成
	switch (type)
	{	
	case BossEnemyType::MOSQUITO:
		m_pBossEnemy = std::make_shared<BossMosquito>(m_bossModelHandleTable[type]);
		break;
	case BossEnemyType::NONE:
		break;
	default:
		// ここに来たら敵の種類が追加されていない
		assert(!"ボス敵の種類がありません");
	}
}

// ウェーブのデータの読み込み
void EnemyManager::LoadWaveFileData(std::string filePath)
{
	// フラグを立てる
	m_isLoadWave = true;

	// ファイル情報の読み込み(読み込みに失敗したら止める)
	std::string localFilePath = wave_file_hierarchy + filePath + file_extension;
	std::ifstream ifs(localFilePath);
	assert(ifs && "Waveデータの読み込み失敗");

	// csvデータを1行ずつ読み取る
	bool isFirst = false;
	std::string line;
	while (getline(ifs, line))
	{
		// 1行目は読み込まない
		// 1行目には項目が書いてあるため
		if (!isFirst)
		{
			isFirst = true;
			continue;
		}

		// csvデータ１行を','で複数の文字列に変換
		std::vector<std::string> strvec = StringManager::GetInstance().SplitString(line, ',');

		WaveData data{};

		// 最後の行はボス敵の種類の読み込み
		if (ifs.eof())
		{
			data.bossType = static_cast<BossEnemyType>(std::stoi(strvec[0]));
		}
		// それ以外は雑魚敵のデータの読み込み
		else
		{
			// 敵のデータの読み込み
			for (auto& str : strvec)
			{
				// 敵のデータの追加
				data.enemyDataList.push_back(LoadEnemyFileData(str));

				// ボス敵の種類は無効
				data.bossType = BossEnemyType::NONE;
			}
		}
		// ウェーブのデータの追加
		m_waveTable.push_back(data);
	}
}

// 敵のデータの読み込み
EnemyData EnemyManager::LoadEnemyFileData(std::string filePath)
{
	// ファイル情報の読み込み(読み込みに失敗したら止める)
	std::string localFilePath = enemy_file_hierarchy + filePath + file_extension;
	std::ifstream ifs(localFilePath);
	assert(ifs && "Enemyデータの読み込み失敗");

	// 初期化
	EnemyData data{};
	bool isFirst = false;
	std::string line;

	// csvデータを1行ずつ読み取る
	while (getline(ifs, line))
	{
		// 1行目は読み込まない
		// 1行目には項目が書いてあるため
		if (!isFirst)
		{
			isFirst = true;
			continue;
		}

		// csvデータ１行を','で複数の文字列に変換
		std::vector<std::string> strvec = StringManager::GetInstance().SplitString(line, ',');

		// 座標の読み込み
		data.pos.x = std::stof(strvec[0]);
		data.pos.y = std::stof(strvec[1]);
		data.pos.z = std::stof(strvec[2]);

		// 種類の読み込み
		data.type = static_cast<EnemyType>(std::stoi(strvec[3]));

		// HPの読み込み
		data.hp = std::stoi(strvec[4]);

		// 攻撃力の読み込み
		data.attack = std::stoi(strvec[5]);

		// 移動速度の読み込み
		data.speed = std::stof(strvec[6]);

		// 大きさの読み込み
		data.scale = std::stof(strvec[7]);

		// 行動データの読み込み
		data.actionDataList = LoadEnemyActionFileData(strvec[8]);
	}
	return data;
}

// 敵の行動のデータの読み込み
std::vector<EnemyActionData> EnemyManager::LoadEnemyActionFileData(std::string filePath)
{
	// ファイル情報の読み込み(読み込みに失敗したら止める)
	std::string localFilePath = enemy_action_file_hierarchy + filePath + file_extension;
	std::ifstream ifs(localFilePath);
	assert(ifs && "EnemyActionデータの読み込み失敗");

	// 初期化
	std::vector<EnemyActionData> dataTable;
	bool isFirst = false;
	std::string line;

	// csvデータを1行ずつ読み取る
	while (getline(ifs, line))
	{
		// 1行目は読み込まない
		// 1行目には項目が書いてあるため
		if (!isFirst)
		{
			isFirst = true;
			continue;
		}

		// csvデータ１行を','で複数の文字列に変換
		std::vector<std::string> strvec = StringManager::GetInstance().SplitString(line, ',');

		EnemyActionData data{};

		// 目的地の読み込み
		data.goalPos.x = std::stof(strvec[0]);
		data.goalPos.y = std::stof(strvec[1]);
		data.goalPos.z = std::stof(strvec[2]);

		// 目的地に到達してから次の目的地に向かうまでの待機フレームの読み込み
		data.idleFrame = std::stoi(strvec[3]);

		// 目的地に到達したらレーザーを発射するかどうかのフラグの読み込み
		data.isLaser = std::stoi(strvec[4]);

		// レーザーを発射する場合
		if (data.isLaser)
		{
			// レーザーの種類の読み込み
		//	data.laserType = static_cast<LaserType>(std::stoi(strvec[5]));

			// レーザーを発射するまでの待機フレームの読み込み
			data.laserIdleFrame = std::stoi(strvec[6]);

			// レーザーの速度の読み込み
			data.laserSpeed = std::stof(strvec[7]);

			// レーザーを何フレームの間、発射するかの読み込み
			data.laserFireFrame = std::stoi(strvec[8]);
		}

		// データの追加
		dataTable.push_back(data);
	}
	return dataTable;
}
