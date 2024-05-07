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
#include "../../Util/InputState.h"
#include "../../Util/FileUtil.h"
#include "../Player.h"
#include "../../SoundManager.h"
#include <fstream>
#include <sstream>
#include <cassert>
#include <DxLib.h>

namespace
{
	// ファイルの階層
	const std::string wave_file_hierarchy = "Data/Csv/EnemyStage/";					// ステージ
	const std::string enemy_file_hierarchy = "Data/Csv/EnemyWave/";					// ウェーブ
	const std::string enemy_action_file_hierarchy = "Data/Csv/EnemyAction/";	// 敵の行動

	// ファイルの拡張子
	const std::string file_extension = ".csv";				

	// ボス敵登場時の警告のフレーム
	constexpr int warning_frame = 180;

	// デバッグ用の敵に与えるダメージ
	constexpr int debug_damage = 100000;

	// ボス登場時のBGMのフェードインのフレーム
	constexpr int boss_bgm_fade_frame = 120;

	// 警告UIの描画優先度
	constexpr int warning_draw_priority = 0;

	// 警告UIの格納時のベクトル
	const Vector2 warning_ui_store_vec = { 0, 0 };
}

// コンストラクタ
EnemyManager::EnemyManager(const std::shared_ptr<Player>& pPlayer, 
						   const std::shared_ptr<LaserManager>& pLaserManager, 
						   const std::shared_ptr<ScreenShaker>& pScreenShaker) :
	m_waveNow(0),
	m_isNextWave(false),
	m_isLoadWave(false),
	m_isDeadBoss(false),
	m_isDeadEffectSound(false),
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
	// デバッグ用
#ifdef _DEBUG
	// 敵の全滅デバッグ
	if (InputState::IsTriggered(InputType::ENEMY_DETH_DEBUG))
	{
		for (auto& enemy : m_pEnemyList)
		{
			enemy->OnDamage(debug_damage, Vector3());
		}
		if (m_pBossEnemy)
		{
			m_pBossEnemy->OnDamage(debug_damage, Vector3());
		}
	}
#endif

	// ボスの死亡演出中だったら
	if (IsDeadBossAnim() && !m_isDeadEffectSound)
	{
		// ボス死亡エフェクト音の再生
		SoundManager::GetInstance().PlaySE("BossDeathEffect");

		// BGMの停止
		SoundManager::GetInstance().StopBGM();

		// フラグを立てる
		m_isDeadEffectSound = true;
	}

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

		// 現在流れているBGMのフェードアウトの設定
		auto& soundManager = SoundManager::GetInstance();
		soundManager.SetFadeSound(soundManager.GetPlayBGMFileName(), boss_bgm_fade_frame, soundManager.GetMaxVolume(), 0);

		// ボス敵のBGMの再生
		soundManager.PlayBGM("BossBatleBgm");

		// ボス敵のBGMのフェードインの設定
		soundManager.SetFadeSound("BossBatleBgm", boss_bgm_fade_frame, 0, soundManager.GetMaxVolume());

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

	// 雑魚敵の座標の描画
	for (int i = 0; i < m_pEnemyList.size(); i++)
	{
		auto itr = m_pEnemyList.begin();
		std::advance(itr, i);

		DebugText::AddLog("NormalEnemyPos", { itr->get()->GetPos().x, itr->get()->GetPos().y, itr->get()->GetPos().z });
	}
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

	// ウェーブデータがなかったら
	if (m_waveTable.empty())
	{
		// ボス敵がいなかったら
		if (m_bossType == BossEnemyType::NONE)
		{
			// ボス敵が倒されたことにしてなにもしない
			m_isBoss     = true;
			m_isDeadBoss = true;
			return;
		}

		// 警告状態に遷移
		m_stateMachine.SetState(State::WARNING);

		// 警告の生成
		m_pWarning = std::make_shared<Warning>(warning_frame);
		UIManager::GetInstance().AddUI("Warning", m_pWarning, warning_draw_priority, warning_ui_store_vec);
	}
	else
	{
		// 最初のウェーブデータを実行
		auto& waveData = m_waveTable.front();

		// 雑魚敵の生成
		for (auto& data : waveData.enemyDataList)
		{
			AddEnemy(data);
		}
	}
}

