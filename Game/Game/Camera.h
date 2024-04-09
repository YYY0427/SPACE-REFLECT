#pragma once
#include "../Math/Vector3.h"

/// <summary>
/// カメラクラス
/// </summary>
class Camera
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	Camera(const Vector3& playerPos);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Camera();

	/// <summary>
	/// スタート演出時の更新
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	void UpdateStart(const Vector3& playerPos);		

	/// <summary>
	/// プレイ時の更新
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	/// <param name="playerVec">プレイヤーのベクトル</param>
	void UpdatePlay(const Vector3& playerPos, const Vector3& playerVec);

	/// <summary>
	/// ゲームクリア時の更新
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	/// <returns>カメラ演出が終了したか</returns>
	bool UpdateGameClear(const Vector3& playerPos);	

	/// <summary>
	/// ゲームオーバー時の更新
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	void UpdateGameOver(const Vector3& playerPos);


	//// セッター ////

	/// <summary>
	/// カメラの設定
	/// </summary>
	void SetCamera();		

	/// <summary>
	/// カメラの設定
	/// </summary>
	/// <param name="pos">カメラの位置</param>
	/// <param name="target">カメラの注視点</param>
	void SetCamera(const Vector3& pos, const Vector3& target);


	//// ゲッター ////

	/// <summary>
	/// カメラの座標の取得
	/// </summary>
	/// <returns>カメラの座標</returns>
	const Vector3& GetPos() const;			

	/// <summary>
	/// カメラの注視点の取得
	/// </summary>
	/// <returns>カメラの注視点</returns>
	const Vector3& GetTarget() const;	

	/// <summary>
	/// カメラの水平方向の取得
	/// </summary>
	/// <returns>カメラの水平方向</returns>
	float GetCameraHorizon() const;	

	/// <summary>
	/// スタート演出をしたかフラグの取得
	/// </summary>
	/// <returns>スタート演出をしたかフラグ</returns>
	bool  IsStartAnimation() const;	

	/// <summary>
	/// カメラがX軸の移動範囲制限を超えたか
	/// </summary>
	/// <returns>超えたか</returns>
	bool IsOverMoveRangeX() const;

	/// <summary>
	/// カメラがY軸の移動範囲制限を超えたか
	/// </summary>
	/// <returns>超えたか</returns>
	bool IsOverMoveRangeY() const;

	/// <summary>
	/// カメラの移動速度の取得
	/// </summary>
	/// <returns>カメラの移動速度</returns>
	float GetCameraMoveSpeed() const;

	/// <summary>
	/// カメラをプレイヤーのベクトルに合わせて少し移動する割合 
	/// </summary>
	/// <returns></returns>
	float GetCameraMoveRate() const;

private:
	// カメラの座標
	Vector3 m_pos;

	// カメラの注視点
	Vector3 m_target;

	// 視野角
	float m_perspective;

	// カメラの垂直方向
	float m_cameraVertical;

	// カメラの水平方向
	float m_cameraHorizon;	

	// フレーム
	int m_hermiteFrame;			// エリミネート補完のフレーム
	int m_gameClearLerpFrame;	// ラープ演出のフレーム
	int m_gameOverLerpFrame;	// ラープ演出のフレーム

	// スタート演出をしたかフラグ
	bool m_isStartAnimation;	
};