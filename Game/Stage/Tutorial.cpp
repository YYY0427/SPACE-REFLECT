#include "Tutorial.h"
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
	const std::string object_data_file_name = "Tutorial";

	// 敵の配置データのファイル名
	const std::string enemy_data_file_name = "Tutorial";

	// ウェーブの待機フレーム数
	constexpr int wave_wait_frame = 200;

	// 移動チュートリアルに遷移するまでのフレーム数
	constexpr int move_tutorial_frame = 200;

	// ダメージ
	constexpr int meteor_damage = 2;			 // 隕石に当たっている間にプレイヤーに与えるダメージ
	constexpr int laser_damage = 1;				 // レーザーに当たっている間にプレイヤーに与えるダメージ
	constexpr int enemy_damage = 1;				 // 敵に当たっている間にプレイヤーに与えるダメージ
	constexpr int normal_enemy_reflect_laser_damage = 1000;  // 反射レーザーに当たっている間に通常の敵に与えるダメージ
	constexpr int boss_reflect_laser_damage = 2;			 // 反射レーザーに当たっている間にボスに与えるダメージ
}

// コンストラクタ
Tutorial::Tutorial(SceneManager& manager) :
	StageBase(manager)
{
	// ダメージの設定
	m_meteorDamage= meteor_damage;
	m_laserDamage = laser_damage;
	m_enemyDamage = enemy_damage;
	m_playerToEnemyDamage = normal_enemy_reflect_laser_damage;
	m_playerToBossDamage = boss_reflect_laser_damage;

	// ステートマシンの設定
	m_stateMachine.AddState(State::START_ANIMATION, {}, [this]() { UpdateStartAnimation(); }, {});
	m_stateMachine.AddState(State::MOVE_TUTORIAL, [this]() { EnterStartAnimation(); }, [this]() { UpdateMoveTutorial(); }, {});
	m_stateMachine.AddState(State::SHIELD_TUTORIAL, {}, [this]() { UpdateShieldTutorial(); }, {});
	m_stateMachine.AddState(State::REFLECT_TUTORIAL, {}, [this]() { UpdateReflectTutorial(); }, {});
	m_stateMachine.AddState(State::CUBE_TUTORIAL, {}, [this]() { UpdateCubeTutorial(); }, {});
	m_stateMachine.AddState(State::PLAY, {}, [this]() { UpdatePlay(); }, {});
	m_stateMachine.AddState(State::GAME_CLEAR, [this]() {EnterGameClear(); }, [this]() { UpdateGameClear(); }, {});
	m_stateMachine.AddState(State::GAME_OVER, {}, [this]() { UpdateGameOver(); }, {});
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
	m_pScreenShaker= std::make_shared<ScreenShaker>(m_pCamera);
	m_pEnemyManager = std::make_shared<EnemyManager>(m_pPlayer, m_pLaserManager, m_pScreenShaker);
	m_pTutorialUI = std::make_shared<TutorialUI>();

	// ポインタの設定
	m_pPlayer->SetCameraPointer(m_pCamera);

	// UIのインスタンスの作成
	m_pDamageFlash = std::make_shared<DamageFlash>();
	UIManager::GetInstance().AddUI("DamageFlash", m_pDamageFlash, 3, { 0, 0 });

	// ウェーブデータの読み込み
	m_pEnemyManager->LoadEnemyStageFileData(enemy_data_file_name);
}

// デストラクタ
Tutorial::~Tutorial()
{
}

// 更新
void Tutorial::Update()
{
	// プレイヤーが死んだら
	if (!m_pPlayer->IsEnabled())
	{
		// ゲームオーバーに遷移
		m_stateMachine.SetState(State::GAME_OVER);
	}

	// 小さい隕石の生成
	m_pMeteorManager->CreateSmallMeteor(5000.0f);

	// 更新
	m_pTutorialUI->Update();								// チュートリアルUI
	m_pSkyDome->Update({ 0, 0, m_pCamera->GetPos().z });	// スカイドーム
	m_pPlanetManager->UpdatePlay(m_pPlayer->GetMoveVec());	// 惑星
	m_pLaserManager->Update();								// レーザー
	m_pEnemyManager->Update();								// 敵
	m_pDamageFlash->Update();								// ダメージフラッシュ
	m_pScreenShaker->Update();								// 画面揺れ
	Effekseer3DEffectManager::GetInstance().Update();		// エフェクト
	UIManager::GetInstance().Update();						// UI

	Collision();										// 当たり判定
	m_pFade->Update();									// フェード
	m_stateMachine.Update();							// ステートマシン
}

