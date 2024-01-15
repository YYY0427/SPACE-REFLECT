#include "NormalLaser.h"
#include "../Enemy/EnemyBase.h"
#include "../Player.h"
#include "../../Math/Vector2.h"
#include "../../Application.h"
#include "../../Effect/Effekseer3DEffectManager.h"
#include "../../Model.h"
#include <DxLib.h>

namespace
{
	// モデル
	const Vector3 init_model_direction = { 1.0f, 0.0f, 0.0f };	// 初期方向
	const Vector3 laser_model_scale = { 0.1f, 0.1f, 0.1f };		// 拡大率

	// エフェクト
	const Vector3 laser_effect_direction = { 0.0f, 0.0f, -1.0f };	// 初期方向
	const Vector3 laser_effect_scale = { 24.0f, 24.0f, 24.0f };		// 拡大率
	constexpr int laser_effect_play_speed = 1.5f;		// 再生速度
	constexpr int laser_effect_total_play_frame = 100;	// 再生フレーム数
	constexpr int laser_effect_charge_frame = 150;		// チャージフレーム

	// プレイヤーを追従しない場合の向かう位置
	const Vector2 window_size = { Application::GetInstance().GetWindowSize().width , Application::GetInstance().GetWindowSize().height };
	const Vector2 goal_pos[] = 
	{
		{ 0 + 100, 0 + 100 },
		{ window_size.x - 100, 0 + 100 },
		{ 0 + 100, window_size.y - 100 },
		{ window_size.x - 100, window_size.y - 100 },
		{ window_size.x / 2, window_size.y / 2 },
	};
}

// コンストラクタ
NormalLaser::NormalLaser(int modelHandle, std::shared_ptr<EnemyBase> pEnemy, std::shared_ptr<Player> pPlayer, int laserFireFrame, float laserSpeed, bool isPlayerFollowing) :
	m_pEnemy(pEnemy),
	m_pPlayer(pPlayer),
	m_laserFireFrame(laserFireFrame),
	m_isPlayerFollowing(isPlayerFollowing)
{
	// 初期化
	m_pos = m_pEnemy->GetLaserFirePos();
	m_scale = laser_model_scale;

	// エフェクトのチャージ時間フレームとエフェクトの再生速度からチャージ時間が何フレームなのか求める
	m_chargeEffectTimer = laser_effect_charge_frame / laser_effect_play_speed;

	// プレイヤーを追従する場合	
	if (m_isPlayerFollowing)
	{
		// プレイヤーの位置を目的地に設定
		m_moveVec = (m_pPlayer->GetPos() - m_pos).Normalized() * laserSpeed;
	}
	// プレイヤーを追従しない場合
	else
	{
		// ランダムで目的地を設定
		Vector2 screenPos = goal_pos[GetRand(5)];
		Vector3 goalPos = Vector3::FromDxLibVector3(ConvScreenPosToWorldPos({ screenPos.x,screenPos.y, 0.0f }));
		m_moveVec = (goalPos - m_pos).Normalized() * laserSpeed;
	}

	// ベクトル方向の回転行列を作成
	m_rotMtx = Matrix::GetRotationMatrix(init_model_direction, m_moveVec);

	// ベクトル方向の回転行列からオイラー角を出力
	Matrix effectRotMtx = Matrix::GetRotationMatrix(laser_effect_direction, m_moveVec);
	Vector3 effectRot = effectRotMtx.ToEulerAngle();

	// エフェクトの再生
	Effekseer3DEffectManager::GetInstance().PlayEffectFollow(
		m_laserEffectHandle,
		EffectID::infinity_laser,
		&m_pos,
		laser_effect_scale,
		laser_effect_play_speed,
		effectRot);

	// 状態の追加
	m_stateMachine.AddState(State::CHARGE, {}, [this]() { UpdateCharge(); }, {});
	m_stateMachine.AddState(State::FIRE_PLYER_FOLLOWING, {}, [this]() { UpdateFirePlayerFollowing(); }, {});
	m_stateMachine.AddState(State::NORMAL_FIRE, {}, [this]() { UpdateNormalFire(); }, {});
	m_stateMachine.SetState(State::CHARGE);

	// 当たり判定に使用するモデルの設定
	m_pModel = std::make_unique<Model>(modelHandle);	// インスタンス生成
	m_pModel->SetUseCollision(true);					// 当たり判定設定
	m_pModel->SetScale(m_scale);	// 拡大率
	m_pModel->SetRotMtx(m_rotMtx);	// 回転行列
	m_pModel->SetPos(m_pos);		// 位置
	m_pModel->SetOpacity(m_opacity);// 不透明度
	m_pModel->Update();				// 当たり判定の更新
}

// デストラクタ
NormalLaser::~NormalLaser()
{
}

// 更新
void NormalLaser::Update()
{
	// 常に発射元の位置に合わせる
	m_pos = m_pEnemy->GetLaserFirePos();

	// 状態の更新
	m_stateMachine.Update();
}

// チャージ状態の更新
void NormalLaser::UpdateCharge()
{
}

// 通常発射状態の更新
void NormalLaser::UpdateNormalFire()
{
}

// プレイヤーを追従して発射状態の更新
void NormalLaser::UpdateFirePlayerFollowing()
{
}

// 描画
void NormalLaser::Draw()
{
}
