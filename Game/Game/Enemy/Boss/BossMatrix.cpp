#include "BossMatrix.h"
#include "../../../Model.h"
#include "../../../UI/Gauge.h"
#include "../../../UI/StringUI.h"
#include "../../../UI/UIManager.h"
#include "../../../Math/MathUtil.h"
#include "../../../Application.h"
#include "../../Laser/LaserManager.h"
#include "../../Laser/LaserBase.h"
#include "../../Player.h"
#include "../../../Effect/Effekseer3DEffectManager.h"
#include "../../../Effect/Flash.h"
#include "../../../Effect/Triangle.h"
#include "../../../Effect/ScreenShaker.h"
#include "../../../Util/InputState.h"
#include "../../../MyDebug/DebugText.h"
#include "../../../Score/Score.h"
#include "../../../ModelHandleManager.h"
#include "../../../SoundManager.h"
#include <random>
#include <algorithm>
#include <cmath>

// TODO : HPが半分以下になったら透明になる(レーザーの発射場所はランダムにする)
// TODO : HPが半分以下になったらプレイヤーのシールドの回転を逆にする
// TODO : ボスの登場時の演出

namespace
{
	// スクリーンサイズ
	auto& screenSize = Application::GetInstance().GetWindowSize();

	// 位置
	const Vector3 init_pos      = { 0.0f, 300.0f, 5000.0f };	// 初期位置(ワールド座標)
	const Vector3 goal_init_pos = { 0.0f, 300.0f, 1800.0f };	// 登場時の位置(ワールド座標)
	const Vector3 normal_pos    = { 640, 360, 1800.0f };		// 通常時の位置(スクリーン座標)

	// モデル
	const Vector3 model_rot = { MathUtil::ToRadian(20), DX_PI_F, 0.0f}; // 回転率 
	const Vector3 model_scale = { 2.0f , 2.0f, 2.0f };					// 拡大率
	const Vector3 init_model_direction = { 0.0f, 0.0f, -1.0f };			// 初期の向き

	// アニメーション番号
	constexpr int idle_anim_no = 0;				// 待機
	constexpr int move_anim_no = 2;				// 移動
	constexpr int laser_fire_anim_no = 3;		// レーザー発射

	// モデルのマテリアル番号
	constexpr int body_material_no = 0;			// 本体
	constexpr int lense_material_no = 1;		// レンズ
	constexpr int eye_material_no = 2;			// 目
	constexpr int laser_material_no = 3;		// レーザー

	// フレーム番号
	constexpr int normal_laser_fire_frame = 2;	// 通常レーザー発射

	// 移動
	constexpr float entry_move_speed = 10.0f;	// 登場時の移動速度
	constexpr float move_speed = 20.0f;			// 移動速度
	constexpr float distance_threshold = 100.0f;// 目的地に到達したかどうか測る閾値
	const Vector3 move_pos[] =					// 移動先の座標
	{
		{ screenSize. width / 2.0f, screenSize.height / 2.0f, 1800 },
		{ 0 + 200, 0 + 200, 1600 },
		{ screenSize. width - 200.0f, 0 + 200, 1800 },
		{ 0 + 200, screenSize.height - 200.0f, 1800 },
		{ screenSize.width - 200.0f, screenSize.height - 200.0f, 1800 }
	};

	// HP
	constexpr int max_hp        = 1000;											// 最大HP
	const Vector2 hp_gauge_pos  = { screenSize.width / 2.0f, 0.0f + 100.0f};	// HPゲージの位置
	const Vector2 hp_gauge_size = { 500, 20 };									// HPゲージのサイズ
	const std::string hp_gauge_img_file_path       = "Data/Image/HP.png";		// HPゲージの画像ファイルパス
	const std::string hp_gauge_back_img_file_path  = "Data/Image/HPBack.png";	// HPゲージの背景画像ファイルパス
	const std::string hp_gauge_frame_img_file_path = "Data/Image/HPFrame.png";	// HPゲージの枠画像ファイルパス
	constexpr int hp_gauge_start_anim_frame = 300;		  // HPゲージの開始アニメーションのフレーム
	constexpr int hp_gauge_sub_frame		= 120;		  // ダメージを受けた時のHPゲージの減るフレーム
	constexpr int hp_gauge_delay_frame		= 60;		  // ダメージを受けてからHPゲージが減るまでの遅延フレーム
	constexpr int hp_gauge_draw_order		= 2;		  // HPゲージUIの描画優先度
	const Vector2 hp_gauge_store_vec        = { 0, -1 };  // HPゲージUIの格納ベクトル

