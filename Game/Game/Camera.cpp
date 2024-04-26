#include "Camera.h"
#include "../Math/MathUtil.h"
#include "../Effect/Effekseer3DEffectManager.h"
#include "../MyDebug/DebugText.h"
#include "../Util/Easing.h"
#include <DxLib.h>
#include <algorithm>
#include <cassert>

namespace
{
	// カメラの視野角
	constexpr float camera_perspective = 70.0f;

	// 描画距離(near, far)
	constexpr float near_distance = 5.0f;
	constexpr float far_distance  = 50000.0f;

	// プレイヤーからのカメラまでの距離
	constexpr float camera_distance = 400.0f;

	// カメラの移動速度
	constexpr float camera_move_speed = 7.5f;

	// カメラの移動制限範囲
	const Vector2 camera_move_limit = { 150.0f * 3.0f, 100.0f * 3.0f };

	// カメラをプレイヤーのベクトルに合わせて少し移動する割合
	constexpr float camera_move_ratio = 0.7f;

	// ゲームスタートの演出を開始するプレイヤーの位置
	constexpr float camera_start_animation_start_pos_z = 200.0f;

	// ゲームスタート時
	constexpr int camera_start_animation_frame = 200;								// カメラの演出のフレーム数
	const Vector3 camera_init_relative_player_pos        = { 500.0f, 100.0f, 2000.0f };	// カメラの初期位置
	const Vector3 camera_start_animation_end_pos         = { 0.0f, 50.0f, -400.0f };// カメラの移動先の位置	
	const Vector3 camera_start_animation_start_direction = { -1.0f, 0.0f, -1.0f };	// カメラの移動先の開始の方向
	const Vector3 camera_start_animation_end_direction   = { 1.0f, 0.0f, 1.0f };	// カメラの移動先の終了の方向

	// ゲームクリア時
	constexpr int camera_game_clear_target_frame = 300;		// カメラのターゲットをプレイヤーに向けるフレーム数
	constexpr int camera_game_clear_move_frame   = 300;		// カメラの位置を移動させるフレーム数
	const Vector3 camera_game_clear_end_pos = { 200.0f, 50.0f, 200.0f };	// カメラの移動先の位置
	const Vector3 camera_game_clear_start_direction = { 1.0f, 0.0f, 1.0f };	// カメラの移動先の開始の方向
	const Vector3 camera_game_clear_end_direction = { -1.0f, 0.0f, -1.0f };	// カメラの移動先の終了の方向

	// ゲームオーバー時のカメラのターゲットをプレイヤーに向けるフレーム数
	constexpr int camera_game_over_target_frame = 300;

	// 目的地に到達したかどうかの判定
	// 判定の閾値（適切な値に調整する必要）
	constexpr float camera_pos_distance_threshold    = 10.0f;	// カメラの位置
	constexpr float camera_target_distance_threshold = 10.0f;	// カメラの注視点
}

// コンストラクタ
Camera::Camera() :
	m_pos({ 0.0f, 0.0f, 0.0f }),
	m_target({ 0.0f, 0.0f, 0.0f }),
	m_perspective(camera_perspective),
	m_cameraVertical(0.0f),
	m_cameraHorizon(DX_PI_F),
	m_isStartAnimation(false),
	m_gameClearLerpFrame(0),
	m_hermiteFrame(0),
	m_gameOverLerpFrame(0)
{
	// カメラの設定
	SetCamera();
}

// コンストラクタ
Camera::Camera(const Vector3& playerPos) :
	m_pos(playerPos + camera_init_relative_player_pos),
	m_target(playerPos),
	m_perspective(camera_perspective),
	m_cameraVertical(0.0f),
	m_cameraHorizon(DX_PI_F),
	m_isStartAnimation(false),
	m_gameClearLerpFrame(0),
	m_hermiteFrame(0),
	m_gameOverLerpFrame(0)
{
	// カメラの設定
	SetCamera();
}

// デストラクタ
Camera::~Camera()
{
}

// プレイ時の更新
void Camera::UpdatePlay(const Vector3& playerPos, const Vector3& playerVec)
{
#if true
	// カメラをプレイヤーのベクトルに合わせて少し移動
	m_pos.x += playerVec.x * camera_move_ratio;
	m_pos.y += playerVec.y * camera_move_ratio;

	// カメラのターゲットを常に正面に向ける
	m_target.x = m_pos.x;
	m_target.y = m_pos.y - camera_start_animation_end_pos.y;

	// カメラのターゲットの制限
	m_target.x = std::clamp(m_target.x, -camera_move_limit.x, camera_move_limit.x);
	m_target.y = std::clamp(m_target.y, 
							-camera_move_limit.y - camera_start_animation_end_pos.y, 
							 camera_move_limit.y - camera_start_animation_end_pos.y);

	// カメラの移動制限
	m_pos.x = std::clamp(m_pos.x, -camera_move_limit.x, camera_move_limit.x);
	m_pos.y = std::clamp(m_pos.y, -camera_move_limit.y, camera_move_limit.y);
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
		m_pos    += (direction * camera_move_speed);
		m_target += (direction * camera_move_speed);
	}

	// カメラの設定
	SetCamera();

	// デバッグログの追加
	DebugText::AddLog("CameraPos",    { m_pos.x, m_pos.y, m_pos.z });
	DebugText::AddLog("CameraTarget", { m_target.x, m_target.y, m_target.z });
}

