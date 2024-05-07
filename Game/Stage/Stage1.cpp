#include "Stage1.h"
#include "../Scene/StageSelectScene.h"
#include "../Scene/SceneManager.h"
#include "../Editor/DataReaderFromUnity.h"
#include "../UI/UIManager.h"
#include "../UI/DamageFlash.h"
#include "../UI/ResultWindow.h"
#include "../Effect/ScreenShaker.h"
#include "../Effect/Effekseer3DEffectManager.h"
#include "../Game/Player.h"
#include "../Game/Camera.h"
#include "../Game/SkyDome.h"
#include "../Game/Shield.h"
#include "../Game/MeteorManager.h"
#include "../Game/PlanetManager.h"
#include "../Game/Laser/LaserManager.h"
#include "../Game/Laser/LaserBase.h"
#include "../Game/Enemy/EnemyManager.h"
#include "../Game/Enemy/EnemyBase.h"
#include "../Score/ScoreRanking.h"
#include "../Score/Score.h"
#include "../String/MessageManager.h"
#include "../MyDebug/DebugText.h"
#include "../UI/TutorialUI.h"
#include "../Util/InputState.h"
#include "../Transitor/Fade.h"
#include "../SoundManager.h"
#include <DxLib.h>

namespace
{
	// オブジェクト配置データのファイル名
	const std::string object_data_file_name = "Stage1";

	// 敵の配置データのファイル名
	const std::string enemy_data_file_name = "Stage1";

	// ウェーブの待機フレーム数
	constexpr int wave_wait_frame = 200;

	// ダメージ
	constexpr int meteor_damage = 2;			 // 隕石に当たっている間にプレイヤーに与えるダメージ
	constexpr int laser_damage = 1;				 // レーザーに当たっている間にプレイヤーに与えるダメージ
	constexpr int enemy_damage = 1;				 // 敵に当たっている間にプレイヤーに与えるダメージ
	constexpr int normal_enemy_reflect_laser_damage = 1000; // 反射レーザーに当たっている間に雑魚敵に与えるダメージ
	constexpr int boss_reflect_laser_damage = 2;			// 反射レーザーに当たっている間にボスに与えるダメージ
}

// コンストラクタ
Stage1::Stage1(SceneManager& manager) :
	StageBase(manager)
{
	// ダメージの設定
	m_meteorDamage = meteor_damage;
	m_laserDamage = laser_damage;
	m_enemyDamage = enemy_damage;
	m_playerToEnemyDamage = normal_enemy_reflect_laser_damage;
	m_playerToBossDamage = boss_reflect_laser_damage;

	// ステートマシンの設定
	m_stateMachine.AddState(State::START_ANIMATION, [this](){ EnterPlay(); }, [this]() { UpdateStartAnimation(); }, {});
	m_stateMachine.AddState(State::PLAY, {}, [this]() { UpdatePlay(); }, {});
	m_stateMachine.AddState(State::GAME_CLEAR, [this]() {EnterGameClear(); }, [this]() { UpdateGameClear(); }, {});
	m_stateMachine.AddState(State::GAME_OVER, [this]() {EnterGameOver(); }, [this]() { UpdateGameOver(); }, {});
	m_stateMachine.AddState(State::RESULT, [this]() { EnterResult(); }, [this]() { UpdateResult(); }, {});
	m_stateMachine.SetState(State::START_ANIMATION);

	// オブジェクト配置データ読み込み
	auto& dataReader = DataReaderFromUnity::GetInstance();
	dataReader.LoadUnityGameObjectData(object_data_file_name.c_str());

	// インスタンスの作成
	m_pPlayer = std::make_shared<Player>(object_data_file_name);
	m_pCamera = std::make_shared<Camera>(m_pPlayer->GetPos());
	m_pLaserManager = std::make_shared<LaserManager>(m_pPlayer, m_pCamera);
	m_pPlanetManager = std::make_shared<PlanetManager>(object_data_file_name);
	m_pMeteorManager = std::make_shared<MeteorManager>(object_data_file_name, m_pPlayer);
	m_pSkyDome = std::make_shared<SkyDome>(m_pCamera->GetPos());
	m_pScreenShaker = std::make_shared<ScreenShaker>(m_pCamera);
	m_pEnemyManager = std::make_shared<EnemyManager>(m_pPlayer, m_pLaserManager, m_pScreenShaker);

	// ポインタの設定
	m_pPlayer->SetCameraPointer(m_pCamera);

	// UIのインスタンスの作成
	m_pDamageFlash = std::make_shared<DamageFlash>();
	UIManager::GetInstance().AddUI("DamageFlash", m_pDamageFlash, 3, { 0, 0 });

	// ウェーブデータの読み込み
	m_pEnemyManager->LoadEnemyStageFileData(enemy_data_file_name);
}

// デストラクタ
Stage1::~Stage1()
{
}

// プレイステートの開始
void Stage1::EnterPlay()
{
	// BGMの再生
	SoundManager::GetInstance().PlayBGM("TutorialBgm");
	SoundManager::GetInstance().SetFadeSound("TutorialBgm", 120, 0, 255);
}

// リザルトステートの開始
void Stage1::EnterResult()
{
	// リザルト画面のインスタンス生成
	m_pResultWindow = std::make_shared<ResultWindow>();
}

// ゲームクリアステートの開始
void Stage1::EnterGameClear()
{
	SoundManager::GetInstance().PlaySE("GameClear");
}

// ゲームオーバーステートの開始
void Stage1::EnterGameOver()
{
	// BGMが再生中なら
	auto& soundManager = SoundManager::GetInstance();
	if (soundManager.IsPlayBGM())
	{
		// BGMのフェードアウト
		auto fileName = soundManager.GetPlayBGMFileName();
		soundManager.SetFadeSound(fileName, 60, soundManager.GetSoundVolume(fileName), 0);
	}
}