	// ボスの名前
	const std::string boss_name_key = "BossMatrixName";							// ボスの名前キー
	const Vector2 boss_name_pos = { hp_gauge_pos.x, hp_gauge_pos.y - 50.0f };	// ボスの名前の位置
	constexpr int boss_name_ui_draw_order = 2;									// ボスの名前UIの描画優先度
	const Vector2 boss_name_ui_store_vec = { 0, -1 };							// ボスの名前UIの格納ベクトル

	// 当たり判定の半径
	constexpr float collision_radius = 250.0f;

	// 登場時に不透明度を下げる速度
	constexpr float entry_opacity_speed = 0.005f;

	// 不透明度の最大値
	constexpr float max_opacity = 1.0f;

	// フレーム
	constexpr int change_anim_frame              = 8;		// アニメーションの変更フレーム
	constexpr int next_attack_state_frame        = 60 * 5;	// 次の攻撃ステートに移るまでのフレーム
	constexpr int stop_normal_laser_attack_frame = 60 * 20;	// 通常レーザー攻撃のフレーム
	constexpr int move_normal_laser_attack_frame = 60 * 20;	// 移動しながら通常レーザー攻撃のフレーム
	constexpr int cube_laser_attack_frame        = 60 * 10;	// キューブレーザー攻撃のフレーム
	constexpr int cube_laser_interval_frame      = 60 * 2;  // キューブレーザー攻撃の間隔フレーム
	constexpr int die_idle_frame                 = 60 * 3;	// 死亡時の待機フレーム
	constexpr int die_draw_stop_frame            = 60 * 7;	// 死亡時の描画停止フレーム
	constexpr int look_player_frame				 = 20;		// プレイヤーの方向に向けるフレーム

	// レーザー
	constexpr float cube_laser_speed = 8.0f;	// キューブレーザーの速度
	constexpr int laser_charge_frame = 140;		// レーザーのチャージフレーム
	constexpr int laser_fire_frame   = 10000;	// レーザーの発射フレーム
	constexpr float laser_move_speed = 5.0f;	// レーザーの移動速度
	constexpr float laser_fire_pos_z_interval = 200.0f;	// レーザーの発射位置のボスとのZ座標の間隔

	// エフェクト
	const Vector3 damage_effect_scale = { 100.0f, 100.0f, 100.0f };	// ダメージエフェクトの拡大率
	const Vector3 die_effect_scale    = { 100.0f, 100.0f, 100.0f };	// 死亡エフェクトの拡大率
	constexpr float die_effect_z_interval = 300.0f;					// 死亡エフェクトとボスとのZ座標の間隔

	// 画面揺れ
	constexpr int shake_frame = 70;					// 画面揺れのフレーム
	const Vector2 shake_size  = { 0.0f, 100.0f };	// 画面揺れの大きさ
}

