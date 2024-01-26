#include "BossMatrix.h"
#include "../../../Model.h"
#include "../../../UI/Gauge.h"
#include "../../../UI/StringUI.h"
#include "../../../UI/UIManager.h"
#include "../../../Math/MathUtil.h"
#include "../../../Application.h"
#include "../../Laser/LaserManager.h"
#include "../../Player.h"
#include "../../../Effect/Effekseer3DEffectManager.h"
#include <random>
#include <algorithm>

// TODO : HPが半分以下になったら透明になる(レーザーの発射場所はランダムにする)

namespace
{
	// 位置
	const Vector3 init_pos = { 0.0f, 300.0f, 5000.0f };				// 初期位置
	const Vector3 goal_init_pos = { 0.0f, 300.0f, 2000.0f };		// 登場時の位置

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
	constexpr float move_speed = 10.0f;			// 移動速度
	constexpr float distance_threshold = 10.0f; // 目的地に到達したかどうか測る閾値
	const Vector3 move_pos[] =					// 移動先の座標
	{
		goal_init_pos,
		{ 930, 300, 800 },
		{ 930, -300, 800 },
		{ -930, 300, 800 },
		{ -930, -300, 800 },
	};

	// 死亡
	constexpr float died_swing_width = 5.0f;	// 死亡時の横揺れの大きさ
	constexpr float died_swing_speed = 1.0f;	// 死亡時の横揺れの速さ
	constexpr int died_continue_frame = 60 * 5;	// 死亡時の演出の継続時間

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
	constexpr int die_idle_frame = 60 * 3;					// 死亡時の待機フレーム
	constexpr int die_shake_frame = 60 * 5;					// 死亡時の揺れるフレーム
	constexpr int die_effect_interval_frame = 20;			// 死亡時のエフェクトの再生間隔
}

// コンストラクタ
BossMatrix::BossMatrix(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LaserManager> pLaserManager) :
	m_attackStateIndex(0),
	m_isMoveEnd(false),
	m_idleFrame(0),
	m_laserKey(-1),
	m_laserFrame(0),
	m_damageEffectHandle(-1),
	m_dieIdleFrame(die_idle_frame),
	m_dieShakeFrame(die_shake_frame),
	m_dieEffectIntervalFrame(die_effect_interval_frame)
{
	// 初期化
	m_pPlayer = pPlayer;
	m_pLaserManager = pLaserManager;
	m_pos = m_pPlayer->GetPos() + init_pos;
	m_rot = model_rot;
	m_opacity = 0.0f;
	m_hp = max_hp;
	m_collisionRadius = collision_radius * model_scale.x;
	for (auto& point : move_pos)
	{
		m_movePointTable.push_back(point);
	}

	// ステートマシンの設定
	m_stateMachine.AddState(State::ENTRY, {}, [this]() {UpdateEntry(); }, {});
	m_stateMachine.AddState(State::IDLE, {}, [this]() {UpdateIdle(); }, {});
	m_stateMachine.AddState(State::DIE, {}, [this]() {UpdateDie(); }, {});
	m_stateMachine.AddState(State::MOVE_NORMAL_LASER_ATTACK, {}, [this]() {UpdateMoveNormalLaserAttack(); }, {});
	m_stateMachine.AddState(State::MOVE_HOMING_LASER_ATTACK, {}, [this]() {UpdateMoveHomingLaserAttack(); }, {});
	m_stateMachine.AddState(State::STOP_NORMAL_LASER_ATTACK, [this]() {EntarStopNormalLaserAttack(); }, [this]() {UpdateStopNormalLaserAttack(); }, {});
	m_stateMachine.AddState(State::CUBE_LASER_ATTACK, {}, [this]() {UpdateCubeLaserAttack(); }, {});
	m_stateMachine.SetState(State::ENTRY);

	// 攻撃ステートの追加
//	m_attackStateTable.push_back(State::MOVE_NORMAL_LASER_ATTACK);
//	m_attackStateTable.push_back(State::MOVE_HOMING_LASER_ATTACK);
	m_attackStateTable.push_back(State::STOP_NORMAL_LASER_ATTACK);
//	m_attackStateTable.push_back(State::CUBE_LASER_ATTACK);
	ShuffleAttackState();

	// HPゲージの設定
	m_pHpGauge = std::make_shared<Gauge>(
		hp_gauge_img_file_path, hp_gauge_back_img_file_path, hp_gauge_frame_img_file_path, max_hp, 
		hp_gauge_pos, hp_gauge_size, true, 2, true, 3);
	UIManager::GetInstance().AddUI("BossHPGauge", m_pHpGauge, 2, { 0, 1 });

	// ボス名前の設定
	m_pBossName = std::make_shared<StringUI>(boss_name_key);
	m_pBossName->SetPos(boss_name_pos);
	UIManager::GetInstance().AddUI("BossName", m_pBossName, 2, { 0, 1 });

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
	// HPゲージの更新
	m_pHpGauge->Update();	

	// Z座標の更新
	m_pos.z += m_pPlayer->GetMoveVec().z;

	// ステートマシンの更新
	m_stateMachine.Update();

	// プレイヤー方向の回転行列の取得
	Matrix rotMtx = Matrix::GetRotationMatrix(init_model_direction, (m_pPlayer->GetPos() - m_pos).Normalized());
	Vector3 rot = { rotMtx.ToEulerAngle().x * -1, rotMtx.ToEulerAngle().y + DX_PI_F, rotMtx.ToEulerAngle().z * -1 };

	// モデル設定
	m_pModel->RestoreAllMaterialDifColor();	// ディフューズカラーを元に戻す
	m_pModel->SetOpacity(m_opacity);	// 不透明度
	m_pModel->SetRot(rot);				// 向き
	m_pModel->SetPos(m_pos);			// 位置
	m_pModel->Update();					// モデルの当たり判定、アニメーションの更新
}

// 描画
void BossMatrix::Draw()
{
	// モデルの描画
	m_pModel->Draw();

#ifdef _DEBUG
	// 当たり判定の描画
	DrawSphere3D(m_pos.ToDxLibVector3(), m_collisionRadius, 16, 0xff0000, 0xff0000, false);
#endif
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

// 通常レーザー攻撃の開始
void BossMatrix::EntarStopNormalLaserAttack()
{
	// フレームの初期化
	m_laserFrame = stop_normal_laser_attack_frame;

	// 通常レーザー発射用のアニメーションに変更
	m_pModel->ChangeAnimation(laser_fire_anim_no, true, false, 8);

	// レーザーの生成
	m_laserKey = m_pLaserManager->AddLaser(LaserType::NORMAL, shared_from_this(), 140, 10000, 2.0f, false);
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
		Vector3 moveVec = ((goal_init_pos + m_pPlayer->GetPos()) - m_pos).Normalized() * entry_move_speed;

		// 移動
		m_pos += moveVec;

		// 目的地に到達したか 
		if (m_pos.Distance(goal_init_pos + m_pPlayer->GetPos()) < distance_threshold)
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
	// レーザーを削除
	m_pLaserManager->DeleteLaser(m_laserKey);

	// UIを格納
	UIManager::GetInstance().Store();
}

// 移動しながら通常レーザー攻撃
void BossMatrix::UpdateMoveNormalLaserAttack()
{

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

	// アニメーションが終了したら
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
}

// 攻撃ステートの順序をシャッフル
void BossMatrix::ShuffleAttackState()
{
	std::random_device seed;
	std::mt19937 engine(seed());
	std::shuffle(m_attackStateTable.begin(), m_attackStateTable.end(), engine);
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