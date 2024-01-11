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

	// 更新
	void Update() override final {};

	// 描画
	void Draw() override final;

	// セッター
	void SetPos(Vector2 pos);		// 位置
	void SetRot(float rot);			// 回転
	void SetCenter(Vector2 center);	// 中心座標
	void SetScale(Vector2 scale);	// 拡大率
	void SetAlpha(float alpha);		// 透明度

	// ゲッター
	Vector2 GetPos() const;		// 位置
	float GetRot() const;		// 回転
	Vector2 GetCenter() const;	// 中心座標
	Vector2 GetScale() const;	// 拡大率
	float GetAlpha() const;		// 透明度

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