// コンストラクタ
BossMatrix::BossMatrix(const std::shared_ptr<Player>& pPlayer, 
					   const std::shared_ptr<LaserManager>& pLaserManager, 
					   const std::shared_ptr<ScreenShaker>& pScreenShaker) :
	m_attackStateIndex(0),
	m_isMoveEnd(false),
	m_idleFrame(0),
	m_laserKey(-1),
	m_laserFrame(0),
	m_damageEffectHandle(-1),
	m_dieIdleFrame(die_idle_frame),
	m_pScreenShaker(pScreenShaker),
	m_dieEffectHandle(-1),
	m_dieDrawStopFrame(die_draw_stop_frame),
	m_movePointIndex(0),
	m_isGoal(false),
	m_cubeLaserIntervalFrame(cube_laser_interval_frame),
	m_playerDirLerp(0),
	m_timer(0),
	m_isExplosionSound(false)
{
	// 初期化
	m_pPlayer         = pPlayer;
	m_pLaserManager   = pLaserManager;
	m_pos			  = m_pPlayer->GetPos() + init_pos;
	m_rot			  = model_rot;
	m_opacity		  = 0.0f;
	m_hp			  = max_hp;
	m_moveSpeed		  = move_speed;
	m_collisionRadius = collision_radius * model_scale.x;
	for (auto& point : move_pos)
	{
		m_movePointTable.push_back(point);
	}

	// ステートマシンの設定
	m_stateMachine.AddState(State::ENTRY, {}, [this]() {UpdateEntry(); }, {});
	m_stateMachine.AddState(State::IDLE, {}, [this]() {UpdateIdle(); }, {});
	m_stateMachine.AddState(State::DIE, [this]() {EnterDie(); }, [this]() {UpdateDie(); }, {});
	m_stateMachine.AddState(State::MOVE_HOMING_LASER_ATTACK, [this]() {EnterMoveHormingLaserAttack(); }, [this]() {UpdateMoveHomingLaserAttack(); }, {});
	m_stateMachine.AddState(State::CUBE_LASER_ATTACK, [this]() {EnterCubeLaserAttack(); }, [this]() {UpdateCubeLaserAttack(); }, {});
	m_stateMachine.AddState(State::GAME_OVER, {}, [this]() {UpdateGameOver(); }, {});
	m_stateMachine.SetState(State::ENTRY);

	// 攻撃ステートの追加
	m_attackStateTable.push_back(State::MOVE_HOMING_LASER_ATTACK);
	m_attackStateTable.push_back(State::CUBE_LASER_ATTACK);
	ShuffleAttackState();

	// HPゲージの設定
	m_pHpGauge = std::make_shared<Gauge>(
		hp_gauge_img_file_path, hp_gauge_back_img_file_path, hp_gauge_frame_img_file_path, max_hp, 
		hp_gauge_pos, hp_gauge_size, hp_gauge_start_anim_frame, hp_gauge_sub_frame, hp_gauge_delay_frame);

	// HPゲージをUIに追加
	UIManager::GetInstance().AddUI("BossHPGauge", m_pHpGauge, hp_gauge_draw_order, hp_gauge_store_vec);

	// ボス名前の設定
	m_pBossName = std::make_shared<StringUI>(boss_name_key);
	m_pBossName->SetPos(boss_name_pos);

	// ボス名前をUIに追加
	UIManager::GetInstance().AddUI("BossName", m_pBossName, boss_name_ui_draw_order, boss_name_ui_store_vec);

	// モデル設定
	m_pModel = std::make_shared<Model>(ModelHandleManager::GetInstance().GetHandle("Matrix"));	// インスタンス生成
	m_pModel->SetUseCollision(true);					// 当たり判定設定
	m_pModel->SetOpacity(m_opacity);					// 不透明度	
	m_pModel->SetScale(model_scale);					// 拡大率
	m_pModel->SetRot(m_rot);							// 向き
	m_pModel->SetPos(m_pos);							// 位置
	m_pModel->Update();									// 当たり判定の更新
}

// デストラクタ
BossMatrix::~BossMatrix()
{
	// HPゲージの削除
	UIManager::GetInstance().DeleteUI("BossHPGauge");

	// ボス名前の削除
	UIManager::GetInstance().DeleteUI("BossName");
}

// 更新
void BossMatrix::Update()
{
	// ステートマシンの更新
	m_stateMachine.Update();

	// プレイヤーが存在していたら
	if (m_pPlayer->IsEnabled())
	{
		// Z座標の更新
		m_pos.z += m_pPlayer->GetMoveVec().z;
	}
	else
	{
		// ゲームオーバーに遷移
		m_stateMachine.SetState(State::GAME_OVER);
	}

	// モデル設定
	m_pModel->RestoreAllMaterialDifColor();	// ディフューズカラーを元に戻す
	m_pModel->SetOpacity(m_opacity);		// 不透明度
	m_pModel->SetRot(m_rot);				// 向き
	m_pModel->SetPos(m_pos);				// 位置
	m_pModel->Update();						// モデルの当たり判定、アニメーションの更新
}

