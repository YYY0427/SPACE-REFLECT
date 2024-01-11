#include "Meteor.h"
#include "../Model.h"
#include "../Application.h"
#include "../Math/Vector2.h"
#include "../Math/MathUtil.h"
#include <DxLib.h>

namespace
{
	// モデルの拡大率
	constexpr int meteor_max_scale = 500;	// 最大拡大率
	constexpr int meteor_min_scale = 100;	// 最小拡大率

	// 移動速度
	constexpr float meteor_move_speed = 100.0f;
}

// コンストラクタ
// ランダムに生成、プレイヤーの方向に移動
Meteor::Meteor(int modelHandle, Vector3 playerPos) :
	m_rot({ 0, 0, 0 }),
	m_slowValue(1.0f),
	m_isEnabled(true)
{
	// モデルのインスタンスの作成
	m_pModel = std::make_unique<Model>(modelHandle);

	// 画面内にランダムに生成
	Vector3 screenPos{};
	screenPos.x = GetRand(static_cast<int>(Application::GetInstance().GetWindowSize().width));
	screenPos.y = GetRand(static_cast<int>(Application::GetInstance().GetWindowSize().height));
	screenPos.z = 1.0f;
	m_pos = Vector3::FromDxLibVector3(ConvScreenPosToWorldPos(screenPos.ToDxLibVector3()));

	// 隕石の拡大率をランダムに設定
	float scale = GetRand(meteor_max_scale - meteor_min_scale) + meteor_min_scale;
	m_scale = { scale, scale, scale };

	// 1フレームに回転する量を0度から～1度の間から取得
	m_rotVec.x = MathUtil::ToRadian(GetRand(10) * 0.1f);
	m_rotVec.y = MathUtil::ToRadian(GetRand(10) * 0.1f);
	m_rotVec.z = MathUtil::ToRadian(GetRand(10) * 0.1f);
	
	// 移動ベクトル
	Vector3 distance = playerPos - m_pos;
	distance.Normalize();
	distance *= meteor_move_speed;
	m_moveVec = distance;

	// モデルの設定
	m_pModel->SetPos(m_pos);			// 位置
	m_pModel->SetRot(m_rot);			// 回転
	m_pModel->SetScale(m_scale);		// 拡大率
	m_pModel->SetUseCollision(true);	// 当たり判定を使用する
	m_pModel->Update();					// 更新
}

// コンストラクタ
// 配置データ通りに生成、移動しない
Meteor::Meteor(int modelHandle, UnityGameObject data):
	m_pos(data.pos),
	m_rot(data.rot),
	m_scale(data.scale),
	m_slowValue(1.0f),
	m_isEnabled(true),
	m_moveVec({ 0, 0, 0 }),
	m_rotVec({ 0, 0, 0 })
{
	// モデルの設定
	m_pModel->SetPos(m_pos);			// 位置
	m_pModel->SetRot(m_rot);			// 回転
	m_pModel->SetScale(m_scale);		// 拡大率
	m_pModel->SetUseCollision(true);	// 当たり判定を使用する
	m_pModel->Update();					// 更新
}

// デストラクタ
Meteor::~Meteor()
{
}

// 更新
void Meteor::Update(Vector3 cameraPos)
{
	// カメラより後ろに行ったら
	if (cameraPos.z > m_pos.z)
	{
		// 消す
		m_isEnabled = false;
	}

	// 移動
	m_pos += (m_moveVec * m_slowValue);

	// 回転
	m_rot += (m_rotVec * m_slowValue);

	// モデルの位置を設定
	m_pModel->SetPos(m_pos);
	
	// モデルの回転を設定
	m_pModel->SetRot(m_rot);

	// モデルの更新
	m_pModel->Update();
}

// 描画
void Meteor::Draw()
{
	// ライティングを使用しない
	SetUseLighting(false);
	m_pModel->Draw();
	SetUseLighting(true);
}

// スローの値の設定
void Meteor::SetSlowValue(float slowValue)
{
	m_slowValue = slowValue;
}

// 存在フラグの取得
bool Meteor::IsEnabled() const
{
	return m_isEnabled;
}

// モデルのハンドルの取得
int Meteor::GetModelHandle() const
{
	return m_pModel->GetModelHandle();
}
