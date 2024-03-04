#include "CubeLaser.h"
#include "../../Model.h"
#include "../../Math/MathUtil.h"
#include "../Player.h"
#include "../../ModelHandleManager.h"
#include <DxLib.h>

namespace
{
	// 拡大速度
	constexpr float scale_speed = 0.0002f;

	// 不透明度の上昇速度
	constexpr float opacity_speed = 0.005f;

	// 不透明度の最大値
	constexpr float max_opacity = 1.0f;

	// 1フレームに回転する角度
	const float max_delta_rot = 1.0f;  // 最大
	const float min_delta_rot = 0.1f;  // 最小

	// レーザーの拡大率
	constexpr float max_scale = 20.0f;	// 最大
	constexpr float min_scale = 10.0f;	// 最小
}

// コンストラクタ
CubeLaser::CubeLaser(const Vector3& firePos, 
				     const float laserSpeed, 
					 const std::shared_ptr<Player>& pPlayer)
{
	// 初期化
	m_pos     = firePos;
	m_pPlayer = pPlayer;
	m_opacity = 0.0f;	
	m_moveVec = (m_pPlayer->GetPos() - m_pos).Normalized() * laserSpeed;

	// 1フレームに回転する量をランダムで取得
	m_deltaRot.x = MathUtil::ToRadian(MathUtil::GetRandFloat(min_delta_rot, max_delta_rot));
	m_deltaRot.y = MathUtil::ToRadian(MathUtil::GetRandFloat(min_delta_rot, max_delta_rot));
	m_deltaRot.z = MathUtil::ToRadian(MathUtil::GetRandFloat(min_delta_rot, max_delta_rot));

	// 最大拡大率をランダムで取得
	m_maxScale.x = MathUtil::ToRadian(MathUtil::GetRandFloat(min_scale, max_scale));
	m_maxScale.y = MathUtil::ToRadian(MathUtil::GetRandFloat(min_scale, max_scale));
	m_maxScale.z = MathUtil::ToRadian(MathUtil::GetRandFloat(min_scale, max_scale));

	// モデル設定
	m_pModel = std::make_unique<Model>(ModelHandleManager::GetInstance().GetHandle("CubeLaser"));
	m_pModel->SetUseCollision(true);	// 当たり判定を使用する
	m_pModel->SetOpacity(m_opacity);	// 不透明度
	m_pModel->SetRot(m_rot);			// 回転情報
	m_pModel->SetScale(m_scale);		// 拡大率
	m_pModel->SetPos(m_pos);			// 位置情報
	m_pModel->Update();					// 当たり判定の更新
}

// デストラクタ
CubeLaser::~CubeLaser()
{
}

// 更新
void CubeLaser::Update()
{
	// 不透明度を加算
	m_opacity = (std::min)(m_opacity + opacity_speed, max_opacity);

	// 移動
	m_pos.z += m_pPlayer->GetMoveVec().z;
	m_pos   += m_moveVec;

	// 拡大
	m_scale += scale_speed;

	// 拡大率の制限
	m_scale.x = (std::min)(m_scale.x, m_maxScale.x);
	m_scale.y = (std::min)(m_scale.y, m_maxScale.y);
	m_scale.z = (std::min)(m_scale.z, m_maxScale.z);

	// 回転
	m_rot += m_deltaRot;

	// モデル設定
	m_pModel->SetOpacity(m_opacity);	// 不透明度
	m_pModel->SetRot(m_rot);			// 回転情報
	m_pModel->SetScale(m_scale);		// 拡大率
	m_pModel->SetPos(m_pos);			// 位置情報
	m_pModel->Update();					// 当たり判定の更新
}

// 描画
void CubeLaser::Draw()
{
	// モデル描画
	m_pModel->Draw();
}