// スタート演出の更新
void Tutorial::UpdateStartAnimation()
{
	// 更新
	m_pPlayer->UpdateStart(m_pCamera->GetPos());	// プレイヤー
	m_pCamera->UpdateStart(m_pPlayer->GetPos());	// カメラ
	m_pMeteorManager->UpdateStart();				// 隕石

	// スタート演出が終わったら移動チュートリアルに遷移
	if (m_pPlayer->IsStartAnimation() &&
		m_pCamera->IsStartAnimation())
	{
		m_stateMachine.SetState(State::MOVE_TUTORIAL);
	}
}

// 移動チュートリアルの更新
void Tutorial::UpdateMoveTutorial()
{
	// 更新
	m_pPlayer->UpdatePlay(m_pCamera->GetCameraHorizon());					// プレイヤー
	m_pCamera->UpdatePlay(m_pPlayer->GetPos(), m_pPlayer->GetMoveVec());	// カメラ
	m_pMeteorManager->Update(m_pCamera->GetPos());							// 隕石

	// 特定のフレームたったら
	m_currentFrame++;
	if (m_currentFrame > wave_wait_frame + move_tutorial_frame)
	{
		// 移動チュートリアルUIの開始
		m_pTutorialUI->StartState(TutorialState::MOVE);

		// 移動チュートリアルUIが終了したら
		if (m_pTutorialUI->IsEndState(TutorialState::MOVE))
		{
			// シールドチュートリアルに遷移
			m_stateMachine.SetState(State::SHIELD_TUTORIAL);
			m_currentFrame = 0;
		}
	}
}

// シールドチュートリアルの更新
void Tutorial::UpdateShieldTutorial()
{
	// 更新
	m_pPlayer->UpdatePlay(m_pCamera->GetCameraHorizon());					// プレイヤー	
	m_pCamera->UpdatePlay(m_pPlayer->GetPos(), m_pPlayer->GetMoveVec());	// カメラ
	m_pMeteorManager->Update(m_pCamera->GetPos());							// 隕石

	// 特定のフレームたったら
	m_currentFrame++;
	if (m_currentFrame > wave_wait_frame)
	{
		// 移動チュートリアルUIの開始
		m_pTutorialUI->StartState(TutorialState::SHIELD);

		// 移動チュートリアルUIが終了したら
		if (m_pTutorialUI->IsEndState(TutorialState::SHIELD))
		{
			// シールドチュートリアルに遷移
			m_stateMachine.SetState(State::REFLECT_TUTORIAL);
			m_currentFrame = 0;
		}
	}

	// デバッグテキストの追加
	DebugText::AddLog("ShieldTutorial");
}

// 反射チュートリアルの更新
void Tutorial::UpdateReflectTutorial()
{
	// 更新
	m_pPlayer->UpdatePlay(m_pCamera->GetCameraHorizon());					// プレイヤー	
	m_pCamera->UpdatePlay(m_pPlayer->GetPos(), m_pPlayer->GetMoveVec());	// カメラ
	m_pMeteorManager->Update(m_pCamera->GetPos());							// 隕石

	// 特定のフレームたったら
	m_currentFrame++;
	if (m_currentFrame > wave_wait_frame)
	{
		// 反射チュートリアルUIの開始
		m_pTutorialUI->StartState(TutorialState::REFLECT);

		// 反射チュートリアルUIが終了したら
		if (m_pTutorialUI->IsEndState(TutorialState::REFLECT))
		{
			// 反射チュートリアルUIの開始
			m_pTutorialUI->StartState(TutorialState::REFLECT2);

			// 敵のウェーブ開始
			m_pEnemyManager->StartWave();
		}
	}
	// 現在の敵のウェーブが終了したら
	if (m_pEnemyManager->IsEndWave() && m_pTutorialUI->IsEndState(TutorialState::REFLECT))
	{
		// 反射チュートリアルを終了
		m_pTutorialUI->EndState();
		if (m_pTutorialUI->IsEndState(TutorialState::REFLECT2))
		{
			// シールドチュートリアルに遷移
			m_stateMachine.SetState(State::CUBE_TUTORIAL);
			m_currentFrame = 0;
		}
	}
	// デバッグテキストの描画
	DebugText::AddLog("ReflectTutorial");
}

