#include "ReflectLaser.h"
#include "../../Math/Matrix.h"
#include "../../Effect/Effekseer3DEffectManager.h"
#include "../../Model.h"
#include "../Shield.h"
#include "../../ModelHandleManager.h"

namespace
{
	// 初期方向
	const Vector3 init_model_direction = { 1.0f, 0.0f, 0.0f };			// モデル
	const Vector3 init_laser_effect_direction = { 0.0f, 0.0f, -1.0f };	// エフェクト

	// 拡大率
	const Vector3 model_scale = { 1.0f, 0.1f, 0.1f };		// モデル
	const Vector3 effect_scale = { 40.0f, 40.0f, 40.0f };	// エフェクト

	// 移動速度
	constexpr float move_speed = 50.0f;	
}

// コンストラクタ
ReflectLaser::ReflectLaser(std::shared_ptr<Shield> pShield, std::shared_ptr<LaserBase> pLaser, Vector3 firePos)
{
	// 初期化
	m_pos = firePos;
	m_pShield = pShield;
	m_pLaser = pLaser;
	m_isReflect = true;
	m_isEnabled = true;

	// 反射ベクトルを作成
	m_directionVec = Vector3::Reflect(m_pLaser->GetDirection(), Vector3::FromDxLibVector3(m_pShield->GetVertex()[0].norm));

	// 指定した位置方向に向ける行列の作成
	Matrix rotEffectMtx = Matrix::GetRotationMatrix(init_laser_effect_direction, m_directionVec);
	Matrix rotMtx = Matrix::GetRotationMatrix(init_model_direction, -m_directionVec);

	// レーザーエフェクトの再生
	Effekseer3DEffectManager::GetInstance().PlayEffectLoopAndFollow(
		m_laserEffectHandle, EffectID::reflect_laser, &m_pos, effect_scale, 1.0f, rotEffectMtx.ToEulerAngle());

	// モデルの設定
	m_pModel = std::make_shared<Model>(ModelHandleManager::GetInstance().GetHandle(ModelType::LASER));	// インスタンス生成
	m_pModel->SetUseCollision(true);					// 当たり判定設定
	m_pModel->SetScale(model_scale);					// 拡大率
	m_pModel->SetRotMtx(rotMtx);						// 回転行列
	m_pModel->SetPos(m_pos);							// 位置
	m_pModel->Update();									// 当たり判定の更新
}

// デストラクタ
ReflectLaser::~ReflectLaser()
{
	// エフェクトの削除
	Effekseer3DEffectManager::GetInstance().DeleteEffect(m_laserEffectHandle);
}

// 更新
void ReflectLaser::Update()
{
	// 発射元のレーザーがシールドに反射していなかったら消す
	if (!m_pLaser->IsReflect() || !m_pLaser->IsEnabled())
	{
		m_isEnabled = false;
		return;
	}

#if false
	m_directionVec = Vector3::Reflect(
		m_pLaser->GetDirection(), Vector3::FromDxLibVector3(m_pShield->GetVertex()[0].norm));
#else
	// 反射ベクトルを作成
	Vector3 goalPos = Vector3::Reflect(
		m_pLaser->GetDirection(), Vector3::FromDxLibVector3(m_pShield->GetVertex()[0].norm));

	// ベクトルの取得
	Vector3 moveVec = (goalPos - m_directionVec).Normalized() * move_speed;

	// 向く方向を更新
	m_directionVec += moveVec;
#endif

	// 指定したベクトル方向に向ける行列の作成
	Matrix rotEffectMtx = Matrix::GetRotationMatrix(init_laser_effect_direction, m_directionVec);
	Matrix rotMtx = Matrix::GetRotationMatrix(init_model_direction, -m_directionVec);

	// エフェクトの回転率を設定
	Effekseer3DEffectManager::GetInstance().SetEffectRot(m_laserEffectHandle, rotEffectMtx.ToEulerAngle());

	// モデルの設定
	m_pModel->SetRotMtx(rotMtx);						// 回転行列
	m_pModel->SetPos(m_pos);							// 位置
	m_pModel->Update();									// 当たり判定の更新
}

// 描画
void ReflectLaser::Draw()
{
#ifdef _DEBUG
	// モデルの描画
//	m_pModel->Draw();
#endif 
}