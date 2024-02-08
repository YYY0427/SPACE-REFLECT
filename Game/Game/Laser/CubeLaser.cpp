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
	constexpr float opacity_speed = 0.01f;
}

// コンストラクタ
CubeLaser::CubeLaser(Vector3 firePos, float laserSpeed, std::shared_ptr<Player> pPlayer)
{
	// 初期化
	m_pos = firePos;
	m_pPlayer = pPlayer;
	m_opacity = 0.0f;	
	m_moveVec = (m_pPlayer->GetPos() - m_pos).Normalized() * laserSpeed;

	// 1フレームに回転する量を0度から～1度の間から取得
	m_deltaRot = MathUtil::ToRadian(GetRand(100) / 100.0f);

	// モデル設定
	m_pModel = std::make_unique<Model>(ModelHandleManager::GetInstance().GetHandle(ModelType::CUBE_LASER));
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
	m_opacity = (std::min)(m_opacity + opacity_speed, 1.0f);

	// 移動
	m_pos += m_moveVec;

	// 拡大
	m_scale += scale_speed;

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
