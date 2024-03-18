#include "Meteor.h"
#include "../Model.h"
#include "../Game/Player.h"
#include "../Application.h"
#include "../Math/Vector2.h"
#include "../Math/MathUtil.h"
#include "../ModelHandleManager.h"
#include "../DataType/PlayerParamDataType.h"
#include <DxLib.h>

namespace
{
	// モデルの拡大率
	constexpr float normal_meteor_max_scale = 5.0f;	// 最大拡大率
	constexpr float normal_meteor_min_scale = 1.0f;	// 最小拡大率
	constexpr float small_meteor_max_scale = 10.0f;	// 最大拡大率
	constexpr float small_meteor_min_scale = 5.0f;	// 最小拡大率

	// モデルの回転速度
	constexpr float max_delta_rot = 1.0f;  // 最大
	constexpr float min_delta_rot = 0.0f;  // 最小

	// 移動速度
	constexpr float meteor_move_speed = 10.0f;
	constexpr float small_meteor_move_speed = 2.7f;

	// スクリーン座標からワールド座標に変換に使用するZ座標の線形
	constexpr float z_linear = 1.0f;

	// プレイヤーの座標を0としたときの隕石のZ座標
	constexpr float meteor_z = 5000.0f;
}

// コンストラクタ
// ランダムに生成、プレイヤーの方向に移動
Meteor::Meteor(const MeteorType type, const std::shared_ptr<Player>& pPlayer, const float playerRelativePos) :
	m_rot({ 0, 0, 0 }),
	m_isEnabled(true),
	m_moveVec({ 0, 0, 0 }),
	m_rotVec({ 0, 0, 0 }),
	m_type(type),
	m_opacity(1.0f),
	m_pPlayer(pPlayer)
{
	// モデルのインスタンスの作成
	m_pModel = std::make_unique<Model>(ModelHandleManager::GetInstance().GetHandle("Meteor"));

	// 1フレームに回転する量を0度から～1度の間から取得
	m_rotVec.x = MathUtil::ToRadian(MathUtil::GetRandFloat(min_delta_rot, max_delta_rot));
	m_rotVec.y = MathUtil::ToRadian(MathUtil::GetRandFloat(min_delta_rot, max_delta_rot));
	m_rotVec.z = MathUtil::ToRadian(MathUtil::GetRandFloat(min_delta_rot, max_delta_rot));

	// 隕石の種類によって処理を変える
	// 小さい隕石
	if (type == MeteorType::SMALL)
	{
		// 隕石の拡大率をランダムに設定
		float scale = MathUtil::GetRandFloat(small_meteor_min_scale, small_meteor_max_scale);
		m_scale = { scale, scale, scale };
	}
	// 通常の隕石
	else if (type == MeteorType::NORMAL)
	{
		// 隕石の拡大率をランダムに設定
		float scale = MathUtil::GetRandFloat(normal_meteor_min_scale, normal_meteor_max_scale);
		m_scale = { scale, scale, scale };

		// プレイヤーに向かうベクトルを取得
		Vector3 distance = m_pPlayer->GetPos() - m_pos;
		distance.Normalize();
		distance *= meteor_move_speed;
		m_moveVec = distance;
	}

	m_pos.x = m_pos.y = 0.0f;
	m_pos.z = m_pPlayer->GetPos().z + playerRelativePos;

	// 画面内にランダムに生成
	Vector3 screenPos{};
	screenPos.x = MathUtil::GetRandFloat(0.0f, Application::GetInstance().GetWindowSize().width);
	screenPos.y = MathUtil::GetRandFloat(0.0f, Application::GetInstance().GetWindowSize().height);
	screenPos.z = z_linear;
	Vector3 targetPos = Vector3::FromDxLibVector3(ConvScreenPosToWorldPos_ZLinear(screenPos.ToDxLibVector3()));

	// ベクトルの作成
	m_moveVec = (targetPos - m_pos).Normalized() * small_meteor_move_speed;

	// モデルの設定
	m_pModel->SetOpacity(m_opacity);	// 透明度
	m_pModel->SetPos(m_pos);			// 位置
	m_pModel->SetRot(m_rot);			// 回転
	m_pModel->SetScale(m_scale);		// 拡大率
	m_pModel->Update();					// 更新
}

// コンストラクタ
// 配置データ通りに生成、移動しない
Meteor::Meteor(const UnityGameObject& data, const std::shared_ptr<Player>& pPlayer):
	m_pos(data.pos),
	m_rot(data.rot),
	m_scale(data.scale),
	m_isEnabled(true),
	m_moveVec({ 0, 0, 0 }),
	m_rotVec({ 0, 0, 0 }),
	m_type(MeteorType::NORMAL),
	m_opacity(1.0f),
	m_pPlayer(pPlayer)
{
	// モデルのインスタンスの作成
	m_pModel = std::make_unique<Model>(ModelHandleManager::GetInstance().GetHandle("Meteor"));

	// 1フレームに回転する量をランダムに取得
	m_rotVec.x = MathUtil::ToRadian(MathUtil::GetRandFloat(min_delta_rot, max_delta_rot));
	m_rotVec.y = MathUtil::ToRadian(MathUtil::GetRandFloat(min_delta_rot, max_delta_rot));
	m_rotVec.z = MathUtil::ToRadian(MathUtil::GetRandFloat(min_delta_rot, max_delta_rot));

	// モデルの設定
	m_pModel->SetOpacity(m_opacity);	// 透明度
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
void Meteor::Update(const Vector3& cameraPos)
{
	// カメラより後ろに行ったら
	if (cameraPos.z > m_pos.z)
	{
		// 消す
		m_isEnabled = false;
	}

	// 移動
	m_pos += m_moveVec;

	// 回転
	m_rot += m_rotVec;

	// モデルの設定
	m_pModel->SetOpacity(m_opacity);	// 透明度
	m_pModel->SetPos(m_pos);			// 位置
	m_pModel->SetRot(m_rot);			// 回転
	m_pModel->Update();					// 更新
}

// スタート演出時の更新
void Meteor::UpdateStart()
{
	// 移動
	m_pos.z += (m_pPlayer->GetMoveVec().z - 
				m_pPlayer->GetParameter(DataType::PlayerParamType::MOVE_SPEED_Z));

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