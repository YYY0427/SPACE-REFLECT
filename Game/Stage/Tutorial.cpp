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
#include <DxLib.h>

namespace
{
	// オブジェクト配置データのファイル名
	const std::string object_data_file_name = "Tutorial";

	// 敵の配置データのファイル名
	const std::string enemy_data_file_name = "Tutorial";

	// 各チュートリアルのフレーム数
	constexpr int move_tutorial_frame = 500;	// 移動チュートリアルのフレーム数
	constexpr int shield_tutorial_frame = 500;	// シールドチュートリアルのフレーム数
	constexpr int reflect_tutorial_frame = 500;	// 反射チュートリアルのフレーム数

	// ウェーブの待機フレーム数
	constexpr int wave_wait_frame = 200;

	// ダメージ
	constexpr int meteor_damage = 2;			 // 隕石に当たっている間にプレイヤーに与えるダメージ
	constexpr int laser_damage = 1;				 // レーザーに当たっている間にプレイヤーに与えるダメージ
	constexpr int enemy_damage = 1;				 // 敵に当たっている間にプレイヤーに与えるダメージ
	constexpr int boss_reflect_laser_damage = 2; // 反射レーザーに当たっている間にボスに与えるダメージ
}

// コンストラクタ
Tutorial::Tutorial(SceneManager& manager) :
	StageBase(manager)
{
	// ライトの設定
	m_directionalLightHandle = CreateDirLightHandle({ 1, 1, 0});
	SetLightDifColorHandle(m_directionalLightHandle, GetColorF(0.5f, 0.5f, 0.5f, 0.0f));

	// 初期化
	m_currentFrame = 0;
	m_waitTimer = wave_wait_frame;
	m_moveTutorialTimer = move_tutorial_frame;
	m_shieldTutorialTimer = shield_tutorial_frame;

	// ステートマシンの設定
	m_stateMachine.AddState(State::START_ANIMATION, {}, [this]() { UpdateStartAnimation(); }, {});
	m_stateMachine.AddState(State::MOVE_TUTORIAL, {}, [this]() { UpdateMoveTutorial(); }, {});
	m_stateMachine.AddState(State::SHIELD_TUTORIAL, {}, [this]() { UpdateShieldTutorial(); }, {});
	m_stateMachine.AddState(State::REFLECT_TUTORIAL, {}, [this]() { UpdateReflectTutorial(); }, {});
	m_stateMachine.AddState(State::CUBE_TUTORIAL, {}, [this]() { UpdateCubeTutorial(); }, {});
	m_stateMachine.AddState(State::PLAY, {}, [this]() { UpdatePlay(); }, {});
	m_stateMachine.AddState(State::GAME_CLEAR, {}, [this]() { UpdateGameClear(); }, {});
	m_stateMachine.AddState(State::GAME_OVER, {}, [this]() { UpdateGameOver(); }, {});
	m_stateMachine.AddState(State::RESULT, [this]() { EnterResult(); }, [this]() { UpdateResult(); }, {});
	m_stateMachine.SetState(State::START_ANIMATION);

	// オブジェクト配置データ読み込み
	auto& dataReader = DataReaderFromUnity::GetInstance();
	dataReader.LoadUnityGameObjectData(object_data_file_name.c_str());

	// スコアのインスタンス生成
	auto& score = Score::GetInstance();

	// インスタンスの作成
	m_pPlayer = std::make_shared<Player>(object_data_file_name);
	m_pLaserManager = std::make_shared<LaserManager>(m_pPlayer);
	m_pPlanetManager = std::make_shared<PlanetManager>(object_data_file_name);
	m_pMeteorManager = std::make_shared<MeteorManager>(object_data_file_name);
	m_pCamera = std::make_shared<Camera>(m_pPlayer->GetPos());
	m_pSkyDome = std::make_shared<SkyDome>(m_pCamera->GetPos());
	m_pScreenShaker= std::make_shared<ScreenShaker>(m_pCamera);
	m_pEnemyManager = std::make_shared<EnemyManager>(m_pPlayer, m_pLaserManager, m_pScreenShaker);
	m_pTutorialUI = std::make_shared<TutorialUI>();

	// UIのインスタンスの作成
	m_pDamageFlash = std::make_shared<DamageFlash>();
	UIManager::GetInstance().AddUI("DamageFlash", m_pDamageFlash, 3, { 0, 0 });

	// ウェーブデータの読み込み
	m_pEnemyManager->LoadWaveFileData(enemy_data_file_name);
}

