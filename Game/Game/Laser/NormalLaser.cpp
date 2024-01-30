#include "NormalLaser.h"
#include "../Enemy/EnemyBase.h"
#include "../Player.h"
#include "../../Math/Vector2.h"
#include "../../Application.h"
#include "../../Effect/Effekseer3DEffectManager.h"
#include "../../Model.h"
#include "../../MyDebug/DebugText.h"
#include <DxLib.h>
#include <random>
#include <algorithm>

namespace
{
	// モデル
	const Vector3 init_model_direction = { 1.0f, 0.0f, 0.0f };	// 初期方向
	const Vector3 laser_model_scale = { 0.1f, 0.1f, 0.1f };		// 拡大率

	// エフェクト
	const Vector3 init_laser_effect_direction = { 0.0f, 0.0f, -1.0f };	// 初期方向
	const Vector3 laser_effect_scale = { 24.0f, 24.0f, 24.0f };		// 拡大率
	constexpr float laser_effect_play_speed = 1.0f;		// 再生速度
	constexpr int init_laser_effect_charge_frame = 140;	// 元々のチャージフレーム
	constexpr float laser_effect_z_length = 300.0f;		// Z軸の長さ

	// フレーム
	constexpr int laser_end_frame_no = 0;	// レーザーの端のフレーム番号

	// 目的地に到達したかどうかの判定
	// 判定の閾値（適切な値に調整する必要）
	constexpr float distance_threshold = 5.0f;

	// カメラのNearを0、Farを1としたときのZ座標
	constexpr float near_far_z_pos = 0.0f;

	// 反射中にシールドに何フレーム当たっていなかったら反射を解除するか
	constexpr int reflect_collision_shield_frame = 5;

	// プレイヤーを追従しない場合の向かう位置
	const Vector2 window_size = 
		{ static_cast<float>(Application::GetInstance().GetWindowSize().width), 
		static_cast<float>(Application::GetInstance().GetWindowSize().height )};
	const Vector2 goal_pos[] = 
	{
		{ 0 - 300, 0 - 300 },
		{ window_size.x + 300, 0 - 300 },
		{ 0 - 300, window_size.y + 300 },
		{ window_size.x + 300, window_size.y + 300 },
		{ window_size.x / 2, window_size.y / 2 },
	};
}

