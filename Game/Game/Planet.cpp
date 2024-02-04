#include "Planet.h"
#include "../Math/MathUtil.h"
#include "../Model.h"
#include <DxLib.h>

namespace
{
	// 1フレームに回転する量の最大値
	constexpr float max_rot = 0.01f;

	// 1フレームに回転する量の最小値
	constexpr float min_rot = 0.0f;
}

// コンストラクタ
Planet::Planet(int modelHandle, UnityGameObject data) :
	m_pos(data.pos),
	m_rot(data.rot),
	m_scale(data.scale)
{
	// モデルのインスタンスの作成
	m_pModel = std::make_unique<Model>(modelHandle);

	// 1フレームに回転する量を0度から～0.1度の間から取得
	m_rotVec.x = MathUtil::ToRadian(GetRand(10) * (max_rot * 0.1f) + min_rot);
	m_rotVec.y = MathUtil::ToRadian(GetRand(10) * (max_rot * 0.1f) + min_rot);
	m_rotVec.z = MathUtil::ToRadian(GetRand(10) * (max_rot * 0.1f) + min_rot);

	// モデルの設定
	m_pModel->SetPos(m_pos);			// 位置
	m_pModel->SetRot(m_rot);			// 回転
	m_pModel->SetScale(m_scale);		// 拡大率
	m_pModel->Update();					// 更新
}

// デストラクタ
Planet::~Planet()
{
}

// スタート時の更新
void Planet::UpdateStart(Vector3 playerVec)
{
	// プレイヤーと同じベクトル移動
	m_pos += playerVec;

	// 回転
	m_rot += m_rotVec;

	// モデルの設定
	m_pModel->SetPos(m_pos);	// 位置
	m_pModel->SetRot(m_rot);	// 回転
	m_pModel->Update();			// 更新
}

// 更新
void Planet::Update()
{
	// 回転
	m_rot += m_rotVec;

	// モデルの設定
	m_pModel->SetRot(m_rot);	// 回転
	m_pModel->Update();			// 更新
}

// 描画
void Planet::Draw()
{
	m_pModel->Draw();
}

// ゲッター
Vector3 Planet::GetPos() const
{
	return m_pos;
}