// 通常の更新
void Stage1::Update()
{
	// プレイヤーが死んだら
	if (!m_pPlayer->IsEnabled())
	{
		// ゲームオーバーに遷移
		m_stateMachine.SetState(State::GAME_OVER);
	}

	// ボスが死亡演出中なら隕石の削除
	if (m_pEnemyManager->IsDeadBossAnim())
	{
		m_pMeteorManager->DeleteAllMeteor();
	}	

	// 更新
	m_pSkyDome->Update({ 0, 0, m_pCamera->GetPos().z });	// スカイドーム
	m_pPlanetManager->UpdatePlay(m_pPlayer->GetMoveVec());	// 惑星
	m_pLaserManager->Update();								// レーザー
	m_pEnemyManager->Update();								// レーザー
	m_pDamageFlash->Update();								// ダメージフラッシュ
	m_pScreenShaker->Update();								// 画面揺れ
	Effekseer3DEffectManager::GetInstance().Update();		// エフェクト
	UIManager::GetInstance().Update();						// UI

	Collision();										// 当たり判定
	m_pFade->Update();									// フェード
	m_stateMachine.Update();							// ステートマシン
}

// スタート演出ステートの更新
void Stage1::UpdateStartAnimation()
{
	// 更新
	m_pPlayer->UpdateStart(m_pCamera->GetPos());	// プレイヤー
	m_pCamera->UpdateStart(m_pPlayer->GetPos());	// カメラ
	m_pMeteorManager->UpdateStart();				// 隕石

	// 小さい隕石の生成
	m_pMeteorManager->CreateSmallMeteor(5000.0f);	

	// スタート演出が終わったらプレイ中に遷移
	if (m_pPlayer->IsStartAnimation() &&
		m_pCamera->IsStartAnimation())
	{
		m_stateMachine.SetState(State::PLAY);
	}
}

// プレイステートの更新
void Stage1::UpdatePlay()
{
	// ボスが死んだらゲームクリアに遷移
	if (m_pEnemyManager->IsDeadBoosEndAnim())
	{
		m_stateMachine.SetState(State::GAME_CLEAR);
		return;
	}

	// 小さい隕石の生成
	m_pMeteorManager->CreateSmallMeteor(5000.0f);

	// 現在の敵のウェーブが終了したら
	if (m_pEnemyManager->IsEndWave())
	{
		if (m_currentFrame++ > wave_wait_frame)
		{
			// ウェーブ開始
	    	m_pEnemyManager->StartWave();

			// 次のウェーブに進む
			m_pEnemyManager->NextWave();

			// ウェーブの待機フレーム数をリセット
			m_currentFrame = 0;
		}
	}

	// 更新
	m_pPlayer->UpdatePlay(m_pCamera->GetCameraHorizon());
	m_pCamera->UpdatePlay(m_pPlayer->GetPos(), m_pPlayer->GetMoveVec());
	m_pMeteorManager->Update(m_pCamera->GetPos());			// 隕石
}

// ゲームクリアステートの更新
void Stage1::UpdateGameClear()
{
	// 全てのレーザーの削除
	m_pLaserManager->DeleteAllLaser();

	// ゲームクリア時の更新
	m_pPlayer->UpdateGameClear();
	m_pMeteorManager->Update(m_pCamera->GetPos());

	// カメラの更新が終了したら
	if (m_pCamera->UpdateGameClear(m_pPlayer->GetPos()))
	{
		// リザルト画面に遷移
		m_stateMachine.SetState(State::RESULT);
	}
}

// ゲームオーバーステートの更新
void Stage1::UpdateGameOver()
{
	// UIの格納
	UIManager::GetInstance().Store();

	// 全てのレーザーの削除
	m_pLaserManager->DeleteAllLaser();

	// ゲームオーバー時の更新
	m_pCamera->UpdateGameOver(m_pPlayer->GetPos());
	m_pMeteorManager->Update(m_pCamera->GetPos());
	if (m_pPlayer->UpdateGameOver())
	{
		// フェードアウトの演出の開始
		m_pFade->StartFadeOut(255, 2);
	}
	// フェードアウトが終了したら
	if (m_pFade->IsFadeOutEnd())
	{
		// ステージセレクトに遷移
		m_manager.ChangeScene(std::make_shared<StageSelectScene>(m_manager));
	}

	// デバッグテキストの追加
	DebugText::AddLog("GameOver");
}

// リザルトステートの更新
void Stage1::UpdateResult()
{
	StageBase::UpdateResult("Stage1");
}

// 描画
void Stage1::Draw()
{
	// 画面揺れの前処理
	m_pScreenShaker->PreDraw();

	// 描画
	m_pSkyDome->Draw();			// スカイドーム
	m_pPlanetManager->Draw();	// 惑星
	m_pMeteorManager->Draw();	// 隕石
	m_pEnemyManager->Draw();	// 敵
	m_pLaserManager->Draw();	// レーザー
	m_pPlayer->Draw();								// プレイヤー
	m_pPlayer->DrawShield();						// シールド
	Effekseer3DEffectManager::GetInstance().Draw();	// エフェクト
	UIManager::GetInstance().Draw();				// UI
	Score::GetInstance().DrawScore();				// スコア

	// リザルト画面が開始されていたら
	if (m_stateMachine.GetCurrentState() == State::RESULT)
	{
		// リザルト画面の描画
		m_pResultWindow->Draw();
	}

	// 画面揺れ描画
	m_pScreenShaker->Draw();

	// フェードの描画
	m_pFade->DrawFade(true);
}