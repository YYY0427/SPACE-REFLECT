#pragma once
#include "UIBase.h"
#include "ImageUI.h"
#include <memory>

/// <summary>
/// プレイヤーのステータスの背景クラス
/// </summary>
class StatusBack : public UIBase
{
public:
	// コンストラクタ
	StatusBack();

	// デストラクタ
	~StatusBack();

	// 更新
	void Update() override final;
	void UpdateStartAnimation();

	// 描画
	void Draw() override final;

	// ゲッター
	bool GetIsStartAnimation() const;

private:
	// ポインタ
	std::unique_ptr<ImageUI> m_pImageUI;

	// スタート演出をしたかフラグ
	bool m_isStartAnimation;
};