// キューブチュートリアルの更新
void Tutorial::UpdateCubeTutorial()
{
	// 更新
	m_pPlayer->UpdatePlay(m_pCamera->GetCameraHorizon());					// プレイヤー
	m_pCamera->UpdatePlay(m_pPlayer->GetPos(), m_pPlayer->GetMoveVec());	// カメラ
	m_pMeteorManager->Update(m_pCamera->GetPos());							// 隕石

	// 特定のフレームたったら
	m_currentFrame++;
	if (m_currentFrame > wave_wait_frame)
	{
		// キューブチュートリアルUIの開始
		m_pTutorialUI->StartState(TutorialState::CUBE);

		if (m_pTutorialUI->IsEndState(TutorialState::CUBE))
		{
			// キューブチュートリアルUIの開始
			m_pTutorialUI->StartState(TutorialState::CUBE2);

			if (!m_isWaveStart)
			{
				m_pEnemyManager->NextWave();
				m_isWaveStart = true;
			}
		}
	}
	if (m_pEnemyManager->IsEndWave() && m_pTutorialUI->IsEndState(TutorialState::CUBE))
	{
		// キューブチュートリアルを終了
		m_pTutorialUI->EndState();
		if (m_pTutorialUI->IsEndState(TutorialState::CUBE2))
		{
			// シールドチュートリアルに遷移
			m_stateMachine.SetState(State::PLAY);
			m_currentFrame = 0;
			m_isWaveStart = false;
		}
	}

	// デバッグテキストの追加
	DebugText::AddLog("CubeTutorial");
}

// プレイ中の更新
void Tutorial::UpdatePlay()
{
	// 更新
	m_pPlayer->UpdatePlay(m_pCamera->GetCameraHorizon());					// プレイヤー
	m_pCamera->UpdatePlay(m_pPlayer->GetPos(), m_pPlayer->GetMoveVec());	// カメラ
	m_pMeteorManager->Update(m_pCamera->GetPos());							// 隕石

	// ウェーブの待機フレーム数を過ぎたら
	m_currentFrame++;
	if (m_currentFrame > wave_wait_frame)
	{
		// 実践チュートリアルUIの開始
		m_pTutorialUI->StartState(TutorialState::PLAY);

		// ウェーブが開始されていなかったら
		if (!m_isWaveStart)
		{
			// ウェーブ開始
			m_pEnemyManager->NextWave();

			// ウェーブが開始されたフラグを立てる
			m_isWaveStart = true;
		}
	}
	// 現在の敵のウェーブが終了したら
	if (m_pEnemyManager->IsEndWave())
	{
		// キューブチュートリアルを終了
		m_pTutorialUI->EndState();

		// 実践チュートリアルUIが終了したら
		if (m_pTutorialUI->IsEndState(TutorialState::PLAY))
		{
			// 現在流れているBGMのフェードアウト
			auto& soundManager = SoundManager::GetInstance();
			auto fileName = soundManager.GetPlayBGMFileName();
			soundManager.SetFadeSound(fileName, 60, soundManager.GetSoundVolume(fileName), 0);

			// シールドチュートリアルに遷移
			m_stateMachine.SetState(State::GAME_CLEAR);
		}
	}
}

// ゲームクリアの更新
void Tutorial::UpdateGameClear()
{
	// UIの格納
	UIManager::GetInstance().Store();

	// 全てのレーザーの削除
	m_pLaserManager->DeleteAllLaser();

	// 更新
	m_pPlayer->UpdateGameClear();
	m_pMeteorManager->Update(m_pCamera->GetPos());

	// カメラの更新が終了したら
	if (m_pCamera->UpdateGameClear(m_pPlayer->GetPos()))
	{
		// リザルト画面に遷移
		m_stateMachine.SetState(State::RESULT);
	}
}

// ゲームオーバーの更新
void Tutorial::UpdateGameOver()
{
	// UIの格納
	UIManager::GetInstance().Store();

	// 全てのレーザーの削除
	m_pLaserManager->DeleteAllLaser();

	// ゲームオーバー時の更新
	m_pCamera->UpdateGameOver(m_pPlayer->GetPos());
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
		return;
	}

	// デバッグテキストの追加
	DebugText::AddLog("GameOver");
}

// スタートアニメーションの開始
void Tutorial::EnterStartAnimation()
{
	// BGMの再生
	SoundManager::GetInstance().PlayBGM("TutorialBgm");
	SoundManager::GetInstance().SetFadeSound("TutorialBgm", 120, 0, 255);
}

void Tutorial::EnterGameClear()
{
	SoundManager::GetInstance().PlaySE("GameClear");
}

// ゲームオーバーの開始
void Tutorial::EnterGameOver()
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

// リザルトの開始
void Tutorial::EnterResult()
{
	// リザルト画面のインスタンス生成
	m_pResultWindow = std::make_shared<ResultWindow>();
}

// リザルトの更新
void Tutorial::UpdateResult()
{
	StageBase::UpdateResult("Tutorial");
}

// 描画
void Tutorial::Draw()
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
	m_pTutorialUI->Draw();							// チュートリアルUI
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