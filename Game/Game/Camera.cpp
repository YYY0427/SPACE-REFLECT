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
	constexpr float camera_perspective = 90.0f;

	// 描画距離(near, far)
	constexpr float near_distance = 50.0f;
	constexpr float far_distance = 25000.0f;

	// プレイヤーからのカメラまでの距離
	constexpr float camera_distance = 400.0f;

	// ゲームクリア時のカメラの回転速度
	constexpr float camera_rotate_speed = 0.03f;

	// カメラの移動速度
	constexpr float camera_move_speed = 3.0f;
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

	// 風エフェクトの再生
	Effekseer3DEffectManager::GetInstance().PlayEffectLoop(
		m_windEffectHandle,
		EffectID::wind,
		m_pos,
		{ 100.0f, 100.0f, 100.0f },
		1.0f,
		{ 0.0f, -DX_PI_F / 2, 0.0f });

	// カメラの設定
	SetCamera();
}

// デストラクタ
Camera::~Camera()
{
}

// 更新
void Camera::Update(Vector3 playerPos)
{
#if false
	// TODO : カメラがターゲットぱっと切り替わっちゃうので、スムーズに切り替わるようにする
	// 特定の角度以上なら補完を入れる
	
	// プレイヤーの位置方向に少しカメラの注視点をずらす
	Vector3 tempTarget = m_target;

	m_target.x = playerPos.x * 0.2f;
	m_target.y = playerPos.y * 0.2f;

	// 注視点を特定の角度以上には回転させない
	Vector3 centerPos = { 0, 0, playerPos.z };
	Vector3 targetPos = m_target - m_pos;
	float angle = MathUtil::ToDegree(centerPos.Angle(targetPos));
	if (angle >= 15.0f)
	{
		DebugText::Log("15度超えたよ");
		m_target = tempTarget;
	}
#else
	// プレイヤーの位置方向とは逆に少しカメラの注視点をずらす
	m_target.x = -playerPos.x * 0.2f;
	m_target.y = -playerPos.y * 0.2f;
#endif

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

	Effekseer3DEffectManager::GetInstance().SetEffectPos(m_windEffectHandle, {m_pos.x, m_pos.y, m_pos.z - 400.0f });

	// カメラの設定
	SetCamera();
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
		}
	}

	Effekseer3DEffectManager::GetInstance().SetEffectPos(m_windEffectHandle, { m_pos.x, m_pos.y, m_pos.z - 400.0f });

	// カメラの設定
	SetCamera();
}

// ゲームクリア時の更新
void Camera::GameClearUpdate(Vector3 playerPos)
{
}

// ゲームオーバー時の更新
void Camera::GameOverUpdate(Vector3 playerPos)
{
	// カメラの垂直方向の回転
	m_cameraHorizon -= camera_rotate_speed * m_slowValue;
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
bool Camera::GetIsStartAnimation() const
{
	return m_isStartAnimation;
}

// スローの値の設定
void Camera::SetSlowValue(float slowValue)
{
	m_slowValue = slowValue;
}