#include "GameMainScene.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "DebugScene.h"
#include "PauseScene.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../Util/InputState.h"
#include "../Util/Debug.h"
#include "../Camera.h"
#include "../Player.h"
#include "../Planet/PlanetManager.h"
#include "../Planet/PlanetBase.h"
#include "../Enemy/EnemyManager.h"
#include "../Laser/LaserManager.h"
#include "../Background.h"
#include "../Shield.h"
#include "../Laser/Laser.h"
#include "../common.h"
#include "../Vector2.h"
#include "../Triangle.h"
#include "../Flash.h"
#include "../ScreenEffect.h"
#include "../Vector3.h"
#include "../Matrix.h"
#include "../UIManager.h"

namespace
{
	// ゲームクリア時に何フレーム間画面を揺らすか
	constexpr int game_clear_shake_frame = 60;
}

// コンストラクタ
GameMainScene::GameMainScene(SceneManager& manager) :
	SceneBase(manager)
{
	screenHandle_ = MakeScreen(common::screen_width, common::screen_height);
	assert(screenHandle_ != -1);

	// インスタンス生成
	pUIManager_ = std::make_shared<UIManager>();
	pScreenEffect_ = std::make_shared<ScreenEffect>();
	pBackground_ = std::make_shared<Background>();
	pLaserManager_ = std::make_shared<LaserManager>();
	pPlayer_ = std::make_shared<Player>(pUIManager_);
	pEnemyManager_ = std::make_shared<EnemyManager>(pPlayer_, pLaserManager_, pScreenEffect_, pUIManager_);
	pCamera_ = std::make_shared<Camera>();

	// ステートの初期化
	stateMachine_.AddState(
		State::NORMAL,
		[this]() { EntarNormalState(); },
		[this]() { UpdateNormalState(); },
		[this]() { ExitNormalState(); }
		);
	stateMachine_.AddState(
		State::GAME_CLEAR,
		[this]() { EntarGameClearState(); },
		[this]() { UpdateGameClearState(); },
		[this]() { ExitGameClearState(); }
		);
	stateMachine_.AddState(
		State::GAME_OVER,
		[this]() { EntarGameOverState(); },
		[this]() { UpdateGameOverState(); },
		[this]() { ExitGameOverState(); }
		);
	stateMachine_.SetState(State::NORMAL);
}

// デストラクタ
GameMainScene::~GameMainScene()
{
	// 全てのエフェクトの削除
	Effekseer3DEffectManager::GetInstance().DeleteAllEffect();
}

// メンバ関数ポインタの更新
void GameMainScene::Update()
{
	// フェードが終わり次第シーン遷移
	if (IsStartFadeOutAfterFadingOut())
	{
		switch (item_)
		{
			// ポーズシーンに遷移
		case SceneItem::PAUSE:
			manager_.PushScene(new PauseScene(manager_));
			break;

		case SceneItem::TITLE:
#ifdef _DEBUG
			manager_.ChangeScene(new DebugScene(manager_));
#else
			manager_.ChangeScene(new TitleScene(manager_));
#endif
			return;
		}
		// PushSceneするのでシーンが残るためフェードインの設定
		StartFadeIn();
		return;
	}

	// タイマーの更新
	gameTimer_.Update(1);

	// ステートマシンの更新
	stateMachine_.Update();

	// 画面エフェクトの更新
	pScreenEffect_->Update();

	// UIの削除確認
	pUIManager_->DeleteCheck();

	// フェードの更新
	UpdateFade();
}

// 描画
void GameMainScene::Draw()
{
	// 画面の描画先を変更
	SetDrawScreen(screenHandle_);

	// SetDrawScreenを使用した場合、カメラが初期化されるので再度更新
	pCamera_->Update();

	// 画面のクリア
	ClearDrawScreen();

	// 各クラスの描画
	pBackground_->Draw();
	pEnemyManager_->Draw();
	pPlayer_->Draw();
	pLaserManager_->Draw();

	// 各クラスのUIの描画
	pPlayer_->DrawUI();
	pEnemyManager_->DrawUI();

	// 現在のシーンのテキスト表示
	Debug::Log("GameMainScene");

	// フェードの描画
	DrawFade(true);

	// モザイクフェードの描画
	DrawGaussFade(true);

	// 画面の描画先を戻す
	SetDrawScreen(DX_SCREEN_BACK);

	// SetDrawScreenを使用した場合、カメラが初期化されるので再度更新
	pCamera_->Update();

	// 画面の描画
	pScreenEffect_->Draw(screenHandle_);
}

