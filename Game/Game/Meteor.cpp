#include "Meteor.h"
#include "../Model.h"
#include "../Application.h"
#include "../Math/Vector2.h"
#include "../Math/MathUtil.h"
#include "../ModelHandleManager.h"
#include <DxLib.h>

namespace
{
	// モデルの拡大率
	constexpr float normal_meteor_max_scale = 5.0f;	// 最大拡大率
	constexpr float normal_meteor_min_scale = 1.0f;	// 最小拡大率

	//constexpr float small_meteor_max_scale = 0.1f;	// 最大拡大率
	//constexpr float small_meteor_min_scale = 0.05f;	// 最小拡大率

	constexpr float small_meteor_max_scale = 5.0f;	// 最大拡大率
	constexpr float small_meteor_min_scale = 5.0f;	// 最小拡大率


	// 移動速度
	constexpr float meteor_move_speed = -10.0f;
}

// コンストラクタ
// ランダムに生成、プレイヤーの方向に移動
Meteor::Meteor(MeteorType type, Vector3 playerPos) :
	m_rot({ 0, 0, 0 }),
	m_isEnabled(true),
	m_moveVec({ 0, 0, 0 }),
	m_rotVec({ 0, 0, 0 }),
	m_type(type)
{
	// モデルのインスタンスの作成
	m_pModel = std::make_unique<Model>(ModelHandleManager::GetInstance().GetHandle(ModelType::METEOR));

	// 1フレームに回転する量を0度から〜1度の間から取得
	m_rotVec.x = MathUtil::ToRadian(GetRand(10) * 0.1f);
	m_rotVec.y = MathUtil::ToRadian(GetRand(10) * 0.1f);
	m_rotVec.z = MathUtil::ToRadian(GetRand(10) * 0.1f);

	// 隕石の種類によって処理を変える
	// 小さい隕石
	if (type == MeteorType::SMALL)
	{
		// 隕石の拡大率をランダムに設定
		float scale = GetRand(small_meteor_max_scale - small_meteor_min_scale) + small_meteor_min_scale;
		m_scale = { scale, scale, scale };
	}
	// 通常の隕石
	else if (type == MeteorType::NORMAL)
	{
		// 隕石の拡大率をランダムに設定
		float scale = GetRand(normal_meteor_max_scale - normal_meteor_min_scale) + normal_meteor_min_scale;
		m_scale = { scale, scale, scale };

		// 移動ベクトル
		Vector3 distance = playerPos - m_pos;
		distance.Normalize();
		distance *= meteor_move_speed;
		m_moveVec = distance;
	}

	// 画面内にランダムに生成
	Vector3 screenPos{};
	screenPos.x = GetRand(static_cast<int>(Application::GetInstance().GetWindowSize().width));
	screenPos.y = GetRand(static_cast<int>(Application::GetInstance().GetWindowSize().height));
	screenPos.z = 0.01f;
	m_pos = Vector3::FromDxLibVector3(ConvScreenPosToWorldPos_ZLinear(screenPos.ToDxLibVector3()));
	m_pos.z = playerPos.z + 1000.0f;

	// モデルの設定
	m_pModel->SetPos(m_pos);			// 位置
	m_pModel->SetRot(m_rot);			// 回転
	m_pModel->SetScale(m_scale);		// 拡大率
	m_pModel->Update();					// 更新
}

// コンストラクタ
// 配置データ通りに生成、移動しない
Meteor::Meteor(UnityGameObject data):
	m_pos(data.pos),
	m_rot(data.rot),
	m_scale(data.scale),
	m_isEnabled(true),
	m_moveVec({ 0, 0, 0 }),
	m_rotVec({ 0, 0, 0 }),
	m_type(MeteorType::NORMAL)
{
	// モデルのインスタンスの作成
	m_pModel = std::make_unique<Model>(ModelHandleManager::GetInstance().GetHandle(ModelType::METEOR));

	// 1フレームに回転する量をランダムに取得
	m_rotVec.x = MathUtil::ToRadian(GetRand(3) * 0.1f);
	m_rotVec.y = MathUtil::ToRadian(GetRand(3) * 0.1f);
	m_rotVec.z = MathUtil::ToRadian(GetRand(3) * 0.1f);

	// モデルの設定
	m_pModel->SetPos(m_pos);			// 位置
	m_pModel->SetRot(m_rot);			// 回転
	m_pModel->SetScale(m_scale);		// 拡大率
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
//	m_pos += m_moveVec;

	// 回転
	m_rot += m_rotVec;

	// モデルの設定
	m_pModel->SetPos(m_pos);	// 位置
	m_pModel->SetRot(m_rot);	// 回転
	m_pModel->Update();			// 更新
}

// スタート演出時の更新
void Meteor::UpdateStart(Vector3 playerVec)
{
	// 移動
	m_pos.z += (25.0f - playerVec.z);

	// 回転
	m_rot += m_rotVec;

	// モデルの設定
	m_pModel->SetPos(m_pos);	// 位置
	m_pModel->SetRot(m_rot);	// 回転
	m_pModel->Update();			// 更新
}

// 描画
void Meteor::Draw()
{
	m_pModel->Draw();
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

// 隕石の種類の取得
MeteorType Meteor::GetType() const
{
	return m_type;
}