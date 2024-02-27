#pragma once
#include "../Util/Timer.h"
#include "../Math/Vector2.h"
#include <memory>

// プロトタイプ宣言
class Camera;

/// <summary>
/// 画面を揺らすクラス
/// </summary>
class ScreenShaker
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pCamera">カメラのポインタ</param>
	ScreenShaker(const std::shared_ptr<Camera>& pCamera);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ScreenShaker();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画準備
	/// 描画前に必ず呼ぶ 
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 画面揺れの開始
	/// </summary>
	/// <param name="quakeSize">画面揺れの大きさ</param>
	/// <param name="frame">画面揺れのフレーム</param>
	void StartShake(const Vector2& quakeSize, const int frame);

private:
	// カメラポインタ
	std::shared_ptr<Camera> m_pCamera;

	// 揺れる時間
	Timer<int> m_quakeTimer;	

	// 揺れる強さ
	Vector2 m_quakeSize;

	// PreDrawをしたか
	bool m_isPreDraw;

	// 画面ハンドル
	int m_screenHandle;
};