void GameMainScene::EntarNormalState()
{
}

void GameMainScene::EntarGameClearState()
{
	waitTimer_ = 120;
}

void GameMainScene::EntarGameOverState()
{
}

void GameMainScene::UpdateNormalState()
{
	// 更新
	pPlayer_->Update(pCamera_->GetCameraYaw());
	pEnemyManager_->Update(gameTimer_.GetTime());
	pLaserManager_->Update();
	pCamera_->Update();

	// 当たり判定
	Collision();

	// ポーズ画面に遷移
	if (InputState::IsTriggered(InputType::PAUSE))
	{
		// フェードアウト開始
		StartFadeOut(200, 64);
		item_ = SceneItem::PAUSE;
	}

	// ゲームクリア
	if (pEnemyManager_->IsBossDied())
	{
		stateMachine_.SetState(State::GAME_CLEAR);
	}

	// ゲームオーバー
	if (!pPlayer_->IsLive())
	{
		stateMachine_.SetState(State::GAME_OVER);
	}
}

void GameMainScene::UpdateGameClearState()
{
	waitTimer_.Update(1);
	if (waitTimer_.IsTimeOut())
	{
		// ゲームクリア演出
		pCamera_->GameClearUpdate(pPlayer_->GetPos());
		
		// TODO : ゲームクリアの文字を出す演出を追加する

		// タイトルに戻る
		if (InputState::IsTriggered(InputType::DECISION))
		{
			// フェードアウト開始
			StartFadeOut(255, 5);
			item_ = SceneItem::TITLE;
		}	
	}
}

void GameMainScene::UpdateGameOverState()
{
	// 更新
	pLaserManager_->Update();
	pPlayer_->GameOverUpdate();
	pEnemyManager_->GameOverUpdate();
	pCamera_->GameOverUpdate(pPlayer_->GetPos());

	// タイトルに戻る
	if (InputState::IsTriggered(InputType::DECISION))
	{
		// フェードアウト開始
		StartFadeOut(255, 5);
		item_ = SceneItem::TITLE;
	}
}

void GameMainScene::ExitNormalState()
{
}

void GameMainScene::ExitGameClearState()
{
}

void GameMainScene::ExitGameOverState()
{
}

