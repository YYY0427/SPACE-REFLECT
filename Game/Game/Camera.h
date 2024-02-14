#pragma once
#include "../Math/Vector3.h"

/// <summary>
/// カメラクラス
/// </summary>
class Camera
{
public:
	// コンストラクタ
	Camera(Vector3 pos, Vector3 target);
	Camera(Vector3 playerPos);

	// デストラクタ
	~Camera();

	// 更新
	void Update(Vector3 pos, Vector3 target);	// 通常時の更新
	void UpdatePlay(Vector3 playerPos, Vector3 playerVec);			// プレイ時の更新
	void UpdateStart(Vector3 playerPos);		// スタート演出時の更新
	bool UpdateGameClear(Vector3 playerPos);	// ゲームクリア時の更新
	void UpdateGameOver(Vector3 playerPos);		// ゲームオーバー時の更新

	// カメラの設定	
	void SetCamera();		

	// ゲッター
	Vector3 GetPos() const;			// カメラの座標の取得
	Vector3 GetTarget() const;		// カメラの注視点の取得
	float GetCameraHorizon() const;	// カメラの水平方向の取得
	bool IsStartAnimation() const;	// スタート演出をしたかフラグの取得

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

	// エルミート曲線の値
	float m_hermiteValue;		

	// ラープの値
	float m_lerpValue;

	// フラグ
	bool m_isStartAnimation;	// スタート演出をしたか
};