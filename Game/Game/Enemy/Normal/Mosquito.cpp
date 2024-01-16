#include "Mosquito.h"
#include "../../../Model.h"

namespace
{
	// アニメーション番号
	constexpr int idle_anim_num = 1;	// 待機

	// 目的地に到達したかどうかの判定
	// 判定の閾値（適切な値に調整する必要）
	constexpr float distance_thres_hold = 0.1f;  
}

// コンストラクタ
Mosquito::Mosquito(int modelHandle, 
	EnemyData data, 
	std::shared_ptr<Player> pPlayer, 
	std::shared_ptr<LaserManager> pLaserManager) :
	m_idleFrame(0)
	/*m_laserFireFrame(0),
	m_laserFireIdleFrame(0),
	m_laserSpeed(0),
	m_laserType({}),
	m_idleFrame(0)*/
{
	// 初期化
	m_pPlayer = pPlayer;
	m_pLaserManager = pLaserManager;
	m_actionDataList = data.actionDataList;
	m_isEnabled = true;
	m_pos = data.pos;
	m_hp = data.hp;
	m_moveSpeed = data.speed;
	m_attackPower = data.attack;
	m_opacity = 1.0f;
	m_rot = { 0, 0 ,0 };

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
	m_pModel->SetRot(m_rot);
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
		return;
	}

	// 現在の移動ポイントのイテレーターの取得
	auto itr = m_actionDataList.begin();
	std::advance(itr, m_movePointIndex);

	// 目的地の設定
	m_goalPos = itr->goalPos;
	m_isGoal = false;

	// 移動ベクトルの設定
	m_moveVec = (itr->goalPos - m_pos).Normalized() * m_moveSpeed;
}

// 更新
void Mosquito::Update()
{
	// ステートマシンの更新
	m_state.Update();

	// モデルの設定
	m_pModel->RestoreAllMaterialDifColor();	// ディフューズカラーを元に戻す
	m_pModel->SetOpacity(m_opacity);
	m_pModel->SetRot(m_rot);
	m_pModel->SetPos(m_pos);
	m_pModel->Update();
}

// 待機状態の更新
void Mosquito::UpdateIdle()
{
	// 待機フレームの更新
	m_actionData.idleFrame--;

	// 待機フレームが終わったら
	if (m_actionData.idleFrame <= 0)
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
		/* 到達 */

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
			//// レーザーの設定
			//m_laserType = static_cast<LaserType>(itr->laserType);	// レーザーの種類
			//m_laserFireIdleFrame = itr->laserIdleFrame;	// レーザー発射までの待機フレーム
			//m_laserFireFrame = itr->laserFireFrame;		// レーザー発射フレーム
			//m_laserSpeed = itr->laserSpeed;				// レーザーの速度
			//m_idleFrame = itr->idleFrame;				// 移動ポイントの待機フレーム

			// レーザー発射
			m_state.SetState(State::ATTACK);
		}
		else
		{
			// 移動ポイントの待機フレームを設定
		//	m_idleFrame = itr->idleFrame;

			// レーザー発射フラグが立っていなかったら
			m_state.SetState(State::IDLE);
		}

		// 次の移動ポイントへ
		m_movePointIndex++;	
	}
	else
	{
		// 目的地に到達していない場合
		m_isGoal = false;
	}
}

// 攻撃状態の更新
void Mosquito::UpdateAttack()
{
	// レーザー発射までの待機フレームの更新
	m_actionData.laserFireFrame--;

	// レーザー発射までの待機フレームが終わったら
	if (m_actionData.laserFireFrame <= 0)
	{
		// レーザーの発射
		m_pLaserManager->AddLaser(
			m_actionData.laserType,
			static_cast<std::shared_ptr<EnemyBase>>(this), 
			m_actionData.laserFireFrame,
			m_actionData.laserSpeed,
			m_actionData.isPlayerFollowing);

		// 待機状態に遷移
		m_state.SetState(State::IDLE);
	}
}

// 死亡状態の更新
void Mosquito::UpdateDead()
{
}

// 描画
void Mosquito::Draw()
{
	m_pModel->Draw();
}