void GameMainScene::Collision()
{
	if (pPlayer_->GetShield()->IsShield())
	{
		// レーザーとシールドの当たり判定
		for (auto& laser : pLaserManager_->GetLazeres())
		{
			// キューブレーザーは反射できないので飛ばす
			if (laser.type == LaserType::CUBE) continue;
			if (laser.type == LaserType::REFLECT) continue;

			VECTOR leftTop = pPlayer_->GetShield()->GetVertex()[0].pos;
			VECTOR rightTop = pPlayer_->GetShield()->GetVertex()[1].pos;
			VECTOR leftBottom = pPlayer_->GetShield()->GetVertex()[2].pos;
			VECTOR rightBottom = pPlayer_->GetShield()->GetVertex()[3].pos;

			// シールドは二つのポリゴンからできてるので二つのポリゴンともチェック
			MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Triangle(laser.pLaser->GetModelHandle(), -1, leftTop, rightTop, leftBottom);
			MV1_COLL_RESULT_POLY_DIM result2 = MV1CollCheck_Triangle(laser.pLaser->GetModelHandle(), -1, rightBottom, leftBottom, rightTop);

			// 1つでもポリゴンと当たっていたら
			if (result.HitNum > 0 || result2.HitNum > 0)
			{
				laser.pLaser->SetIsReflect(true);

				/*VECTOR hitPos{};
				if (result.HitNum > 0)
					hitPos = VAdd(VAdd(result.Dim->Position[0], result.Dim->Position[1]), result.Dim->Position[2]);
				else
					hitPos = VAdd(VAdd(result2.Dim->Position[0], result2.Dim->Position[1]), result2.Dim->Position[2]);*/

				Vector3 hitPos{};
				if (result.HitNum > 0)
				{
					Vector3 resultPos0{ result.Dim->Position[0].x, result.Dim->Position[0].y, result.Dim->Position[0].z };
					Vector3 resultPos1{ result.Dim->Position[1].x, result.Dim->Position[1].y, result.Dim->Position[1].z };
					Vector3 resultPos2{ result.Dim->Position[2].x, result.Dim->Position[2].y, result.Dim->Position[2].z };
					hitPos = (resultPos0 + resultPos1 + resultPos2) / 3;
				}
				else
				{
					Vector3 resultPos0{ result2.Dim->Position[0].x, result2.Dim->Position[0].y, result2.Dim->Position[0].z };
					Vector3 resultPos1{ result2.Dim->Position[1].x, result2.Dim->Position[1].y, result2.Dim->Position[1].z };
					Vector3 resultPos2{ result2.Dim->Position[2].x, result2.Dim->Position[2].y, result2.Dim->Position[2].z };
					hitPos = (resultPos0 + resultPos1 + resultPos2) / 3;
				}

				// レーザーを止める
				auto pLaser = std::dynamic_pointer_cast<Laser>(laser.pLaser);
				pLaser->Stop(pPlayer_->GetShield()->GetPos());

				// 反射レーザーを作成(既に作成されていた場合更新を行う)
				// シールドの法線情報
				VECTOR shieldNorm = pPlayer_->GetShield()->GetVertex()[0].norm;
				pLaserManager_->Reflect(hitPos, laser.pLaser->GetVec(), shieldNorm);

				// 画面を揺らす
				pScreenEffect_->SetShake(1, 1, 5);

				int handle = 0;
				Effekseer3DEffectManager::GetInstance().PlayEffect(handle, EffectID::enemy_boss_hit_effect, hitPos, {50, 50, 50});
			}
			else
			{
				laser.pLaser->SetIsReflect(false);
			}

			// 当たり判定情報の後始末
			MV1CollResultPolyDimTerminate(result);
			MV1CollResultPolyDimTerminate(result2);
		}
	}
	else
	{
		pLaserManager_->DeleteReflectLaser();
	}

	// レーザーと敵の当たり判定
	for (auto& laser : pLaserManager_->GetLazeres())
	{
		// 反射可能なレーザー以外の場合は判定を行わない
		if (laser.type != LaserType::REFLECT) continue;

		// 全ての敵をチェック
		for (auto& enemy : pEnemyManager_->GetEnemies())
		{
			// レーザーのモデルと敵を球体にみたてて当たり判定
			MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(laser.pLaser->GetModelHandle(), -1, enemy->GetPos(), enemy->GetCollisionRadius());

			// ポリゴン一つにでも当たっていたら
			if (result.HitNum > 0)
			{
				enemy->OnDamage(1.0f, result.Dim->HitPosition);
			}

			// 当たり判定情報の後始末
			MV1CollResultPolyDimTerminate(result);
		}
	}

	// レーザーとプレイヤーの当たり判定
	for (auto& laser : pLaserManager_->GetLazeres())
	{
		if (laser.type == LaserType::REFLECT) continue;

		// レーザーとプレイヤーの当たり判定
		MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(laser.pLaser->GetModelHandle(), -1, pPlayer_->GetPos(), pPlayer_->GetCollsionRadius());

		// 1つでもポリゴンと当たっていたら
		if (result.HitNum > 0)
		{
			pPlayer_->OnDamage();

			// 画面を揺らす
			pScreenEffect_->SetShake(3, 3, 10);

			pScreenEffect_->SetScreenColor(0xff0000, 50, 3);

			Debug::Log("あああああああああああああああああああああ");

			// 当たり判定情報の後始末
			MV1CollResultPolyDimTerminate(result);
			return;
		}

		// 当たり判定情報の後始末
		MV1CollResultPolyDimTerminate(result);
	}
}

//void GameMainScene::CollisionRockUpdate()
//{
//	// フェードアウトが終わり次第シーン遷移
//	if (IsStartFadeOutAfterFadingOut())
//	{
//		if (pPlayer_->IsLive())
//		{
//			StartFadeIn();
//
//			// Updateを通常時のUpdateに変更
//			updateFunc_ = &GameMainScene::NormalUpdate;
//			return;
//		}
//	}
//
//	if (!IsFadeing())
//	{
//		bool isEnd = pPlayer_->OnDamageUpdate();
//
//		if (isEnd)
//		{
//			// フェードアウトの開始
//			StartFadeOut(255);
//		}
//	}
//
//	// 更新
//	pCamera_->Update();
//	pLaserManager_->Update();
//	pEnemyManager_->Update(timer_.GetTime());
//
//	// フェードの更新
//	UpdateFade();
//}