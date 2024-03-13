#include "Planet.h"
#include "../Math/MathUtil.h"
#include "../Model.h"
#include <DxLib.h>

// コンストラクタ
Planet::Planet(const int modelHandle, const UnityGameObject& data) :
	m_pos(data.pos),
	m_rot(data.rot),
	m_scale(data.scale)
{
	// モデルのインスタンスの作成
	m_pModel = std::make_unique<Model>(modelHandle);

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

// プレイ時の更新
void Planet::Update(const Vector3& playerVec, const Vector3& rotVec)
{
	// プレイヤーと同じZベクトル移動
	m_pos.z += playerVec.z;

	// 回転
	m_rot += rotVec;

	// モデルの設定
	m_pModel->SetPos(m_pos);	// 位置
	m_pModel->SetRot(m_rot);	// 回転
	m_pModel->Update();			// 更新
}

// 更新
void Planet::UpdateStageSelect(const Vector3& rotVec)
{
	// 回転
	m_rot += rotVec;

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
const Vector3& Planet::GetPos() const
{
	return m_pos;
}