// 次のウェーブへ
void EnemyManager::NextWave()
{
	// ウェーブが始まっていなかったらなにもしない
	if(!m_isStartWave) return;

	// ウェーブが終わっていなかったらなにもしない
	if (!IsEndWave())	return;

	// データがなかったらなにもしない
	if(m_waveTable.empty()) return;

	// 最後のウェーブだったらボス敵を生成
	if(m_waveNow == m_waveTable.size() - 1)
	{
		// ボス敵がいなかったら
		if (m_bossType == BossEnemyType::NONE)
		{
			// ボス敵が倒されたことにする
			m_isBoss     = true;
			m_isDeadBoss = true;
		}
		// まだ警告が出ていなかったら
		else if(!m_pWarning)
		{
			// 警告状態に遷移
			m_stateMachine.SetState(State::WARNING);

			// 警告の生成
			m_pWarning = std::make_shared<Warning>(warning_frame);
			UIManager::GetInstance().AddUI("Warning", m_pWarning, warning_draw_priority, warning_ui_store_vec);
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
void EnemyManager::AddEnemy(const EnemyData& data)
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
void EnemyManager::AddBossEnemy(const BossEnemyType& type)
{
	// 既にボス敵が生成されていたら
	if(m_pBossEnemy || m_isBoss) return;

	// フラグを立てる
	m_isBoss = true;

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
void EnemyManager::LoadEnemyStageFileData(const std::string& filePath)
{
	// フラグを立てる
	m_isLoadWave = true;

	// ファイルの読み込み
	auto data = FileUtil::LoadCsvFile(wave_file_hierarchy + filePath + file_extension);
	for (auto& line : data)
	{
		// 敵のデータの読み込み
		WaveData waveData{};
		for (int i = 0; i < line.size(); i++)
		{
			if (i == line.size() - 1)
			{
				// ボス敵の種類の読み込み
				m_bossType = static_cast<BossEnemyType>(std::stoi(line[i]));
			}
			else
			{
				// 敵のデータの追加
				waveData.enemyDataList = LoadEnemyWaveFileData(line[i]);

				// ウェーブのデータの追加
				m_waveTable.push_back(waveData);
			}
		}
	}
}

// 敵のデータの読み込み
std::vector<EnemyData> EnemyManager::LoadEnemyWaveFileData(const std::string& filePath)
{
	// ファイル情報の読み込み(読み込みに失敗したら止める)
	auto data = FileUtil::LoadCsvFile(enemy_file_hierarchy + filePath + file_extension);
	std::vector<EnemyData> dataTable;
	for (auto& line : data)
	{
		// 座標の読み込み
		EnemyData enemyData{};
		enemyData.pos.x = std::stof(line[static_cast<int>(EnemyWaveIndex::POS_X)]);
		enemyData.pos.y = std::stof(line[static_cast<int>(EnemyWaveIndex::POS_Y)]);
		enemyData.pos.z = std::stof(line[static_cast<int>(EnemyWaveIndex::POS_Z)]);

		// 種類の読み込み
		enemyData.type = static_cast<EnemyType>(std::stoi(line[static_cast<int>(EnemyWaveIndex::TYPE)]));

		// HPの読み込み
		enemyData.hp = std::stoi(line[static_cast<int>(EnemyWaveIndex::HP)]);

		// 攻撃力の読み込み
		enemyData.attack = std::stoi(line[static_cast<int>(EnemyWaveIndex::ATTACK)]);

		// 大きさの読み込み
		enemyData.scale = std::stof(line[static_cast<int>(EnemyWaveIndex::SCALE)]);

		// 移動速度の読み込み
		enemyData.speed = std::stof(line[static_cast<int>(EnemyWaveIndex::SPEED)]);

		// 行動データの読み込み
		enemyData.actionDataList = LoadEnemyActionFileData(line[static_cast<int>(EnemyWaveIndex::ACTION)]);

		// データの追加
		dataTable.push_back(enemyData);
	}
	return dataTable;
}

// 敵の行動のデータの読み込み
std::vector<EnemyActionData> EnemyManager::LoadEnemyActionFileData(const std::string& filePath)
{
	auto data = FileUtil::LoadCsvFile(enemy_action_file_hierarchy + filePath + file_extension);
	std::vector<EnemyActionData> dataTable;
	for (auto& line : data)
	{
		EnemyActionData enemyActionData{};

		// 目的地の読み込み
		enemyActionData.goalPos.x = std::stof(line[static_cast<int>(EnemyActionIndex::GOAL_POS_X)]);
		enemyActionData.goalPos.y = std::stof(line[static_cast<int>(EnemyActionIndex::GOAL_POS_Y)]);
		enemyActionData.goalPos.z = std::stof(line[static_cast<int>(EnemyActionIndex::GOAL_POS_Z)]);

		// 目的地に到達してから次の目的地に向かうまでの待機フレームの読み込み
		enemyActionData.idleFrame = std::stoi(line[static_cast<int>(EnemyActionIndex::IDLE_FRAME)]);

		// 目的地に到達したらレーザーを発射するかどうかのフラグの読み込み
		enemyActionData.isLaser = std::stoi(line[static_cast<int>(EnemyActionIndex::IS_LASER)]);

		// レーザーを発射する場合
		if (enemyActionData.isLaser)
		{
			// レーザーの種類の読み込み
			enemyActionData.laserType = static_cast<LaserType>(std::stoi(line[static_cast<int>(EnemyActionIndex::LASER_TYPE)]));

			// レーザーを発射するまでのチャージフレームの読み込み
			enemyActionData.laserChargeFrame = std::stoi(line[static_cast<int>(EnemyActionIndex::LASER_CHARGE_FRAME)]);

			// レーザーを発射するまでの待機フレームの読み込み
			enemyActionData.laserIdleFrame = std::stoi(line[static_cast<int>(EnemyActionIndex::LASER_IDLE_FRAME)]);

			// レーザーの速度の読み込み
			enemyActionData.laserSpeed = std::stof(line[static_cast<int>(EnemyActionIndex::LASER_SPEED)]);

			// レーザーを何フレームの間、発射するかの読み込み
			enemyActionData.laserFireFrame = std::stoi(line[static_cast<int>(EnemyActionIndex::LASER_FIRE_FRAME)]);

			// レーザーがプレイヤーを追従するかどうかのフラグの読み込み
			enemyActionData.isPlayerFollowing = std::stoi(line[static_cast<int>(EnemyActionIndex::IS_PLAYER_FOLLOWING)]);

		}
		// データの追加
		dataTable.push_back(enemyActionData);
	}
	return dataTable;
}

// ボス敵が倒されて、死亡演出が終了したかどうか
bool EnemyManager::IsDeadBoosEndAnim() const
{
	return m_isBoss && m_isDeadBoss;
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

// ボス敵が生存しているかどうか
bool EnemyManager::IsBossAlive() const
{
	return (m_pBossEnemy && m_isBoss);
}

// ボスが倒され、死亡演出中かどうかの取得
bool EnemyManager::IsDeadBossAnim() const
{
	// ボス敵がいたら
	if (m_pBossEnemy)
	{
		return m_pBossEnemy->IsDeadAnim();
	}
	return false;
}