// コンストラクタ
NormalLaser::NormalLaser(int modelHandle, std::shared_ptr<EnemyBase> pEnemy, std::shared_ptr<Player> pPlayer, int laserChargeFrame, int laserFireFrame, float laserSpeed, bool isPlayerFollowing) :
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

	// エフェクトのチャージフレームを設定
	m_chargeEffectFrame = laserChargeFrame;

	// エフェクトのチャージフレームから再生速度を算出
	float effectSpeed = static_cast<float>(init_laser_effect_charge_frame) / static_cast<float>(m_chargeEffectFrame);

	// プレイヤーを追従しない場合
	if(!m_isPlayerFollowing)
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

		// 目的地を設定
		Vector2 screenPos = m_normalFireMovePointList.front();
		m_normalFireGoalPos = Vector3::FromDxLibVector3(ConvScreenPosToWorldPos({ screenPos.x,screenPos.y, near_far_z_pos }));

	}
	// レーザーの向く座標を設定
	m_directionPos = m_pPlayer->GetPosLogTable().back();

	// ベクトル方向の回転行列を作成
	m_rotMtx = Matrix::GetRotationMatrix(init_model_direction, (m_directionPos - m_pos).Normalized());

	// ベクトル方向の回転行列からオイラー角を出力
	Matrix effectRotMtx = Matrix::GetRotationMatrix(init_laser_effect_direction, (m_directionPos - m_pos).Normalized());
	Vector3 effectRot = effectRotMtx.ToEulerAngle();

	// エフェクトの再生
	Effekseer3DEffectManager::GetInstance().PlayEffectFollow(
		m_laserEffectHandle,
		EffectID::normal_laser,
		&m_pos,
		laser_effect_scale,
		effectSpeed,
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
	// 発射元の敵が死んでいたら
	if (!m_pEnemy->IsEnabled())
	{
		// 存在フラグを下げる
		m_isEnabled = false;
		return;
	}

	// ステートがチャージ状態でない場合
	if(m_stateMachine.GetCurrentState() != State::CHARGE)
	{
		// エフェクトの再生速度の設定
		Effekseer3DEffectManager::GetInstance().SetEffectSpeed(m_laserEffectHandle, 1.0f);

		// レーザーの発射フレームが0以下になったら
		if (m_laserFireFrame-- <= 0)
		{
			// 存在フラグを下げる
			m_isEnabled = false;
		}
	}
	
	// レーザーが反射状態なら
	if (m_isReflect)
	{
		// フレームを加算
		// 一定フレーム経過したら反射状態を解除
		if (m_reflectFrame++ > reflect_collision_shield_frame)
		{
			// 反射状態を解除
			UndoReflect();
		}
	}
	
	// 状態の更新
	m_stateMachine.Update();

	// 常に発射元の位置に合わせる
	m_pos = m_pEnemy->GetLaserFirePos();

	// レーザーの先端の座標を設定
	m_endPos = Vector3::FromDxLibVector3(
		MV1GetFramePosition(m_pModel->GetModelHandle(), laser_end_frame_no));

	// ベクトル方向の回転行列を作成
	m_rotMtx = Matrix::GetRotationMatrix(init_model_direction, (m_directionPos - m_pos).Normalized());

	// ベクトル方向の回転行列からオイラー角を出力
	Matrix effectRotMtx = Matrix::GetRotationMatrix(init_laser_effect_direction, (m_directionPos - m_pos).Normalized());
	Vector3 effectRot = effectRotMtx.ToEulerAngle();

	// エフェクトの回転率を設定
	Effekseer3DEffectManager::GetInstance().SetEffectRot(m_laserEffectHandle, effectRot);

	// レーザーの反射フラグが立っていない場合
	if (!m_isReflect)
	{
		// エフェクトの拡大率を設定
		Effekseer3DEffectManager::GetInstance().SetEffectScale(m_laserEffectHandle, laser_effect_scale);
	}

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
	// チャージ時間が終わったら
	if(m_chargeEffectFrame-- <= 0)
	{
		// モデルの拡大率を設定
		m_scale.x = -1.0f;

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
	if (m_directionPos.Distance(m_normalFireGoalPos) < distance_threshold)
	{
		// 目的地に到達したら次の目的地を設定
		m_normalFireMovePointIndex++;

		// 次の目的地がリストの最後まで到達したら
		if (m_normalFireMovePointIndex >= m_normalFireMovePointList.size())
		{
			// インデックスを初期化
			m_normalFireMovePointIndex = 0;

			// 地点の移動順序の入れ替え(配列の中身をシャッフル)
			std::random_device seed;
			std::mt19937 engine(seed());
			std::shuffle(m_normalFireMovePointList.begin(), m_normalFireMovePointList.end(), engine);
		}

		// 次の目的地を設定
		Vector2 screenPos = m_normalFireMovePointList[m_normalFireMovePointIndex];
		m_normalFireGoalPos = Vector3::FromDxLibVector3(ConvScreenPosToWorldPos({ screenPos.x,screenPos.y, near_far_z_pos }));
	}
	else
	{
		// ゴールの座標を設定
		m_normalFireGoalPos = Vector3::FromDxLibVector3(
			ConvScreenPosToWorldPos(
				{ m_normalFireMovePointList[m_normalFireMovePointIndex].x,
				  m_normalFireMovePointList[m_normalFireMovePointIndex].y,
				  near_far_z_pos }));

		// ベクトルを設定
		m_directionVec = (m_normalFireGoalPos - m_directionPos).Normalized() * m_speed;

		// ベクトルがNaNだったら
		if (std::isnan(m_directionVec.x) || std::isnan(m_directionVec.y) || std::isnan(m_directionVec.z))
		{
			// 0に設定
			m_directionVec = { 0.0f, 0.0f, 0.0f };
		}

		// 座標を更新
		m_directionPos += m_directionVec;
	}
}

// プレイヤーを追従して発射状態の更新
void NormalLaser::UpdateFirePlayerFollowing()
{
	// ベクトルを設定
	m_directionVec = (m_pPlayer->GetPos() - m_directionPos).Normalized() * m_speed;

	// ベクトルがNaNだったら
	if (std::isnan(m_directionVec.x) || std::isnan(m_directionVec.y) || std::isnan(m_directionVec.z))
	{
		// 0に設定
		m_directionVec = { 0.0f, 0.0f, 0.0f };
	}

	// 座標の更新
	m_directionPos += m_directionVec;
}

// 描画
void NormalLaser::Draw()
{
#ifdef _DEBUG
	// モデルの描画
	SetUseLighting(false);
//	m_pModel->Draw();
	SetUseLighting(true);

	DebugText::Log("レーザーの向く座標", { m_directionPos.x, m_directionPos.y, m_directionPos.z });
#endif 
}

// レーザーを止める
void NormalLaser::Stop(Vector3 pos)
{
	// レーザーの反射フラグを立てる
	m_isReflect = true;

	// シールドに当たったので初期化
	m_reflectFrame = 0;

	// レーザーの発射地点からシールドまでの距離を算出
	Vector3 vec = pos - m_pos;
	float length = vec.Length();

	// レーザーのエフェクトがシールドで止まってるよう見えるように、エフェクトの拡大率を設定
	Effekseer3DEffectManager::GetInstance().SetEffectScale(
		m_laserEffectHandle, { laser_effect_scale.x, laser_effect_scale.y, length / laser_effect_z_length });

	// レーザーの当たり判定用モデルを反対側に向ける
	m_pModel->SetScale(m_scale);
	m_pModel->Update();
}

// 反射された状態から元に戻す
void NormalLaser::UndoReflect()
{
	// 初期化
	m_isReflect = false;
	m_reflectFrame = 0;
}

// 方向ベクトルの取得
Vector3 NormalLaser::GetDirection() const
{
	return (m_directionPos - m_pos);
}