// デストラクタ
Tutorial::~Tutorial()
{
	// エフェクトの全削除
	Effekseer3DEffectManager::GetInstance().DeleteAllEffect();
	// ライトの削除
	DeleteLightHandleAll();	
}

// 更新
void Tutorial::Update()
{
	// プレイヤーが死んだらゲームオーバーに遷移
	if (!m_pPlayer->IsLive())
	{
		m_stateMachine.SetState(State::GAME_OVER);
	}

	// 更新
	m_pTutorialUI->Update();								// チュートリアルUI
	m_pSkyDome->Update({ 0, 0, m_pCamera->GetPos().z });	// スカイドーム
	m_pPlanetManager->UpdatePlay(m_pPlayer->GetMoveVec());	// 惑星
	m_pMeteorManager->Update(m_pCamera->GetPos());			// 隕石
	m_pLaserManager->Update();								// レーザー
	m_pEnemyManager->Update();								// レーザー
	m_pDamageFlash->Update();								// ダメージフラッシュ
	m_pScreenShaker->Update();								// 画面揺れ
	Effekseer3DEffectManager::GetInstance().Update();		// エフェクト
	UIManager::GetInstance().Update();						// UI

	Collision();	// 当たり判定
	m_stateMachine.Update();							// ステートマシン
}

// スタート演出の更新
void Tutorial::UpdateStartAnimation()
{
	// 更新
	m_pPlayer->UpdateStart(m_pCamera->GetPos());				// プレイヤー
	m_pCamera->UpdateStart(m_pPlayer->GetPos());				// カメラ
	m_pMeteorManager->UpdateStart({ 0, 0, m_pPlayer->GetMoveZVec() });				// 隕石

	// スタート演出が終わったらプレイ中に遷移
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
	m_pPlayer->Update(m_pCamera->GetCameraHorizon());					// プレイヤー
	m_pCamera->UpdatePlay(m_pPlayer->GetPos(), m_pPlayer->GetMoveVec());// カメラ

	// 特定のフレームたったら
	m_currentFrame++;
	if (m_currentFrame > wave_wait_frame)
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
	m_pPlayer->Update(m_pCamera->GetCameraHorizon());					// プレイヤー
	m_pCamera->UpdatePlay(m_pPlayer->GetPos(), m_pPlayer->GetMoveVec());// カメラ

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
	// デバッグテキストの描画
	DebugText::Log("ShieldTutorial");
}

// 反射チュートリアルの更新
void Tutorial::UpdateReflectTutorial()
{
	// 更新
	m_pPlayer->Update(m_pCamera->GetCameraHorizon());					// プレイヤー
	m_pCamera->UpdatePlay(m_pPlayer->GetPos(), m_pPlayer->GetMoveVec());// カメラ

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
	DebugText::Log("ReflectTutorial");
}

// キューブチュートリアルの更新
void Tutorial::UpdateCubeTutorial()
{
	// 更新
	m_pPlayer->Update(m_pCamera->GetCameraHorizon());					// プレイヤー
	m_pCamera->UpdatePlay(m_pPlayer->GetPos(), m_pPlayer->GetMoveVec());// カメラ

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
	// デバッグテキストの描画
	DebugText::Log("CubeTutorial");
}

// プレイ中の更新
void Tutorial::UpdatePlay()
{
	m_currentFrame++;
	if (m_currentFrame > wave_wait_frame)
	{
		// キューブチュートリアルUIの開始
		m_pTutorialUI->StartState(TutorialState::PLAY);
		if (!m_isWaveStart)
		{
			m_pEnemyManager->NextWave();
			m_isWaveStart = true;
		}
	}
	if (m_pEnemyManager->IsEndWave())
	{
		// キューブチュートリアルを終了
		m_pTutorialUI->EndState();
		if (m_pTutorialUI->IsEndState(TutorialState::PLAY))
		{
			// シールドチュートリアルに遷移
			m_stateMachine.SetState(State::GAME_CLEAR);
		}
	}

	// 更新
	m_pPlayer->Update(m_pCamera->GetCameraHorizon());	// プレイヤー
	m_pCamera->UpdatePlay(m_pPlayer->GetPos(), m_pPlayer->GetMoveVec());		// カメラ
}