// 描画
void BossMatrix::Draw()
{
	// 存在フラグが立っていなかったら描画しない
	if (m_isEnabled)
	{
		// デバッグログの追加
		DebugText::AddLog("BossMatrixPos", { m_pos.x, m_pos.y, m_pos.z});

		// モデルの描画
		m_pModel->Draw();

		// フラッシュの描画
		if (m_pFlash)
		{
			m_pFlash->Draw();
		}

#ifdef _DEBUG
		// 当たり判定の描画
		DrawSphere3D(m_pos.ToDxLibVector3(), m_collisionRadius, 16, 0xff0000, 0xff0000, false);
#endif
	}
}

// ダメージ処理
void BossMatrix::OnDamage(const int damage, const Vector3& pos)
{
	// HPを減らす
	m_hp -= damage;
	m_pHpGauge->SetValue(m_hp);

	// ダメージエフェクトの再生
	Effekseer3DEffectManager::GetInstance().PlayEffect(
		m_damageEffectHandle,
		"PlayerAttackHitEffect",
		pos,
		damage_effect_scale);

	// HPが0以下になったら死亡
	if (m_hp <= 0)
	{
		// ステートを死亡に変更
		m_stateMachine.SetState(State::DIE);
	}
	else
	{
		// 全てのマテリアルのディフューズカラーを反転
		m_pModel->InversAllMaterialDifColor();
	}
}

// 死亡演出の開始
void BossMatrix::EnterDie()
{
	// アニメーションの停止
	m_pModel->StopAnim();

	// 画面揺れ開始
	m_pScreenShaker->StartShake(shake_size, shake_frame);

	// フラッシュの開始
	m_pFlash = std::make_shared<Flash>(10);
}

// 移動しながらホーミングレーザー攻撃の開始
void BossMatrix::EnterMoveHormingLaserAttack()
{
	// フレームの初期化
	m_laserFrame = move_normal_laser_attack_frame;

	// 通常レーザー発射用のアニメーションに変更
	m_pModel->ChangeAnimation(laser_fire_anim_no, true, false, change_anim_frame);

	// レーザーの生成
	m_laserKey = m_pLaserManager->AddLaser(
		LaserType::NORMAL, 
		shared_from_this(), 
		laser_charge_frame, 
		laser_fire_frame,
		laser_move_speed,
		true);

	// 移動の初期化
	InitMove();
}

// キューブレーザー攻撃の開始
void BossMatrix::EnterCubeLaserAttack()
{
	// フレームの初期化
	m_laserFrame = cube_laser_attack_frame;

	// キューブレーザー発射用のアニメーションに変更
	m_pModel->ChangeAnimation(laser_fire_anim_no, true, false, change_anim_frame);
}

// 登場時の更新
void BossMatrix::UpdateEntry()
{
	// 不透明度を上げる
	m_opacity += entry_opacity_speed;
	m_opacity = (std::min)(m_opacity, max_opacity);
	m_pModel->SetOpacity(m_opacity);

	// 移動中なら
	if (!m_isMoveEnd)
	{
		// ベクトルの取得
		Vector3 goalPos = { goal_init_pos.x, goal_init_pos.y, goal_init_pos.z + m_pPlayer->GetPos().z };
		Vector3 moveVec = (goalPos - m_pos).Normalized() * entry_move_speed;

		// 移動
		m_pos += moveVec;

		// 目的地に到達したか 
		if (m_pos.Distance(goalPos) < distance_threshold)
		{
			m_isMoveEnd = true;
		}
	}
	
	// 不透明度が最大値を超えたら
	// HPゲージの登場演出が終了したら
	// 目的地に到着したら
	if (m_opacity >= 1.0f && 
		m_pHpGauge->IsEndEntranceAnim() &&
		m_isMoveEnd)
	{
		// 初期化
		m_isMoveEnd = false;

		// ステートを待機に変更
		m_stateMachine.SetState(State::IDLE);
	}
}

// 待機時の更新
void BossMatrix::UpdateIdle()
{
	// プレイヤーの方向を見る
	LookPlayerDir();

	// 待機アニメーションに変更
	m_pModel->ChangeAnimation(idle_anim_no, true, false, change_anim_frame);

	// 待機
	if (m_idleFrame++ > next_attack_state_frame)
	{
		// 初期化
		m_idleFrame = 0;

		// 攻撃ステートの設定
		SetAttackState();
	}
}

