#include "EnemyManager.h"
#include "EnemyBase.h"
#include "Normal/Mosquito.h"
#include "Boss/BossMosquito.h"
#include "Boss/BossMatrix.h"
#include "../../String/StringUtil.h"
#include "../../Util/DrawFunctions.h"
#include "../../UI/Warning.h"
#include "../../UI/UIManager.h"
#include "../../MyDebug/DebugText.h"
#include "../Player.h"
#include <fstream>
#include <sstream>
#include <cassert>
#include <DxLib.h>

namespace
{
	// ファイルの階層
	const std::string wave_file_hierarchy = "Data/Wave/";					// ウェーブ
	const std::string enemy_file_hierarchy = "Data/Enemy/";					// 敵
	const std::string enemy_action_file_hierarchy = "Data/EnemyAction/";	// 敵の行動

	// ファイルの拡張子
	const std::string file_extension = ".csv";				

	// ボス敵登場時の警告のフレーム
	constexpr int warning_frame = 180;
}

// コンストラクタ
EnemyManager::EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<LaserManager> pLaserManager, std::shared_ptr<ScreenShaker> pScreenShaker) :
	m_waveNow(0),
	m_isNextWave(false),
	m_isLoadWave(false),
	m_isDeadBoss(false),
	m_isBoss(false),
	m_pPlayer(pPlayer),
	m_pLaserManager(pLaserManager),
	m_pScreenShaker(pScreenShaker),
	m_bossType(BossEnemyType::NONE),
	m_isStartWave(false)
{
	// ステートマシンの設定
	m_stateMachine.AddState(State::NORMAL, {}, [this]() {UpdateNormal(); }, {});
	m_stateMachine.AddState(State::WARNING, {}, [this]() {UpdateWarning(); }, {});
	m_stateMachine.SetState(State::NORMAL);
}

// デストラクタ
EnemyManager::~EnemyManager()
{
}

// 更新
void EnemyManager::Update()
{
	// ステートマシンの更新
	m_stateMachine.Update();
}

// 通常時の更新
void EnemyManager::UpdateNormal()
{
	// 存在フラグが下がっている敵の削除
	m_pEnemyList.remove_if([](std::shared_ptr<EnemyBase> data) { return !data->IsEnabled(); });

	// 雑魚敵の更新
	for (auto& enemy : m_pEnemyList)
	{
		enemy->Update();
	}

	// ボス敵が出現していたら
	if (m_pBossEnemy)
	{
		// ボス敵が出現していて、かつボス敵が倒されていたら
		if (!m_pBossEnemy->IsEnabled())
		{
			// ボス敵が倒されたことにする
			m_isDeadBoss = true;
		}

		// 更新
		m_pBossEnemy->Update();
	}
}

