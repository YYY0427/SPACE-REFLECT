#include "Tutorial.h"
#include "../Editor/DataReaderFromUnity.h"
#include "../UI/UIManager.h"
#include "../UI/DamageFlash.h"
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
#include "../Score/Score.h"
#include "../String/MessageManager.h"
#include <DxLib.h>

namespace
{
	// オブジェクト配置データのファイルパス
	const std::string object_data_file_path = "Test";

	// ダメージ
	constexpr int meteor_damage = 2; // 隕石に当たっている間にプレイヤーに与えるダメージ
	constexpr int laser_damage = 1;  // レーザーに当たっている間にプレイヤーに与えるダメージ
	constexpr int enemy_damage = 1;  // 敵に当たっている間にプレイヤーに与えるダメージ
	constexpr int boss_reflect_laser_damage = 2; // 反射レーザーに当たっている間にボスに与えるダメージ
}

// コンストラクタ
Tutorial::Tutorial(SceneManager& manager) :
	StageBase(manager)
{
	// ステートマシンの設定
	m_stateMachine.AddState(State::START_ANIMATION, {}, [this]() { UpdateStartAnimation(); }, {});
	m_stateMachine.AddState(State::PLAY, {}, [this]() { UpdatePlay(); }, {});
	m_stateMachine.AddState(State::GAME_CLEAR, {}, [this]() { UpdateGameClear(); }, {});
	m_stateMachine.AddState(State::RESULT, {}, [this]() { UpdateResult(); }, {});
	m_stateMachine.SetState(State::START_ANIMATION);

	// オブジェクト配置データ読み込み
	auto& dataReader = DataReaderFromUnity::GetInstance();
	dataReader.LoadUnityGameObjectData(object_data_file_path.c_str());

	// スコアのインスタンス生成
	auto& score = Score::GetInstance();

	// インスタンスの作成
	m_pPlayer = std::make_shared<Player>(object_data_file_path);
	m_pLaserManager = std::make_shared<LaserManager>(m_pPlayer);
	m_pPlanetManager = std::make_shared<PlanetManager>(object_data_file_path);
	m_pCamera = std::make_shared<Camera>(m_pPlayer->GetPos());
	m_pSkyDome = std::make_shared<SkyDome>(m_pCamera->GetPos());
	m_pMeteorManager = std::make_shared<MeteorManager>(object_data_file_path);
	m_pScreenShaker= std::make_shared<ScreenShaker>(m_pCamera);
	m_pEnemyManager = std::make_shared<EnemyManager>(m_pPlayer, m_pLaserManager, m_pScreenShaker);

	// UIのインスタンスの作成
	m_pDamageFlash = std::make_shared<DamageFlash>();
	UIManager::GetInstance().AddUI("DamageFlash", m_pDamageFlash, 3, { 0, 0 });

	// ウェーブデータの読み込み
	m_pEnemyManager->LoadWaveFileData("Test");
}

// デストラクタ
Tutorial::~Tutorial()
{
}

// 更新
void Tutorial::Update()
{
	// 更新
	m_stateMachine.Update();	// ステートマシン
	Effekseer3DEffectManager::GetInstance().Update();	// エフェクト
	UIManager::GetInstance().Update();					// UI
}

// スタート演出の更新
void Tutorial::UpdateStartAnimation()
{
	// 更新
	m_pPlayer->UpdateStart(m_pCamera->GetPos());	// プレイヤー
	m_pCamera->UpdateStart(m_pPlayer->GetPos());	// カメラ
	m_pSkyDome->Update(m_pCamera->GetPos());		// スカイドーム
	m_pPlanetManager->UpdateStart(m_pPlayer->GetMoveVec());	// 惑星

	// スタート演出が終わったらプレイ中に遷移
	if (m_pPlayer->GetIsStartAnimation() &&
		m_pCamera->GetIsStartAnimation())
	{
		m_stateMachine.SetState(State::PLAY);
	}
}

// プレイ中の更新
void Tutorial::UpdatePlay()
{
	static bool isWaveStart = false;
	if(!isWaveStart)
	{
		m_pEnemyManager->StartWave();
		isWaveStart = true;
	}

	// 更新
	m_pPlayer->Update(m_pCamera->GetCameraHorizon());	// プレイヤー
	m_pCamera->UpdatePlay(m_pPlayer->GetPos());				// カメラ
	m_pEnemyManager->Update();							// 敵
	m_pLaserManager->Update();							// レーザー
	m_pSkyDome->Update(m_pCamera->GetPos());			// スカイドーム
	m_pPlanetManager->Update();							// 惑星
	m_pMeteorManager->Update(m_pCamera->GetPos());		// 隕石
	m_pDamageFlash->Update();							// ダメージフラッシュ
	m_pScreenShaker->Update();							// 画面揺れ

	// 隕石の生成
//	m_pMeteorManager->CreateMeteor(120, m_pPlayer->GetPos());

	// 当たり判定
	Collision();

	// ボスが死んだらゲームクリアに遷移
	if (m_pEnemyManager->IsDeadBoss())
	{
		m_stateMachine.SetState(State::GAME_CLEAR);
	}
}

// ゲームクリアの更新
void Tutorial::UpdateGameClear()
{
	m_stateMachine.SetState(State::RESULT);
}

// リザルトの更新
void Tutorial::UpdateResult()
{
	StageBase::UpdateResult("Tutorial", "UNKO");
}

// 描画
void Tutorial::Draw()
{
	// 画面揺れの前処理
	m_pScreenShaker->PreDraw();

	// 画面をクリア
	ClearDrawScreen();

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

	// 画面揺れ描画
	m_pScreenShaker->Draw();
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
