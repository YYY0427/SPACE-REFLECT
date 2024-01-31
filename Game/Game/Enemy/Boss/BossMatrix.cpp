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
#include <random>
#include <algorithm>
#include <cmath>

// TODO : HPが半分以下になったら透明になる(レーザーの発射場所はランダムにする)
// TODO : HPが半分以下になったらプレイヤーのシールドの回転を逆にする

namespace
{
	// 位置
	const Vector3 init_pos = { 0.0f, 300.0f, 5000.0f };				// 初期位置
	const Vector3 goal_init_pos = { 0.0f, 300.0f, 1500.0f };		// 登場時の位置

	// モデル
	const Vector3 model_rot = { MathUtil::ToRadian(20), DX_PI_F, 0.0f};
	const Vector3 model_scale = { 2.0f , 2.0f, 2.0f };			// 拡大率
	const Vector3 init_model_direction = { 0.0f, 0.0f, -1.0f };	// 初期の向き

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
	constexpr float distance_threshold = 10.0f; // 目的地に到達したかどうか測る閾値
	constexpr float near_far_z_pos = 0.068f;	// 0.0 = near, 1.0 = far
	const Vector3 move_pos[] =					// 移動先の座標
	{
		{ 640, 360, 1300 },
		{ 100, 100, 1300 },
		{ 1180, 100, 1300 },
		{ 100, 620, 1300 },
		{ 1180, 620, 1300 },
	};

	// 死亡
	constexpr float died_swing_width = 5.0f;	// 死亡時の横揺れの大きさ
	constexpr float died_swing_speed = 1.0f;	// 死亡時の横揺れの速さ
	constexpr int died_continue_frame = 60 * 5;	// 死亡時の演出の継続時間
	constexpr int died_flash_frame = 60 * 1;	// 死亡時のフラッシュのフレーム
	constexpr int die_effect_interval_frame = 20;			// 死亡時のエフェクトの再生間隔
	constexpr int die_idle_frame = 60 * 3;					// 死亡時の待機フレーム
	constexpr int die_draw_stop_frame = 60 * 5;				// 死亡時の描画停止フレーム

	// HP
	auto& screenSize = Application::GetInstance().GetWindowSize();
	constexpr int max_hp = 1000;		// 最大HP
	const Vector2 hp_gauge_pos = { screenSize.width / 2.0f, 0.0f + 100.0f};	// HPゲージの位置
	const Vector2 hp_gauge_size = { 500, 20 };										// HPゲージのサイズ
	const std::string hp_gauge_img_file_path = "Data/Image/HP.png";				// HPゲージの画像ファイルパス
	const std::string hp_gauge_back_img_file_path = "Data/Image/HPBack.png";	// HPゲージの背景画像ファイルパス
	const std::string hp_gauge_frame_img_file_path = "Data/Image/HPFrame.png";	// HPゲージの枠画像ファイルパス

	// ボスの名前
	const std::string boss_name_key = "BossMatrixName";	// ボスの名前キー
	const Vector2 boss_name_pos = { hp_gauge_pos.x, hp_gauge_pos.y - 50.0f };	// ボスの名前の位置

	// 当たり判定の半径
	constexpr float collision_radius = 250.0f;

	// フレーム
	constexpr int next_attack_state_frame = 60 * 5;			// 次の攻撃ステートに移るまでのフレーム
	constexpr int stop_normal_laser_attack_frame = 60 * 20;	// 通常レーザー攻撃のフレーム
	constexpr int move_normal_laser_attack_frame = 60 * 20;	// 移動しながら通常レーザー攻撃のフレーム
	constexpr int cube_laser_attack_frame = 60 * 10;		// キューブレーザー攻撃のフレーム
	constexpr int cube_laser_interval_frame = 60 * 2;		// キューブレーザー攻撃の間隔フレーム
}