// 死亡時の更新
void BossMatrix::UpdateDie()
{
	// レーザーを削除
	m_pLaserManager->DeleteAllLaser();

	// フラッシュの更新
	m_pFlash->Update(0xffffff);

	// 徐々にプレイヤーを透明にする
//	m_pPlayer->FadeOut();

	// 特定のフレームが経過したら演出開始
	if (m_dieIdleFrame-- <= 0)
	{
		// UIを格納
		UIManager::GetInstance().Store();

		// まだエフェクトが再生されていなかったら
		if (m_dieEffectHandle == -1)
		{
			// スコアの加算
			Score::GetInstance().AddScore(ScoreType::BOSS);

			// エフェクトの再生
			Effekseer3DEffectManager::GetInstance().PlayEffect(
				m_dieEffectHandle,
				"EnemyBossDie",
				m_pos,
				die_effect_scale);
		}
		// エフェクトの再生を開始してから、一定フレーム経過したら
		else if (m_dieDrawStopFrame-- <= 0)
		{
			// 見えなくする
			m_opacity = 0.0f;

			// エフェクトの再生が終了したら
			if (!Effekseer3DEffectManager::GetInstance().IsPlayingEffect(m_dieEffectHandle))
			{
				// 存在フラグを下げる
				m_isEnabled = false;
			}
		}
		if (m_timer++ >= 300 && !m_isExplosionSound)
		{
			SoundManager::GetInstance().PlaySE("BossExplosion");
			m_isExplosionSound = true;
		}

		// エフェクトの位置の更新
		Effekseer3DEffectManager::GetInstance().SetEffectPos(
			m_dieEffectHandle, 
			{ m_pos.x, m_pos.y, m_pos.z - die_effect_z_interval });
	}
}

// ゲームオーバー時の更新
void BossMatrix::UpdateGameOver()
{
	// プレイヤーの方向を見る
	LookPlayerDir();
}

// 移動しながらホーミングレーザー攻撃
void BossMatrix::UpdateMoveHomingLaserAttack()
{
	// レーザーの発射位置の更新
	Vector3 pos = Vector3::FromDxLibVector3(
		MV1GetFramePosition(m_pModel->GetModelHandle(), normal_laser_fire_frame));
	m_laserFirePos = { pos.x, pos.y, pos.z - laser_fire_pos_z_interval };

	// レーザーの方向に向けるようにする
	Vector3 directionVec = m_pLaserManager->GetLaserData(m_laserKey).pLaser->GetDirection();
	Matrix rotMtx        = Matrix::GetRotationMatrix(init_model_direction, directionVec);
	m_rot = { rotMtx.ToEulerAngle().x * -1, rotMtx.ToEulerAngle().y + DX_PI_F, rotMtx.ToEulerAngle().z * -1 };

	// フレームが経過したら
	if (m_laserFrame-- <= 0)
	{
		// 初期位置へ移動
		MoveInitPos();

		// 移動が終了したら
		if (m_isMoveEnd)
		{
			// ステートを待機に変更
			m_stateMachine.SetState(State::IDLE);

			// レーザーの削除
			m_pLaserManager->DeleteLaser(m_laserKey);
		}
	}
	else
	{
		// 移動
		Move();
	}
}

// キューブレーザー攻撃
void BossMatrix::UpdateCubeLaserAttack()
{
	// プレイヤーの方向を見る
	LookPlayerDir();

	// フレーム内なら
	if (m_laserFrame-- >= 0)
	{
		// フレームが経過したら
		if (m_cubeLaserIntervalFrame-- <= 0)
		{
			// フレームの初期化
			m_cubeLaserIntervalFrame = cube_laser_interval_frame;

			// 発射位置の取得
			Vector3 firePos = Vector3::FromDxLibVector3(
				MV1GetFramePosition(m_pModel->GetModelHandle(), normal_laser_fire_frame));

			// キューブレーザーの生成
			int key = m_pLaserManager->AddCubeLaser(firePos, cube_laser_speed);
		}
	}
	else
	{
		// ステートを待機に変更
		m_stateMachine.SetState(State::IDLE);
	}
}

// 攻撃ステートの順序をシャッフル
void BossMatrix::ShuffleAttackState()
{
	std::random_device seed;
	std::mt19937 engine(seed());
	std::shuffle(m_attackStateTable.begin(), m_attackStateTable.end(), engine);
}