// 警告時の更新
void EnemyManager::UpdateWarning()
{
	// 警告が終了したら
	if (m_pWarning->IsEnd())
	{
		// 警告の削除
		UIManager::GetInstance().DeleteUI("Warning");

		// ボス敵の生成
		AddBossEnemy(m_bossType);

		// ステートを通常に遷移
		m_stateMachine.SetState(State::NORMAL);
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

	// デバッグ用
#ifdef _DEBUG
	// 雑魚敵の座標の描画
	for (int i = 0; i < m_pEnemyList.size(); i++)
	{
		auto itr = m_pEnemyList.begin();
		std::advance(itr, i);

		DebugText::Log("NormalEnemyPos", { itr->get()->GetPos().x, itr->get()->GetPos().y, itr->get()->GetPos().z });
	}
#endif
}

// ウェーブのスタート
void EnemyManager::StartWave()
{
	// 既にウェーブが始まっていたらなにもしない
	if (m_isStartWave) return;

	// フラグを立てる
	m_isStartWave = true;

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
	if (m_pBossEnemy)	return;

	// 最後のウェーブだったらボス敵を生成
	if(m_waveNow == m_waveTable.size() - 1)
	{
		// ボス敵がいなかったら
		if (m_bossType == BossEnemyType::NONE)
		{
			// ボス敵が倒されたことにする
			m_isDeadBoss = true;
		}
		else
		{
			// 警告状態に遷移
			m_stateMachine.SetState(State::WARNING);

			// 警告の生成
			m_pWarning = std::make_shared<Warning>(warning_frame);
			UIManager::GetInstance().AddUI("Warning", m_pWarning, 0, { 0, 0 });
		}
	}
	else
	{
		// ウェーブを進める
		m_waveNow++;
		auto& waveData = m_waveTable[m_waveNow];

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
		m_pBossEnemy = std::make_shared<BossMosquito>(
			m_pPlayer,
			m_pLaserManager);
		break;

	case BossEnemyType::MATRIX:
		m_pBossEnemy = std::make_shared<BossMatrix>(
			m_pPlayer,
			m_pLaserManager,
			m_pScreenShaker);
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
		std::vector<std::string> strvec = StringUtil::Split(line, ',');

		// 敵のデータの読み込み
		WaveData data{};
		for (int i = 0; i < strvec.size(); i++)
		{
			if(i == strvec.size() - 1)
			{
				// ボス敵の種類の読み込み
				m_bossType = static_cast<BossEnemyType>(std::stoi(strvec[i]));
			}
			else
			{
				// 敵のデータの追加
				data.enemyDataList = LoadEnemyFileData(strvec[i]);

				// ウェーブのデータの追加
				m_waveTable.push_back(data);
			}
		}
	}
}

// 敵のデータの読み込み
std::vector<EnemyData> EnemyManager::LoadEnemyFileData(std::string filePath)
{
	// ファイル情報の読み込み(読み込みに失敗したら止める)
	std::string localFilePath = enemy_file_hierarchy + filePath + file_extension;
	std::ifstream ifs(localFilePath);
	assert(ifs && "Enemyデータの読み込み失敗");

	// 初期化
	std::vector<EnemyData> dataTable{};
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
		std::vector<std::string> strvec = StringUtil::Split(line, ',');

		EnemyData data{};

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

		// 大きさの読み込み
		data.scale = std::stof(strvec[6]);

		// 移動速度の読み込み
		data.speed = std::stof(strvec[7]);

		// 行動データの読み込み
		data.actionDataList = LoadEnemyActionFileData(strvec[8]);

		// データの追加
		dataTable.push_back(data);
	}
	return dataTable;
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
		std::vector<std::string> strvec = StringUtil::Split(line, ',');

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
			data.laserType = static_cast<LaserType>(std::stoi(strvec[5]));

			// レーザーを発射するまでのチャージフレームの読み込み
			data.laserChargeFrame = std::stoi(strvec[6]);

			// レーザーを発射するまでの待機フレームの読み込み
			data.laserIdleFrame = std::stoi(strvec[7]);

			// レーザーの速度の読み込み
			data.laserSpeed = std::stof(strvec[8]);

			// レーザーを何フレームの間、発射するかの読み込み
			data.laserFireFrame = std::stoi(strvec[9]);

			// レーザーがプレイヤーを追従するかどうかのフラグの読み込み
			data.isPlayerFollowing = std::stoi(strvec[10]);
		}

		// データの追加
		dataTable.push_back(data);
	}
	return dataTable;
}

// ボス敵の死亡判定
bool EnemyManager::IsDeadBoss() const
{
	return m_isDeadBoss;
}

// 現在のウェーブが終了したかどうか
bool EnemyManager::IsEndWave() const
{
	// まだ敵が残っていたら
	for (auto& enemy : m_pEnemyList)
	{
		if (enemy->IsEnabled())
		{
			return false;
		}
	}

	// ボス敵が出現していたら
	if (m_pBossEnemy)
	{
		return false;
	}

	// それ以外はウェーブが終了している
	return true;
}

// 雑魚敵のリストの取得
const std::list<std::shared_ptr<EnemyBase>>& EnemyManager::GetEnemyList() const
{
	return m_pEnemyList;
}

// ボス敵の取得
const std::shared_ptr<EnemyBase>& EnemyManager::GetBossEnemy() const
{
	return m_pBossEnemy;
}