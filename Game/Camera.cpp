#include "Camera.h"
#include "../Math/MathUtil.h"
#include <DxLib.h>

namespace
{
	// カメラの視野角
	constexpr float camera_perspective = 90.0f;

	// 描画距離(near, far)
	constexpr float near_distance = 5.0f;
	constexpr float far_distance = 20000.0f;

	// プレイヤーからのカメラまでの距離
	constexpr float camera_distance = 200.0f;

	// ゲームクリア時のカメラの回転速度
	constexpr float camera_rotate_speed = 0.03f;
}

// コンストラクタ
Camera::Camera(UnityGameObject data) :
	m_pos(data.pos),
	m_target(data.rot),
	m_perspective(camera_perspective),
	m_cameraVertical(0.0f),
	m_cameraHorizon(DX_PI_F)
{
	// カメラの設定
	CameraSet();
}

// デストラクタ
Camera::~Camera()
{
}

// 更新
void Camera::Update()
{
}

// ゲームクリア時の更新
void Camera::GameClearUpdate(Vector3 playerPos)
{
}

// ゲームオーバー時の更新
void Camera::GameOverUpdate(Vector3 playerPos)
{
	// カメラの垂直方向の回転
	m_cameraHorizon -= camera_rotate_speed;
	m_cameraHorizon = (std::max)(m_cameraHorizon, MathUtil::ToRadian(30));

	// 基準の長さを垂直方向に回転させたときの水平分の長さ
	float verticalLength = camera_distance * cosf(m_cameraVertical);

	// 高さ
	float horizonLength = camera_distance * sinf(m_cameraVertical);

	// カメラ座標の設定
	// xz座標は水平方向の長さ分進めたところ
	m_pos.x = playerPos.x + verticalLength * sinf(m_cameraHorizon);
	m_pos.z = playerPos.z + verticalLength * cosf(m_cameraHorizon);

	// Ｙ座標は垂直方向分上に
	m_pos.y = playerPos.y + horizonLength;

	// カメラの注視点の設定
	m_target = playerPos;

	// カメラの設定
	CameraSet();
}

// カメラの設定
void Camera::CameraSet()
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

float Camera::GetCameraHorizon() const
{
	return m_cameraHorizon;
}