// コンストラクタ
BossMatrix::BossMatrix(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LaserManager> pLaserManager, std::shared_ptr<ScreenShaker> pScreenShaker) :
	m_attackStateIndex(0),
	m_isMoveEnd(false),
	m_idleFrame(0),
	m_laserKey(-1),
	m_laserFrame(0),
	m_damageEffectHandle(-1),
	m_dieIdleFrame(die_idle_frame),
	m_dieShakeFrame(0),
	m_dieEffectIntervalFrame(die_effect_interval_frame),
	m_pScreenShaker(pScreenShaker),
	m_dieEffectHandle(-1),
	m_dieDrawStopFrame(die_draw_stop_frame),
	m_movePointIndex(0),
	m_isGoal(false),
	m_cubeLaserIntervalFrame(cube_laser_interval_frame)
{
	// 初期化
	m_pPlayer = pPlayer;
	m_pLaserManager = pLaserManager;
	m_pos = m_pPlayer->GetPos() + init_pos;
	m_rot = model_rot;
	m_opacity = 0.0f;
	m_hp = max_hp;
	m_moveSpeed = move_speed;
	m_collisionRadius = collision_radius * model_scale.x;
	for (auto& point : move_pos)
	{
		m_movePointTable.push_back(point);
	}

	// ステートマシンの設定
	m_stateMachine.AddState(State::ENTRY, {}, [this]() {UpdateEntry(); }, {});
	m_stateMachine.AddState(State::IDLE, {}, [this]() {UpdateIdle(); }, {});
	m_stateMachine.AddState(State::DIE, [this]() {EntarDie(); }, [this]() {UpdateDie(); }, {});
	m_stateMachine.AddState(State::MOVE_NORMAL_LASER_ATTACK, [this]() {EntarMoveNormalLaserAttack(); }, [this]() {UpdateMoveNormalLaserAttack(); }, {});
	m_stateMachine.AddState(State::MOVE_HOMING_LASER_ATTACK, {}, [this]() {UpdateMoveHomingLaserAttack(); }, {});
	m_stateMachine.AddState(State::STOP_NORMAL_LASER_ATTACK, [this]() {EntarStopNormalLaserAttack(); }, [this]() {UpdateStopNormalLaserAttack(); }, {});
	m_stateMachine.AddState(State::CUBE_LASER_ATTACK, [this]() {EntarCubeLaserAttack(); }, [this]() {UpdateCubeLaserAttack(); }, {});
	m_stateMachine.SetState(State::ENTRY);

	// 攻撃ステートの追加
	m_attackStateTable.push_back(State::MOVE_NORMAL_LASER_ATTACK);
//	m_attackStateTable.push_back(State::MOVE_HOMING_LASER_ATTACK);
//	m_attackStateTable.push_back(State::STOP_NORMAL_LASER_ATTACK);
//	m_attackStateTable.push_back(State::CUBE_LASER_ATTACK);
	ShuffleAttackState();

	// HPゲージの設定
	m_pHpGauge = std::make_shared<Gauge>(
		hp_gauge_img_file_path, hp_gauge_back_img_file_path, hp_gauge_frame_img_file_path, max_hp, 
		hp_gauge_pos, hp_gauge_size, true, 2, true, 3);
	UIManager::GetInstance().AddUI("BossHPGauge", m_pHpGauge, 2, { 0, -1 });

	// ボス名前の設定
	m_pBossName = std::make_shared<StringUI>(boss_name_key);
	m_pBossName->SetPos(boss_name_pos);
	UIManager::GetInstance().AddUI("BossName", m_pBossName, 2, { 0, -1 });

	// モデル設定
	m_pModel = std::make_shared<Model>(modelHandle);	// インスタンス生成
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
}

// 更新
void BossMatrix::Update()
{
#ifdef _DEBUG
	// デバッグ用
	if (InputState::IsTriggered(InputType::BOSS_DETH_DEBUG))
	{
		m_hp = 0;
	}
#endif

	// HPゲージの更新
	m_pHpGauge->Update();	

	// Z座標の更新
	m_pos.z += m_pPlayer->GetMoveVec().z;

	// ステートマシンの更新
	m_stateMachine.Update();

	// モデル設定
	m_pModel->RestoreAllMaterialDifColor();	// ディフューズカラーを元に戻す
	m_pModel->SetOpacity(m_opacity);	// 不透明度
	m_pModel->SetRot(m_rot);				// 向き
	m_pModel->SetPos(m_pos);			// 位置
	m_pModel->Update();					// モデルの当たり判定、アニメーションの更新
}

// 描画
void BossMatrix::Draw()
{
	// 存在フラグが立っていなかったら描画しない
	if (m_isEnabled)
	{
		DebugText::Log("BossMatrixPos", { m_pos.x, m_pos.y, m_pos.z});

		// モデルの描画
		m_pModel->Draw();

#ifdef _DEBUG
		// 当たり判定の描画
		DrawSphere3D(m_pos.ToDxLibVector3(), m_collisionRadius, 16, 0xff0000, 0xff0000, false);
#endif
	}

	//// 死亡時の演出の描画
	//if (m_stateMachine.GetCurrentState() == State::DIE)
	//{
	//	m_pFlash->Draw();
	//	m_pTriangle->Draw();
	//}
}

