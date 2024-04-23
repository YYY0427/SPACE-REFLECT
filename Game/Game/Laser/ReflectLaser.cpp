#include "ReflectLaser.h"
#include "../Shield.h"
#include "../../Math/Matrix.h"
#include "../../Math/MathUtil.h"
#include "../../Effect/Effekseer3DEffectManager.h"
#include "../../Game/Enemy/EnemyManager.h"
#include "../../Game/Enemy/EnemyBase.h"
#include "../../Model.h"
#include "../../MyDebug/DebugText.h"
#include "../../ModelHandleManager.h"
#include "../../SoundManager.h"

namespace
{
	// 初期方向
	const Vector3 init_model_direction        = { 1.0f, 0.0f, 0.0f };	// モデル
	const Vector3 init_laser_effect_direction = { 0.0f, 0.0f, -1.0f };	// エフェクト

	// 拡大率
	const Vector3 model_scale  = { 1.0f, 0.2f, 0.2f };		// モデル
//	const Vector3 model_scale  = { 1.0f, 0.1f, 0.1f };		// モデル
	const Vector3 effect_scale = { 70.0f, 70.0f, 40.0f };	// エフェクト
//	const Vector3 effect_scale = { 40.0f, 40.0f, 40.0f };	// エフェクト

	// レーザーの移動速度
	constexpr float move_speed = 35.0f;			

	// レーザーエフェクトの再生速度
	constexpr float effect_play_speed = 1.0f;

	// エイムアシスト
	constexpr float aim_assist_range = 3000.0f;	// エイムアシストの範囲
	constexpr float aim_assist_power = 0.45;	// エイムアシストの強さ(0.0～1.0)
}

// コンストラクタ
ReflectLaser::ReflectLaser(const std::shared_ptr<EnemyManager>& pEnemyManager, 
						   const std::shared_ptr<Shield>& pShield, 
						   const std::shared_ptr<LaserBase>& pLaser, 
						   const Vector3& firePos)
{
	// 初期化
	m_pos = firePos;
	m_pShield = pShield;
	m_pLaser = pLaser;
	m_pEnemyManager = pEnemyManager;
	m_isReflect = true;
	m_isEnabled = true;

	// 反射ベクトルを作成
	m_directionPos = Vector3::Reflect(m_pLaser->GetDirection(), Vector3::FromDxLibVector3(m_pShield->GetVertex().front().norm));

	// 指定した位置方向に向ける行列の作成
	Matrix rotEffectMtx = Matrix::GetRotationMatrix(init_laser_effect_direction, m_directionPos);	// エフェクト
	Matrix rotModelMtx  = Matrix::GetRotationMatrix(init_model_direction, -m_directionPos);			// モデル

	// レーザーエフェクトの再生
	Effekseer3DEffectManager::GetInstance().PlayEffectLoopAndFollow(
		m_laserEffectHandle, "ReflectLaser", &m_pos, effect_scale, effect_play_speed, rotEffectMtx.ToEulerAngle());

	// 元々のレーザー音を停止
	auto& soundManager = SoundManager::GetInstance();
	soundManager.StopSound("Laser");
	
	// レーザーの音の再生
	soundManager.PlaySE("ReflectLaser");

	// モデルの設定
	m_pModel = std::make_shared<Model>(ModelHandleManager::GetInstance().GetHandle("Laser"));	// インスタンス生成
	m_pModel->SetUseCollision(true);	// 当たり判定設定
	m_pModel->SetScale(model_scale);	// 拡大率
	m_pModel->SetRotMtx(rotModelMtx);	// 回転行列
	m_pModel->SetPos(m_pos);			// 位置
	m_pModel->Update();					// 当たり判定の更新
}											

// デストラクタ
ReflectLaser::~ReflectLaser()
{
	// エフェクトの削除
	Effekseer3DEffectManager::GetInstance().DeleteEffect(m_laserEffectHandle);

	// レーザーの音の停止
	SoundManager::GetInstance().StopSound("ReflectLaser");
}

// 更新
void ReflectLaser::Update()
{
	// 発射元のレーザーがシールドに反射していないまたは無効なら
	if (!m_pLaser->IsReflect() || !m_pLaser->IsEnabled())
	{
		// 無効にする
		m_isEnabled = false;
		return;
	}

	// レーザーが向いている方向と近い敵を探す
	Vector3 enemyPos{};	
	if (!m_pEnemyManager->GetEnemyList().empty())
	{
		enemyPos = m_pEnemyManager->GetEnemyList().front()->GetPos();
		float minDistance = enemyPos.Distance(m_directionPos);

		for (auto& enemy : m_pEnemyManager->GetEnemyList())
		{
			// 敵が有効なら
			if (enemy->IsEnabled())
			{
				// 敵とレーザーの向いている方向との距離を比較
				float distance = enemy->GetPos().Distance(m_directionPos);

				// より近い敵の座標を取得
				if (distance < minDistance)
				{
					enemyPos = enemy->GetPos();
					minDistance = distance;
				}
			}
		}
	}

	// ボスが生存しているなら
	if (m_pEnemyManager->IsBossAlive())
	{
		// ボスの座標を取得
		enemyPos = m_pEnemyManager->GetBossEnemy()->GetPos();
	}

	// Z軸を無効にする
	Vector3 directionPos = m_directionPos;
	directionPos.z = 0;
	enemyPos.z     = 0;

	// 敵とレーザーの向いている方向との距離が一定範囲内なら
	Vector3 aimAssistVec{};
	if (enemyPos.Distance(directionPos) < aim_assist_range)
	{
		// エイムアシストを有効にする
		aimAssistVec = (enemyPos - directionPos).Normalized();
	}

	// 反射ベクトルを作成
	Vector3 goalPos = Vector3::Reflect(
		m_pLaser->GetDirection(), Vector3::FromDxLibVector3(m_pShield->GetVertex().front().norm));
	Vector3 reflectVec = (goalPos - m_directionPos).Normalized();

	// ベクトルの取得
	float reflectVecPower = 1.0f - aim_assist_power;
	Vector3 moveVec = ((reflectVec * reflectVecPower) + (aimAssistVec * aim_assist_power)) * move_speed;

	// 向く方向を更新
	m_directionPos += moveVec;

	// 指定したベクトル方向に向ける行列の作成
	Matrix rotEffectMtx = Matrix::GetRotationMatrix(init_laser_effect_direction, m_directionPos);	// エフェクト
	Matrix rotModelMtx  = Matrix::GetRotationMatrix(init_model_direction, -m_directionPos);			// モデル

	// エフェクトの回転率を設定
	Effekseer3DEffectManager::GetInstance().SetEffectRot(m_laserEffectHandle, rotEffectMtx.ToEulerAngle());

	// モデルの設定
	m_pModel->SetRotMtx(rotModelMtx);	// 回転行列
	m_pModel->SetPos(m_pos);			// 位置
	m_pModel->Update();					// 当たり判定の更新
}													

// 描画
void ReflectLaser::Draw()
{
#ifdef _DEBUG
	// モデルの描画
//	m_pModel->Draw();
#endif 
}