// スタート演出時の更新
void Camera::UpdateStart(const Vector3& playerPos)
{
	// 注視点の更新
	m_target = playerPos;

	// プレイヤーの位置がカメラの位置より前に来たら
	if (playerPos.z > m_pos.z + camera_start_animation_start_pos_z)
	{
		// エルミート曲線の値を増やす
		m_hermiteFrame     = (std::min)(++m_hermiteFrame, camera_start_animation_frame);
		float hermiteValue = static_cast<float>(m_hermiteFrame) / static_cast<float>(camera_start_animation_frame);

		// カメラの位置をエルミート曲線で移動させる
		m_pos = Vector3::Hermite
			(
				m_pos,
				camera_start_animation_start_direction,
				playerPos + camera_start_animation_end_pos,
				camera_start_animation_end_direction,
				hermiteValue
			);

		// エルミート曲線の値が1.0fを超えたら
		if (m_hermiteFrame >= camera_start_animation_frame)
		{
			m_isStartAnimation = true;
			m_hermiteFrame = 0;
		}
	}

	// カメラの設定
	SetCamera();
}

// ゲームクリア時の更新
bool Camera::UpdateGameClear(const Vector3& playerPos)
{
	// フレーム数を増やす
	m_gameClearLerpFrame = (std::min)(++m_gameClearLerpFrame, camera_game_clear_target_frame);

	// フレーム数からラープの値を算出
	float lerp  = static_cast<float>(m_gameClearLerpFrame) / static_cast<float>(camera_game_clear_target_frame);

	// カメラのターゲットをプレイヤーの位置に徐々に変更
	m_target = Vector3::Lerp(m_target, playerPos, lerp);

	// フレーム数を増やす
	m_hermiteFrame = (std::min)(++m_hermiteFrame, camera_game_clear_move_frame);

	// エルミート曲線の値を算出
	float hermiteValue = static_cast<float>(m_hermiteFrame) / static_cast<float>(camera_game_clear_move_frame);

	// カメラの移動先の位置
	Vector3 endPos = playerPos + camera_game_clear_end_pos;

	// カメラの位置をエルミート曲線で移動
	m_pos = Vector3::Hermite
	(
		m_pos,
		camera_game_clear_start_direction,
		endPos,
		camera_game_clear_end_direction,
		hermiteValue
	);

	// カメラの設定
	SetCamera();

	// 目標位置に到達したら
	if (endPos.Distance(m_pos) < camera_pos_distance_threshold && 
		m_target.Distance(playerPos) < camera_target_distance_threshold)
	{
		// カメラの演出が終了
		return true;
	}

	// カメラの演出が終了していない
	return false;
}

// ゲームオーバー時の更新
void Camera::UpdateGameOver(const Vector3& playerPos)
{
	// カメラのターゲットをプレイヤーの位置に徐々に変更
	m_gameOverLerpFrame = (std::min)(++m_gameOverLerpFrame, camera_game_over_target_frame);
	float lerp = static_cast<float>(m_gameOverLerpFrame) / static_cast<float>(camera_game_over_target_frame);
	m_target = Vector3::Lerp(m_target, playerPos, lerp);

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
	SetCameraPositionAndTarget_UpVecY(
		m_pos.ToDxLibVector3(),
		m_target.ToDxLibVector3());
}

// カメラの設定
void Camera::SetCamera(const Vector3& pos, const Vector3& target)
{
	// カメラの座標と注視点の更新
	m_pos    = pos;
	m_target = target;

	// カメラの設定
	SetCamera();
}

// カメラの位置の取得
const Vector3& Camera::GetPos() const
{
	return m_pos;
}

// カメラの注視点の取得
const Vector3& Camera::GetTarget() const
{
	return m_target;
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

// カメラがX軸の移動範囲制限を超えたか
bool Camera::IsOverMoveRangeX() const
{
	// 移動範囲を超えたか
	if (m_pos.x >= camera_move_limit.x || m_pos.x <= -camera_move_limit.x)
	{
		// 移動範囲を超えている
		return true;
	}

	// 移動範囲を超えていない
	return false;
}

// カメラがY軸の移動範囲制限を超えたか
bool Camera::IsOverMoveRangeY() const
{
	// 移動範囲を超えたか
	if (m_pos.y >= camera_move_limit.y || m_pos.y <= -camera_move_limit.y)
	{
		// 移動範囲を超えている
		return true;
	}

	// 移動範囲を超えていない
	return false;
}

// カメラの移動速度の取得
float Camera::GetCameraMoveSpeed() const
{
	return camera_move_speed;
}

// カメラをプレイヤーのベクトルに合わせて少し移動する割合 
float Camera::GetCameraMoveRate() const
{
	return camera_move_ratio;
}