// 移動
void BossMatrix::Move()
{
	// 目的地に到達したかどうかの判定
	if (m_pos.Distance(m_goalPos) < distance_threshold && !m_isGoal)
	{
		// 到達した場合、一回しか通ってほしくない為、
		// フラグを立てる
		m_isGoal = true;

		// 目的地に到達したら次の目的地を設定
		m_movePointIndex++;

		// 次の目的地がリストの最後まで到達したら
		if (m_movePointIndex >= m_actionDataList.size())
		{
			// 移動の初期化
			InitMove();	
		}

		// 目的地の設定
		SetGoalPos();
	}
	else
	{
		// 目的地とベクトルの設定
		SetGoalPos();

		// 移動
		m_pos += m_moveVec;
	}
}

// 移動の初期化
void BossMatrix::InitMove()
{
	// 初期化
	m_movePointIndex = 0;
	m_isGoal = false;
	m_isMoveEnd = false;

	// 地点の移動順序の入れ替え(配列の中身をシャッフル)
	std::random_device seed;
	std::mt19937 engine(seed());
	std::shuffle(m_movePointTable.begin(), m_movePointTable.end(), engine);

	// 目的地の設定
	SetGoalPos();
}

// 目的地の設定
void BossMatrix::SetGoalPos()
{
	// 現在の移動ポイントのイテレーターの取得
	auto itr = m_movePointTable.begin();
	std::advance(itr, m_movePointIndex);

	// 目的地の設定
	float z = (fabs(GetCameraPosition().z - m_pPlayer->GetPos().z) + itr->z) / GetCameraFar();
	m_goalPos = Vector3::FromDxLibVector3(ConvScreenPosToWorldPos_ZLinear({ itr->x, itr->y, z }));

	// 移動ベクトルの設定
	m_moveVec = (m_goalPos - m_pos).Normalized() * m_moveSpeed;
}

// 死亡演出中か 
bool BossMatrix::IsDeadAnim()
{
	return m_stateMachine.GetCurrentState() == State::DIE;
}

// プレイヤーの方向を見る
void BossMatrix::LookPlayerDir()
{
	// ラープの値の取得
	if (m_playerDirLerp++ >= look_player_frame) m_playerDirLerp = 0;
	float lerpValue = static_cast<float>(m_playerDirLerp) / static_cast<float>(look_player_frame);

	// プレイヤーの方向の行列を取得
	Vector3 directionVec = m_pPlayer->GetPos() - m_pos;
	Matrix rotMtx = Matrix::GetRotationMatrix(init_model_direction, directionVec);

	// 徐々にプレイヤーの方向を向く
	m_rot = m_rot.Lerp(m_rot,
		{ rotMtx.ToEulerAngle().x * -1, rotMtx.ToEulerAngle().y + DX_PI_F, rotMtx.ToEulerAngle().z * -1 },
		lerpValue);
}

// 攻撃ステートの設定
void BossMatrix::SetAttackState()
{
	// 攻撃ステートの設定
	m_stateMachine.SetState(m_attackStateTable[m_attackStateIndex]);

	// 攻撃ステートのインデックスを進める
	m_attackStateIndex++;

	// 攻撃ステートのインデックスが攻撃ステートのサイズを超えたら
	if (m_attackStateIndex >= m_attackStateTable.size())
	{
		// 攻撃ステートをシャッフル
		ShuffleAttackState();

		// 攻撃ステートのインデックスを初期化
		m_attackStateIndex = 0;
	}
}

// 初期位置へ移動
void BossMatrix::MoveInitPos()
{
	// 目的地の設定
	float z = (fabs(GetCameraPosition().z - m_pPlayer->GetPos().z) + normal_pos.z) / GetCameraFar();
	m_goalPos = Vector3::FromDxLibVector3(ConvScreenPosToWorldPos_ZLinear({ normal_pos.x, normal_pos.y, z }));

	// 移動ベクトルの設定
	m_moveVec = (m_goalPos - m_pos).Normalized() * m_moveSpeed;

	// 目的地に到達したかどうかの判定
	if (m_pos.Distance(m_goalPos) < distance_threshold)
	{
		// 移動が終了したらフラグを立てる
		m_isMoveEnd = true;
	}
	else
	{
		// 移動
		m_pos += m_moveVec;
	}
}