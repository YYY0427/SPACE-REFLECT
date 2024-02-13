#include "Camera.h"
#include "../Math/MathUtil.h"
#include "../Effect/Effekseer3DEffectManager.h"
#include "../MyDebug/DebugText.h"
#include <DxLib.h>
#include <algorithm>
#include <cassert>

namespace
{
	// カメラの視野角
	constexpr float camera_perspective = 70.0f;

	// 描画距離(near, far)
	constexpr float near_distance = 5.0f;
	constexpr float far_distance = 50000.0f;

	// プレイヤーからのカメラまでの距離
	constexpr float camera_distance = 400.0f;

	// ゲームクリア時のカメラの回転速度
	constexpr float camera_rotate_speed = 0.03f;

	// カメラの移動速度
	constexpr float camera_move_speed = 7.5f;

	// カメラの移動制限範囲
	const Vector2 camera_move_limit = { 150.0f, 100.0f };
}

// コンストラクタ
Camera::Camera(Vector3 pos, Vector3 target) :
	m_pos(pos),
	m_target(target),
	m_perspective(camera_perspective),
	m_cameraVertical(0.0f),
	m_cameraHorizon(DX_PI_F),
	m_isStartAnimation(false),
	m_hermiteValue(0.0f),
	m_lerpValue(0.0f),
	m_slowValue(1.0f)
{
}

// コンストラクタ
Camera::Camera(Vector3 playerPos) :
	m_pos({ playerPos.x + 500, playerPos.y + 100, playerPos.z + 2000}),
	m_target(playerPos),
	m_perspective(camera_perspective),
	m_cameraVertical(0.0f),
	m_cameraHorizon(DX_PI_F),
	m_isStartAnimation(false),
	m_hermiteValue(0.0f),
	m_lerpValue(0.0f),
	m_slowValue(1.0f)
{
	// カメラの設定
	SetCamera();
}

// デストラクタ
Camera::~Camera()
{
}

// 更新
void Camera::Update(Vector3 pos, Vector3 target)
{
	// カメラの座標と注視点の更新
	m_pos = pos;
	m_target = target;

	// カメラの設定
	SetCamera();
}

// プレイ時の更新
void Camera::UpdatePlay(Vector3 playerPos, Vector3 playerVec)
{
	// カメラをプレイヤーのベクトルに合わせて少し移動
	if (std::fabs(m_pos.x) <= camera_move_limit.x) 
	{
		m_pos.x += -playerVec.x * 0.3f;
	}
	if (std::fabs(m_pos.y) <= camera_move_limit.y) 
	{
		m_pos.y += -playerVec.y * 0.3f;
	}

	// カメラの移動制限
	m_pos.x = std::clamp(m_pos.x, -camera_move_limit.x, camera_move_limit.x);
	m_pos.y = std::clamp(m_pos.y, -camera_move_limit.y, camera_move_limit.y);

	// カメラとプレイヤーの差分
	Vector3 direction = playerPos - m_pos;

	// Y軸、X軸を無視
	direction.y = 0.0f;
	direction.x = 0.0f;

	// カメラとプレイヤーの距離が一定以上離れていたら
	if (direction.Length() > camera_distance)
	{
		// カメラと注視点の移動
		direction.Normalize();
		m_pos += (direction * camera_move_speed * m_slowValue);
		m_target += (direction * camera_move_speed * m_slowValue);
	}

	// カメラの設定
	SetCamera();

	DebugText::Log("CameraPos", { m_pos.x, m_pos.y, m_pos.z });
	DebugText::Log("CameraTarget", { m_target.x, m_target.y, m_target.z });
}

// スタート演出時の更新
void Camera::UpdateStart(Vector3 playerPos)
{
	// 注視点の更新
	m_target = playerPos;

	// プレイヤーの位置がカメラの位置より前に来たら
	if (playerPos.z > m_pos.z + 200)
	{
		// エルミート曲線の値を増やす
		m_hermiteValue += 0.005f * m_slowValue;

		// カメラの位置をエルミート曲線で移動させる
		m_pos = Vector3::Hermite
			(
				m_pos,
				{ -1.0f, 0.0f, -1.0f },
				{ playerPos.x, playerPos.y + 50, playerPos.z - camera_distance },
				{ 1.0f, 0.0f, 1.0f },
				m_hermiteValue
			);

		// エルミート曲線の値が1.0fを超えたら
		if (m_hermiteValue >= 1.0f)
		{
			// スタート演出をしたかフラグを立てる
			m_isStartAnimation = true;
			m_hermiteValue = 0.0f;
		}
	}

	// カメラの設定
	SetCamera();
}

// ゲームクリア時の更新
bool Camera::UpdateGameClear(Vector3 playerPos)
{
	// カメラのターゲットをプレイヤーの位置に徐々に変更
	m_lerpValue += 0.001f * m_slowValue;
	m_target = Vector3::Lerp(m_target, playerPos, m_lerpValue);

	// カメラの位置をエルミート曲線で移動させる
	m_hermiteValue += 0.002f * m_slowValue;
	Vector3 targetPos = { playerPos.x + 200.0f, playerPos.y + 50, playerPos.z + 200.0f };
	m_pos = Vector3::Hermite
	(
		m_pos,
		{ 1.0f, 0.0f, 1.0f },
		targetPos, 
		{ -1.0f, 0.0f, -1.0f },
		m_hermiteValue
	);

	// 目標位置に到達したら
	if (targetPos.Distance(m_pos) < 50.0f &&  m_target.Distance(playerPos) < 100.0f)
	{
		return true;
	}

	// カメラの設定
	SetCamera();

	return false;
}

// ゲームオーバー時の更新
void Camera::UpdateGameOver(Vector3 playerPos)
{
	// カメラのターゲットをプレイヤーの位置に徐々に変更
	m_lerpValue += 0.001f * m_slowValue;
	m_target = Vector3::Lerp(m_target, playerPos, m_lerpValue);

	// カメラの設定
	SetCamera();
}

// カメラの設定
void Camera::SetCamera()
{
	// カメラからどれだけ離れたところ( Near )から、 どこまで( Far )のものを描画するかを設定
	SetCameraNearFar(near_distance, far_distance);

	// カメラの視野角を設定(ラジアン)
	SetupCamera_Perspective(MathUtil::ToRadian(m_perspective));

	// カメラの位置、どこを見ているかを設定する
	SetCameraPositionAndTargetAndUpVec(
		m_pos.ToDxLibVector3(),
		m_target.ToDxLibVector3(),
		VGet(0, 1, 0));
}

// カメラの位置の取得
Vector3 Camera::GetPos() const
{
	return m_pos;
}

// カメラの水平方向を取得
float Camera::GetCameraHorizon() const
{
	return m_cameraHorizon;
}

// スタート演出をしたかフラグの取得
bool Camera::IsStartAnimation() const
{
	return m_isStartAnimation;
}

// スローの値の設定
void Camera::SetSlowValue(float slowValue)
{
	m_slowValue = slowValue;
}