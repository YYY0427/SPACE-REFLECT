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
	// コンストラクタ
	ScreenShaker(std::shared_ptr<Camera> pCamera);

	// デストラクタ
	~ScreenShaker();

	// 更新
	void Update();

	// 描画前に必ず呼ぶ
	void PreDraw();

	// 描画
	void Draw();

	// 画面揺れの開始
	void StartShake(Vector2 quakeSize, int frame);

private:
	// ポインタ
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