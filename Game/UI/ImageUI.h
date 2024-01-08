#pragma once
#include "UIBase.h"
#include "../Math/Vector2.h"
#include <string>

/// <summary>
/// UIの画像クラス
/// </summary>
class ImageUI : public UIBase
{
public:
	// コンストラクタ
	ImageUI(std::string filePath);

	// デストラクタ
	~ImageUI();

	// 描画
	void Draw() override final;

private:
	// 画像ハンドル
	int m_imgHandle;

	// 画像の回転率
	float m_rot;

	// 画像の中心座標
	Vector2 m_center;

	// 画像の拡大率
	Vector2 m_scale;

	// 画像の透明度
	float m_alpha;
};