// ゲームクリアの更新
void Tutorial::UpdateGameClear()
{
	// 全てのレーザーの削除
	m_pLaserManager->DeleteAllLaser();

	// ゲームクリア時の更新
	m_pPlayer->UpdateGameClear();

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
		// ステージセレクトに遷移
		m_manager.ChangeScene(std::make_shared<StageSelectScene>(m_manager));
	}

	DebugText::Log("GameOver");
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
	// 更新
	m_pResultWindow->Update();

	// リザルト画面が終了したら
	if (m_pResultWindow->IsEnd())
	{
		// スコアをランキングに追加
		ScoreRanking::GetInstance().AddScore("Tutorial", "NO NAME", Score::GetInstance().GetTotalScore());

		// スコアを初期化
		Score::GetInstance().Reset();

		// ステージセレクトに遷移
		m_manager.ChangeScene(std::make_shared<StageSelectScene>(m_manager));
	}
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
	Effekseer3DEffectManager::GetInstance().Draw();	// エフェクト
	m_pPlayer->DrawShield();						// シールド
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

	// チュートリアルUIの描画
	m_pTutorialUI->Draw();
}

// 当たり判定
void Tutorial::Collision()
{
	// プレイヤーと隕石の当たり判定
	for (auto& meteor : m_pMeteorManager->GetMeteor())
	{
		// 球とメッシュの当たり判定
		MV1_COLL_RESULT_POLY_DIM result{};
		result = MV1CollCheck_Sphere(				
			meteor->GetModelHandle(), 
			-1, 
			m_pPlayer->GetPos().ToDxLibVector3(),
			m_pPlayer->GetCollsionRadius());

		// 当たっていたら
		if (result.HitNum > 0)
		{
			// プレイヤーのダメージ処理
			m_pPlayer->OnDamage(meteor_damage);

			// ダメージフラッシュの演出
			m_pDamageFlash->Start(60, 50, 0xff0000);

			// 画面揺れの演出
			m_pScreenShaker->StartShake({ meteor_damage * 10.0f, meteor_damage * 10.0f}, 30);
		}

		// 当たり判定情報の後始末
		MV1CollResultPolyDimTerminate(result);
	}

	// シールドと敵レーザーの当たり判定
	for (auto& laser : m_pLaserManager->GetLaserList())
	{
		// レーザーの種類が反射レーザーなら判定しない
		if (laser.type == LaserType::REFLECT)	continue;

		if (!m_pPlayer->GetShield()->IsShield())
		{
			// レーザーを元に戻す
			laser.pLaser->UndoReflect();

			// シールドがなければ判定しない
			continue;
		}

		// シールドの頂点の座標を取得
		Vector3 shieldLeftTopPos = Vector3::FromDxLibVector3(m_pPlayer->GetShield()->GetVertex()[0].pos);
		Vector3 shieldRightTopPos = Vector3::FromDxLibVector3(m_pPlayer->GetShield()->GetVertex()[1].pos);
		Vector3 shieldLeftBottomPos = Vector3::FromDxLibVector3(m_pPlayer->GetShield()->GetVertex()[2].pos);
		Vector3 shieldRightBottomPos = Vector3::FromDxLibVector3(m_pPlayer->GetShield()->GetVertex()[3].pos);

		// シールドは2つのポリゴンからできてるので2つのポリゴンともチェック
		HITRESULT_LINE result = HitCheck_Line_Triangle(
			laser.pLaser->GetPos().ToDxLibVector3(), laser.pLaser->GetEndPos().ToDxLibVector3(),
			shieldLeftTopPos.ToDxLibVector3(), shieldRightTopPos.ToDxLibVector3(), shieldLeftBottomPos.ToDxLibVector3());

		HITRESULT_LINE result2 = HitCheck_Line_Triangle(
			laser.pLaser->GetPos().ToDxLibVector3(), laser.pLaser->GetEndPos().ToDxLibVector3(),
			shieldRightBottomPos.ToDxLibVector3(), shieldLeftBottomPos.ToDxLibVector3(), shieldRightTopPos.ToDxLibVector3());

		// どっちかのポリゴンが当たっていたら
		if (result.HitFlag || result2.HitFlag)
		{
			// 反射レーザーの発射位置を取得
			Vector3 firePos{};
			if(result.HitFlag)	firePos = Vector3::FromDxLibVector3(result.Position);
			else				firePos = Vector3::FromDxLibVector3(result2.Position);

			// まだ反射レーザーがなければ反射レーザーを追加
			if (!laser.pLaser->IsReflect())
			{
				// 反射レーザーを追加
				int key = m_pLaserManager->AddReflectLaser(m_pPlayer->GetShield(), laser.pLaser, firePos);
				laser.pLaser->SetReflectLaserKey(key);
			}
			// 既に反射レーザーがあれば反射レーザーの位置を設定
			else
			{
				// 反射レーザーの位置を設定
				m_pLaserManager->SetLaserPosition(laser.pLaser->GetReflectLaserKey(), firePos);
			}

			// 敵のレーザーを止める
			laser.pLaser->Reflect(m_pPlayer->GetShield()->GetPos());

			// プレイヤーの反射処理
		//	m_pPlayer->OnReflect();
		}
	}

	// プレイヤーと敵のレーザーの当たり判定
	for (auto& laser : m_pLaserManager->GetLaserList())
	{
		// 反射中レーザーなら判定しない
		if(laser.pLaser->IsReflect()) continue;	

		// レーザーの種類が反射レーザーなら判定しない
		if (laser.type == LaserType::REFLECT)	continue;

		// 球とメッシュの当たり判定
		MV1_COLL_RESULT_POLY_DIM result{};
		result = MV1CollCheck_Sphere(
			laser.pLaser->GetModelHandle(),
			-1,
			m_pPlayer->GetPos().ToDxLibVector3(),
			m_pPlayer->GetCollsionRadius());

		// 当たっていたら
		if (result.HitNum > 0)
		{
			// プレイヤーのダメージ処理
			m_pPlayer->OnDamage(laser_damage);

			// ダメージフラッシュの演出
			m_pDamageFlash->Start(60, 50, 0xff0000);

			// 画面揺れの演出
			m_pScreenShaker->StartShake({ laser_damage * 10.0f, laser_damage * 10.0f }, 30);

			// 当たっていたレーザーが通常のレーザーなら
			if (laser.type == LaserType::NORMAL)
			{
				// レーザーを止める
				laser.pLaser->Stop(m_pPlayer->GetPos());
			}
		}
		// 当たり判定情報の後始末
		MV1CollResultPolyDimTerminate(result);
	}

	// 反射したレーザーと敵の当たり判定
	for (auto& laser : m_pLaserManager->GetLaserList())
	{
		// 反射中レーザーでなければ判定しない
		if(laser.type != LaserType::REFLECT) continue;

		// 雑魚敵
		for (auto& enemy : m_pEnemyManager->GetEnemyList())
		{
			// 球とメッシュの当たり判定
			MV1_COLL_RESULT_POLY_DIM result{};
			result = MV1CollCheck_Sphere(
				laser.pLaser->GetModelHandle(),
				-1,
				enemy->GetPos().ToDxLibVector3(),
				enemy->GetCollisionRadius());

			// 当たっていたら
			if (result.HitNum > 0)
			{
				// 敵にダメージ処理
				enemy->OnDamage(1000, Vector3::FromDxLibVector3(result.Dim->HitPosition));
			}
			// 当たり判定情報の後始末
			MV1CollResultPolyDimTerminate(result);
		}
		// ボス
		if (m_pEnemyManager->GetBossEnemy())
		{
			MV1_COLL_RESULT_POLY_DIM result{};
			result = MV1CollCheck_Sphere(
				laser.pLaser->GetModelHandle(),
				-1,
				m_pEnemyManager->GetBossEnemy()->GetPos().ToDxLibVector3(),
				m_pEnemyManager->GetBossEnemy()->GetCollisionRadius());

			// 当たっていたら
			if (result.HitNum > 0)
			{
				// ボスにダメージ処理
				m_pEnemyManager->GetBossEnemy()->OnDamage(
					boss_reflect_laser_damage, Vector3::FromDxLibVector3(result.Dim->HitPosition));
			}
			// 当たり判定情報の後始末
			MV1CollResultPolyDimTerminate(result);
		}
	}

	// プレイヤーと敵の当たり判定
	for (auto& enemy : m_pEnemyManager->GetEnemyList())
	{
		// 球と球の当たり判定
		float distance = (enemy->GetPos() - m_pPlayer->GetPos()).Length();
		if (distance < enemy->GetCollisionRadius() + m_pPlayer->GetCollsionRadius())
		{
			// プレイヤーのダメージ処理
			m_pPlayer->OnDamage(enemy_damage);

			// ダメージフラッシュの演出
			m_pDamageFlash->Start(60, 50, 0xff0000);

			// 画面揺れの演出
			m_pScreenShaker->StartShake({ enemy_damage * 10.0f, enemy_damage * 10.0f }, 30);
		}
	}
}