// ダメージ処理
void BossMatrix::OnDamage(int damage, Vector3 pos)
{
	// HPを減らす
	m_hp -= damage;
	m_pHpGauge->SetValue(m_hp);

	// ダメージエフェクトの再生
	Effekseer3DEffectManager::GetInstance().PlayEffect(
		m_damageEffectHandle,
		EffectID::enemy_boss_hit_effect,
		pos,
		{ 100.0f, 100.0f, 100.0f }
	);

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

// 死亡演出
void BossMatrix::PerformDeathEffect()
{
	// 死亡してから特定のフレームが経過したら開始
	if (m_dieIdleFrame-- <= 0)
	{
		// 横揺れ
		m_pos.x += sinf(m_dieShakeFrame++ * died_swing_speed) * died_swing_width;

		// 徐々にY座標を下げる
		m_pos.y -= 0.5f;

		// エフェクトの再生間隔が経過したら
		if (m_dieEffectIntervalFrame-- <= 0)
		{
			// エフェクトデータ
			DieEffectData data{};

			// エフェクトの発生位置をプレイヤーの周りにランダムに設定
			data.pos =
			{
				// エフェクトの発生位置を-500～500の間でランダムに設定
				m_pos.x + GetRand(1000) - 500,
				m_pos.y + GetRand(1000) - 500,
				m_pos.z
			};

			// エフェクトの大きさを10倍から100倍の間でランダムに設定
			data.scale = GetRand(30) + 10;

			// xyのベクトルをランダム作成
			data.vec = { static_cast<float>(GetRand(10) - 5), static_cast<float>(GetRand(10) - 5), 0.0f };
			data.vec = Vector3::FromDxLibVector3(VNorm(data.vec.ToDxLibVector3()));
			data.vec = Vector3::FromDxLibVector3(VScale(data.vec.ToDxLibVector3(), 50.0f));

			// エフェクトの再生
			Effekseer3DEffectManager::GetInstance().PlayEffectFollow(
				data.effectHandle,
				EffectID::enemy_died,
				&data.pos,
				{ data.scale, data.scale, data.scale },
				0.5f);

			// テーブルに追加
			m_dieEffectTable.push_back(data);

			// フレームの初期化
			m_dieEffectIntervalFrame = die_effect_interval_frame;
		}
		// 三角形エフェクトの更新
		m_pTriangle->Update(m_pos);

		// 三角形エフェクトが終了したら
		if (m_pTriangle->IsEnd())
		{
			// フラッシュの更新
			Vector3 screenPos = Vector3::FromDxLibVector3(ConvWorldPosToScreenPos(m_pos.ToDxLibVector3()));
			m_pFlash->Update({screenPos.x, screenPos.y}, 0xffffff);

			// フラッシュが終了したら
			if (m_pFlash->IsEnd())
			{
				// ゲームクリア
				m_isEnabled = false;
			}
		}
	}

	// エフェクトの更新
	for (auto& effect : m_dieEffectTable)
	{
		effect.pos += effect.vec;
	}
}

// 通常レーザー攻撃の開始
void BossMatrix::EntarStopNormalLaserAttack()
{
	// フレームの初期化
	m_laserFrame = stop_normal_laser_attack_frame;

	// 通常レーザー発射用のアニメーションに変更
	m_pModel->ChangeAnimation(laser_fire_anim_no, true, false, 8);

	// レーザーの生成
	m_laserKey = m_pLaserManager->AddLaser(LaserType::NORMAL, shared_from_this(), 140, 10000, 3.0f, false);
}

// 死亡演出の開始
void BossMatrix::EntarDie()
{
	// レーザーを削除
	m_pLaserManager->DeleteLaser(m_laserKey);

	// アニメーションの停止
	m_pModel->StopAnim();

	// 画面揺れ開始
	m_pScreenShaker->StartShake({ 100.0f, 0.0f }, 60);

	// インスタンスの生成
//	m_pFlash = std::make_unique<Flash>(died_flash_frame);
//	m_pTriangle = std::make_unique<Triangle>(died_continue_frame);
}

// 移動しながら通常レーザー攻撃の開始
void BossMatrix::EntarMoveNormalLaserAttack()
{
	// フレームの初期化
	m_laserFrame = move_normal_laser_attack_frame;

	// 通常レーザー発射用のアニメーションに変更
	m_pModel->ChangeAnimation(laser_fire_anim_no, true, false, 8);

	// レーザーの生成
	m_laserKey = m_pLaserManager->AddLaser(LaserType::NORMAL, shared_from_this(), 140, 10000, 5.0f, true);

	// 移動の初期化
	InitMove();
}

// キューブレーザー攻撃の開始
void BossMatrix::EntarCubeLaserAttack()
{
	// フレームの初期化
	m_laserFrame = cube_laser_attack_frame;

	// キューブレーザー発射用のアニメーションに変更
	m_pModel->ChangeAnimation(laser_fire_anim_no, true, false, 8);
}

// 登場時の更新
void BossMatrix::UpdateEntry()
{
	// 不透明度を上げる
	m_opacity += 0.005f;
	m_opacity = (std::min)(m_opacity, 1.0f);
	m_pModel->SetOpacity(m_opacity);

	// 移動
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
	
	// 不透明度が1.0fを超えたら
	// HPゲージの登場演出が終了したら
	// 目的地に到着したら
	if (m_opacity >= 1.0f && 
		m_pHpGauge->IsEndBurst() &&
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
	// 待機アニメーションに変更
	m_pModel->ChangeAnimation(idle_anim_no, true, false, 8);

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
	// 特定のフレームが経過したら演出開始
	if (m_dieIdleFrame-- <= 0)
	{
		// UIを格納
		UIManager::GetInstance().Store();

		// まだエフェクトが再生されていなかったら
		if (m_dieEffectHandle == -1)
		{
			// エフェクトの再生
			Effekseer3DEffectManager::GetInstance().PlayEffect(
				m_dieEffectHandle,
				EffectID::enemy_boss_die,
				m_pos,
				{ 100.0f, 100.0f, 100.0f });
		}
		// エフェクトの再生を開始してから、一定フレーム経過したら
		else if (m_dieDrawStopFrame-- <= 0)
		{
			// 存在フラグを下げる
			m_isEnabled = false;
		}
	}
	// 死亡演出
//	PerformDeathEffect();
}

// 移動しながら通常レーザー攻撃
void BossMatrix::UpdateMoveNormalLaserAttack()
{
	// レーザーの発射位置の更新
	Vector3 pos = Vector3::FromDxLibVector3(
		MV1GetFramePosition(m_pModel->GetModelHandle(), normal_laser_fire_frame));
	m_laserFirePos = { pos.x, pos.y, pos.z - 200.0f };

	// レーザーの方向に向けるようにする
	Vector3 directionVec = m_pLaserManager->GetLaserData(m_laserKey).pLaser->GetDirection();
	Matrix rotMtx = Matrix::GetRotationMatrix(init_model_direction, directionVec);
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

// 移動しながらホーミングレーザー攻撃
void BossMatrix::UpdateMoveHomingLaserAttack()
{
}

// 通常レーザー攻撃
void BossMatrix::UpdateStopNormalLaserAttack()
{
	// レーザーの発射位置の更新
	Vector3 pos = Vector3::FromDxLibVector3(
		MV1GetFramePosition(m_pModel->GetModelHandle(), normal_laser_fire_frame));
	m_laserFirePos = { pos.x, pos.y, pos.z - 200.0f };

	// レーザーの方向に向けるようにする
	Vector3 directionVec = m_pLaserManager->GetLaserData(m_laserKey).pLaser->GetDirection();
	Matrix rotMtx = Matrix::GetRotationMatrix(init_model_direction, directionVec);
	m_rot = { rotMtx.ToEulerAngle().x * -1, rotMtx.ToEulerAngle().y + DX_PI_F, rotMtx.ToEulerAngle().z * -1 };

	// フレームが経過したら
	if (m_laserFrame-- <= 0)
	{
		// ステートを待機に変更
		m_stateMachine.SetState(State::IDLE);

		// レーザーの削除
		m_pLaserManager->DeleteLaser(m_laserKey);
	}
}

// キューブレーザー攻撃
void BossMatrix::UpdateCubeLaserAttack()
{
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
			int key = m_pLaserManager->AddCubeLaser(firePos);
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
	float z = (fabs(GetCameraPosition().z - m_pPlayer->GetPos().z) + 1300.0f) / GetCameraFar();
	m_goalPos = Vector3::FromDxLibVector3(ConvScreenPosToWorldPos_ZLinear({ itr->x, itr->y, z }));
	m_goalPos.z = m_pPlayer->GetPos().z + itr->z;

	// 移動ベクトルの設定
	m_moveVec = (m_goalPos - m_pos).Normalized() * m_moveSpeed;
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
	float z = (fabs(GetCameraPosition().z - m_pPlayer->GetPos().z) + 1300.0f) / GetCameraFar();
	auto& screenSize = Application::GetInstance().GetWindowSize();
	m_goalPos = Vector3::FromDxLibVector3(ConvScreenPosToWorldPos_ZLinear({ screenSize.width / 2.0f, screenSize.height / 2.0f, z }));
	m_goalPos.z = m_pPlayer->GetPos().z + 1300.0f;

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