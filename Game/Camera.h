#pragma once
#include "../Math/Vector3.h"
#include "../Editor/DataReaderFromUnity.h"

/// <summary>
/// カメラクラス
/// </summary>
class Camera
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="data">配置データ</param>
	Camera(UnityGameObject data);

	// デストラクタ
	~Camera();

	// 更新
	void Update();								// 通常時の更新
	void GameClearUpdate(Vector3 playerPos);	// ゲームクリア時の更新
	void GameOverUpdate(Vector3 playerPos);		// ゲームオーバー時の更新

	// カメラの設定	
	void CameraSet();		

	// カメラの座標を取得
	Vector3 GetPos() const;

	// カメラの水平方向を取得
	float GetCameraHorizon() const;

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
};