#include "Mosquito.h"
#include "../../../Model.h"
#include "../../Player.h"
#include <DxLib.h>

namespace
{
	// アニメーション番号
	constexpr int idle_anim_num = 1;	// 待機

	// 目的地に到達したかどうかの判定
	// 判定の閾値（適切な値に調整する必要）
	constexpr float distance_thres_hold = 5.0f;  

	// 
	constexpr float afisajf = 0.9f;

	// モデルの初期の向いている方向
	const Vector3 init_model_direction = { 0, 0, -1 };
}

// コンストラクタ
Mosquito::Mosquito(int modelHandle, 
	EnemyData data, 
	std::shared_ptr<Player> pPlayer, 
	std::shared_ptr<LaserManager> pLaserManager) :
	m_idleFrame(0),
	m_laserKey(-1)
{
	// 初期化
	m_pPlayer = pPlayer;
	m_pLaserManager = pLaserManager;
	m_actionDataList = data.actionDataList;
	m_isEnabled = true;
	m_pos = Vector3::FromDxLibVector3(ConvScreenPosToWorldPos({data.pos.x, data.pos.y, afisajf }));
	m_pos.z = m_pPlayer->GetPos().z + data.pos.z;
	m_hp = data.hp;
	m_moveSpeed = data.speed;
	m_attackPower = data.attack;
	m_opacity = 1.0f;

	// プレイヤーを向くように回転行列を設定
	Matrix rotMtx = Matrix::GetRotationMatrix(init_model_direction, (m_pPlayer->GetPos() - m_pos).Normalized());

	// ステートマシンの設定
	m_state.AddState(State::IDLE, {}, [this](){ UpdateIdle(); }, {});
	m_state.AddState(State::MOVE, [this]() { EntarMove(); }, [this](){ UpdateMove(); }, {});
	m_state.AddState(State::ATTACK, {}, [this](){ UpdateAttack(); }, {});
	m_state.AddState(State::DEAD, {}, [this](){ UpdateDead(); }, {});
	m_state.SetState(State::MOVE);

	// モデルのインスタンスの作成
	m_pModel = std::make_shared<Model>(modelHandle);

	// モデルの設定
	m_pModel->SetOpacity(m_opacity);
	m_pModel->SetRotMtx(rotMtx);
	m_pModel->SetScale({ data.scale, data.scale, data.scale });
	m_pModel->SetPos(m_pos);
	m_pModel->ChangeAnimation(idle_anim_num, true, false, 8);
	m_pModel->Update();
}

// デストラクタ
Mosquito::~Mosquito()
{
}

// 移動状態に入る前の処理
void Mosquito::EntarMove()
{
	// 移動ポイントのインデックスが行動データのサイズを超えていたら
	if (m_movePointIndex >= m_actionDataList.size())
	{
		// 存在フラグを下げる
		m_isEnabled = false;

		// レーザーを発射していたら
		if (m_laserKey != -1)
		{
			// レーザーの削除
			m_pLaserManager->DeleteLaser(m_laserKey);
		}
		return;
	}

	// 目的地の取得
	GetGoalPos();

	// 初期化
	m_isGoal = false;
}

// 更新
void Mosquito::Update()
{
	// ステートマシンの更新
	m_state.Update();

	// レーザーの発射位置の更新
	m_laserFirePos = m_pos;

	// プレイヤーを向くように回転行列を設定
	Matrix rotMtx = Matrix::GetRotationMatrix(init_model_direction, (m_pPlayer->GetPos() - m_pos).Normalized());

	// モデルの設定
	m_pModel->RestoreAllMaterialDifColor();	// ディフューズカラーを元に戻す
	m_pModel->SetOpacity(m_opacity);
	m_pModel->SetRotMtx(rotMtx);
	m_pModel->SetPos(m_pos);
	m_pModel->Update();
}

// 待機状態の更新
void Mosquito::UpdateIdle()
{
	// Z座標の更新
	m_pos.z += m_pPlayer->GetMoveVec().z;

	// 待機フレームが終わったら
	if (m_actionData.idleFrame-- <= 0)
	{
		// 移動状態に遷移
		m_state.SetState(State::MOVE);
	}
}

// 移動状態の更新
void Mosquito::UpdateMove()
{
	// 目的地に到達したかの判定
	if (m_pos.Distance(m_goalPos) < distance_thres_hold &&
		!m_isGoal)
	{
		// 到達した場合、一回しか通ってほしくない為、
		// フラグを立てる
		m_isGoal = true;

		// 現在の移動ポイントのイテレーターの取得
		auto itr = m_actionDataList.begin();
		std::advance(itr, m_movePointIndex);

		// 行動データの設定
		m_actionData = *itr;

		// 現在の移動ポイントのレーザー発射フラグが立っていたら
		// レーザー発射状態に遷移
		if (itr->isLaser)
		{
			// レーザー発射
			m_state.SetState(State::ATTACK);
		}
		else
		{
			// レーザー発射フラグが立っていなかったら
			m_state.SetState(State::IDLE);
		}
		// 次の移動ポイントへ
		m_movePointIndex++;	
	}
	else
	{
		// 目的地の取得
		GetGoalPos();

		// 座標の更新
		m_pos += m_moveVec;
	}
}

// 攻撃状態の更新
void Mosquito::UpdateAttack()
{
	// Z座標の更新
	m_pos.z += m_pPlayer->GetMoveVec().z;

	// レーザー発射までの待機フレームが終わったら
	if (m_actionData.laserIdleFrame-- <= 0 && m_actionData.isLaser)
	{
		// レーザーの発射
		m_laserKey = m_pLaserManager->AddLaser(
			m_actionData.laserType,
			shared_from_this(),
			m_actionData.laserFireFrame,
			m_actionData.laserSpeed,
			m_actionData.isPlayerFollowing);

		// レーザーを発射したのでフラグを下げる
		m_actionData.isLaser = false;
	}
	// レーザー発射後、レーザー発射中フレームの更新
	else if (m_actionData.laserIdleFrame <= 0 && !m_actionData.isLaser)
	{
		// レーザー発射中フレームが終わったら
		if (m_actionData.laserFireFrame-- <= 0)
		{
			// 待機状態に遷移
			m_state.SetState(State::IDLE);
		}
	}
}

// 死亡状態の更新
void Mosquito::UpdateDead()
{
}

// 目的地の取得
void Mosquito::GetGoalPos()
{
	// 現在の移動ポイントのイテレーターの取得
	auto itr = m_actionDataList.begin();
	std::advance(itr, m_movePointIndex);

	// 目的地の設定
	m_goalPos = Vector3::FromDxLibVector3(ConvScreenPosToWorldPos({ itr->goalPos.x, itr->goalPos.y, afisajf }));
	m_goalPos.z = m_pPlayer->GetPos().z + itr->goalPos.z;

	// 移動ベクトルの設定
	m_moveVec = (m_goalPos - m_pos).Normalized() * m_moveSpeed;
}

// 描画
void Mosquito::Draw()
{
	// モデルの描画
	m_pModel->Draw();

#ifdef _DEBUG
	// 目的地の描画
	DrawSphere3D(m_goalPos.ToDxLibVector3(), 10.0f, 10, 0xff0000, 0xff0000, TRUE);
#endif
}