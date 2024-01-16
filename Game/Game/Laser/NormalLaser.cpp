#include "NormalLaser.h"
#include "../Enemy/Normal/EnemyBase.h"
#include "../Player.h"
#include "../../Math/Vector2.h"
#include "../../Application.h"
#include "../../Effect/Effekseer3DEffectManager.h"
#include "../../Model.h"
#include <DxLib.h>
#include <random>
#include <algorithm>

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

	// 目的地に到達したかどうかの判定
	// 判定の閾値（適切な値に調整する必要）
	constexpr float distance_thres_hold = 0.1f;

	// プレイヤーを追従しない場合の向かう位置
	const Vector2 window_size = { static_cast<float>(Application::GetInstance().GetWindowSize().width) , static_cast<float>(Application::GetInstance().GetWindowSize().height )};
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
	m_isPlayerFollowing(isPlayerFollowing),
	m_normalFireMovePointIndex(0)
{
	// 初期化
	m_pos = m_pEnemy->GetLaserFirePos();
	m_scale = laser_model_scale;
	m_speed = laserSpeed;

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
		// レーザーの目的地をリストに追加
		for (auto& movePoint : goal_pos)
		{
			m_normalFireMovePointList.push_back(movePoint);
		}

		// ランダムで目的地を設定(配列の中身をシャッフル)
		std::random_device seed;
		std::mt19937 engine(seed());
		std::shuffle(m_normalFireMovePointList.begin(), m_normalFireMovePointList.end(), engine);
		Vector2 screenPos = m_normalFireMovePointList.front();
		m_normalFireGoalPos = Vector3::FromDxLibVector3(ConvScreenPosToWorldPos({ screenPos.x,screenPos.y, 0.0f }));

		// レーザーの移動ベクトルを設定
		m_moveVec = (m_normalFireGoalPos - m_pos).Normalized() * laserSpeed;
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
	// エフェクトの削除
	Effekseer3DEffectManager::GetInstance().DeleteEffect(m_laserEffectHandle);
}

// 更新
void NormalLaser::Update()
{
	// ステートがチャージ状態でない場合
	if(m_stateMachine.GetCurrentState() != State::CHARGE)
	{
		// レーザーの発射フレームを減らす
		m_laserFireFrame--;

		// レーザーの発射フレームが0以下になったら
		if (m_laserFireFrame <= 0)
		{
			// 存在フラグを下げる
			m_isEnabled = false;
		}
	}
	
	// 状態の更新
	m_stateMachine.Update();

	// 常に発射元の位置に合わせる
	m_pos = m_pEnemy->GetLaserFirePos();

	// ベクトル方向の回転行列を作成
	m_rotMtx = Matrix::GetRotationMatrix(init_model_direction, m_moveVec);

	// ベクトル方向の回転行列からオイラー角を出力
	Matrix effectRotMtx = Matrix::GetRotationMatrix(laser_effect_direction, m_moveVec);
	Vector3 effectRot = effectRotMtx.ToEulerAngle();

	// エフェクトの回転率を設定
	Effekseer3DEffectManager::GetInstance().SetEffectRot(m_laserEffectHandle, effectRot);

	// モデルの設定
	m_pModel->SetRotMtx(m_rotMtx);	// 回転行列
	m_pModel->SetPos(m_pos);		// 位置
	m_pModel->SetOpacity(m_opacity);// 不透明度
	m_pModel->SetScale(m_scale);	// 拡大率
	m_pModel->Update();				// 当たり判定の更新
}

// チャージ状態の更新
void NormalLaser::UpdateCharge()
{
	// チャージ時間のタイマーの更新	
	m_chargeEffectTimer.Update(1);

	// チャージ時間が終わったら
	if(m_chargeEffectTimer.IsTimeOut())
	{
		// モデルの拡大率を設定
		m_scale.x *= -10.0f;

		// ステートの変更
		if (m_isPlayerFollowing)
		{
			// プレイヤーを追従する場合
			m_stateMachine.SetState(State::FIRE_PLYER_FOLLOWING);
		}
		else
		{
			// プレイヤーを追従しない場合
			m_stateMachine.SetState(State::NORMAL_FIRE);
		}
	}
}

// 通常発射状態の更新
void NormalLaser::UpdateNormalFire()
{
	// 目的地に到達したかどうかの判定
	if (m_pos.Distance(m_normalFireGoalPos) < distance_thres_hold)
	{
		// 目的地に到達したら次の目的地を設定
		m_normalFireMovePointIndex++;

		// 次の目的地がリストの最後まで到達したら最初に戻す
		if (m_normalFireMovePointIndex >= m_normalFireMovePointList.size())
		{
			m_normalFireMovePointIndex = 0;

			// 地点の移動順序の入れ替え(配列の中身をシャッフル)
			std::random_device seed;
			std::mt19937 engine(seed());
			std::shuffle(m_normalFireMovePointList.begin(), m_normalFireMovePointList.end(), engine);
		}

		// 次の目的地を設定
		Vector2 screenPos = m_normalFireMovePointList[m_normalFireMovePointIndex];
		m_normalFireGoalPos = Vector3::FromDxLibVector3(ConvScreenPosToWorldPos({ screenPos.x,screenPos.y, 0.0f }));

		// レーザーの移動ベクトルを設定
		m_moveVec = (m_normalFireGoalPos - m_pos).Normalized() * m_speed;
	}
}

// プレイヤーを追従して発射状態の更新
void NormalLaser::UpdateFirePlayerFollowing()
{
	// プレイヤーの位置を目的地に設定
	m_moveVec = (m_pPlayer->GetPos() - m_pos).Normalized() * m_speed;
}

// 描画
void NormalLaser::Draw()
{
#ifdef _DEBUG
	// モデルの描画
	m_pModel->Draw();
#endif 
}
