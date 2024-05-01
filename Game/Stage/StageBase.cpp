#include "StageBase.h"
#include "../Scene/StageSelectScene.h"
#include "../Scene/SceneManager.h"
#include "../Editor/DataReaderFromUnity.h"
#include "../UI/UIManager.h"
#include "../Effect/Effekseer3DEffectManager.h"
#include "../Transitor/Fade.h"
#include "../Game/Player.h"
#include "../Game/MeteorManager.h"
#include "../Game/Laser/LaserManager.h"
#include "../Game/Laser/LaserBase.h"
#include "../Game/Shield.h"
#include "../Game/Enemy/EnemyManager.h"
#include "../Game/Enemy/EnemyBase.h"
#include "../Effect/ScreenShaker.h"
#include "../UI/DamageFlash.h"
#include "../Score/Score.h"
#include "../Score/ScoreRanking.h"
#include "../UI/ResultWindow.h"
#include "../SoundManager.h"
#include <DxLib.h>

// コンストラクタ
StageBase::StageBase(SceneManager& manager) :
	m_manager(manager),
	m_currentFrame(0),
	m_isWaveStart(false),
	m_directionalLightHandle(-1),
	m_meteorDamage(0),
	m_laserDamage(0),
	m_enemyDamage(0),
	m_playerToBossDamage(0),
	m_playerToEnemyDamage(0)
{
	// フェードのスタート
	m_pFade = std::make_unique<Fade>();
	m_pFade->StartFadeIn(0);

	// ライトの設定
	m_directionalLightHandle = CreateDirLightHandle({ 1, 1, 0 });
	SetLightDifColorHandle(m_directionalLightHandle, GetColorF(0.5f, 0.5f, 0.5f, 0.0f));

	// スコアの初期化
	Score::GetInstance().Reset();
}

// デストラクタ
StageBase::~StageBase()
{
	// ライトの削除
	DeleteLightHandle(m_directionalLightHandle);

	// すべてのUIを削除
	UIManager::GetInstance().DeleteAllUI();

	// エフェクトの全削除
	Effekseer3DEffectManager::GetInstance().DeleteAllEffect();

	// オブジェクトの配置データの削除
	DataReaderFromUnity::GetInstance().DeleteAllData();

	// 全ての音楽を停止
	SoundManager::GetInstance().StopAllSound();
}

// リザルト画面の更新
void StageBase::UpdateResult(const std::string& stageName)
{
	// フェードが終了したら
	if (m_pFade->IsFadeOutEnd())
	{
		// ステージセレクトに遷移
		m_manager.ChangeScene(std::make_shared<StageSelectScene>(m_manager));
		return;
	}

	// リザルト画面の更新
	m_pResultWindow->Update();

	// リザルト画面が終了したら
	if (m_pResultWindow->IsEnd() && !m_pFade->IsFadingOut())
	{
		// スコアをランキングに追加
		ScoreRanking::GetInstance().AddScore(stageName, "NO NAME", Score::GetInstance().GetTotalScore());

		// フェードアウト開始
		m_pFade->StartFadeOut(255);
	}
}

// 描画
void StageBase::Draw()
{
}

// 当たり判定
void StageBase::Collision()
{
	// プレイヤーと隕石の当たり判定
	for (auto& meteor : m_pMeteorManager->GetMeteor())
	{
		// 小さい隕石なら判定しない
		if(meteor->GetType() == MeteorType::SMALL) continue;

		// 球とメッシュの当たり判定
		MV1_COLL_RESULT_POLY_DIM result{};
		result = MV1CollCheck_Sphere(
			meteor->GetModelHandle(),
			-1,
			m_pPlayer->GetPos().ToDxLibVector3(),
			m_pPlayer->GetCollisionRadius());

		// 当たっていたら
		if (result.HitNum > 0)
		{
			// プレイヤーのダメージ処理
			m_pPlayer->OnDamage(m_meteorDamage);

			// ダメージフラッシュの演出
			m_pDamageFlash->Start(60, 50, 0xff0000);

			// 画面揺れの演出
			m_pScreenShaker->StartShake({ m_meteorDamage * 10.0f, m_meteorDamage * 10.0f }, 30);
		}

		// 当たり判定情報の後始末
		MV1CollResultPolyDimTerminate(result);
	}

	// シールドと敵レーザーの当たり判定
	for (auto& laser : m_pLaserManager->GetLaserList())
	{
		// レーザーの種類が反射レーザーなら判定しない
		if (laser.type == LaserType::REFLECT)	continue;

		// キューブレーザーなら判定しない
		if (laser.type == LaserType::CUBE) continue;

		// プレイヤーのシールドがなければ判定しない
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
			if (result.HitFlag)	firePos = Vector3::FromDxLibVector3(result.Position);
			else				firePos = Vector3::FromDxLibVector3(result2.Position);

			// まだ反射レーザーがなければ反射レーザーを追加
			if (!laser.pLaser->IsReflect())
			{
				// 反射音の再生
				SoundManager::GetInstance().PlaySE("Reflect");

				// 反射レーザーを追加
				int key = m_pLaserManager->AddReflectLaser(m_pEnemyManager, m_pPlayer->GetShield(), laser.pLaser, firePos);
				laser.pLaser->SetReflectLaserKey(key);
			}
			// 既に反射レーザーがあれば反射レーザーの位置を設定
			else
			{
				// 反射レーザーの位置を設定
				m_pLaserManager->SetLaserPos(laser.pLaser->GetReflectLaserKey(), firePos);
			}

			// 敵のレーザーを止める
			laser.pLaser->Reflect(m_pPlayer->GetShield()->GetPos());
		}
	}

	// プレイヤーと敵のレーザーの当たり判定
	for (auto& laser : m_pLaserManager->GetLaserList())
	{
		// 反射中レーザーなら判定しない
		if (laser.pLaser->IsReflect()) continue;

		// レーザーの種類が反射レーザーなら判定しない
		if (laser.type == LaserType::REFLECT)	continue;

		// 球とメッシュの当たり判定
		MV1_COLL_RESULT_POLY_DIM result{};
		result = MV1CollCheck_Sphere(
			laser.pLaser->GetModelHandle(),
			-1,
			m_pPlayer->GetPos().ToDxLibVector3(),
			m_pPlayer->GetCollisionRadius());

		// 当たっていたら
		if (result.HitNum > 0)
		{
			// プレイヤーのダメージ処理
			m_pPlayer->OnDamage(m_laserDamage);

			// ダメージフラッシュの演出
			m_pDamageFlash->Start(60, 50, 0xff0000);

			// 画面揺れの演出
			m_pScreenShaker->StartShake({ m_laserDamage * 10.0f, m_laserDamage * 10.0f }, 30);

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
		if (laser.type != LaserType::REFLECT) continue;

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
				enemy->OnDamage(m_playerToEnemyDamage, Vector3::FromDxLibVector3(result.Dim->HitPosition));
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
					m_playerToBossDamage, Vector3::FromDxLibVector3(result.Dim->HitPosition));
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
		if (distance < enemy->GetCollisionRadius() + m_pPlayer->GetCollisionRadius())
		{
			// プレイヤーのダメージ処理
			m_pPlayer->OnDamage(m_enemyDamage);

			// ダメージフラッシュの演出
			m_pDamageFlash->Start(60, 50, 0xff0000);

			// 画面揺れの演出
			m_pScreenShaker->StartShake({ m_enemyDamage * 10.0f, m_enemyDamage * 10.0f }, 30);
		}
	}
}