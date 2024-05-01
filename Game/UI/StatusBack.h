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
	/// <summary>
	/// コンストラクタ
	/// </summary>
	StatusBack();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StatusBack();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override final;

	/// <summary>
	/// スタート演出の更新
	/// </summary>
	void UpdateStartAnimation();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override final;

	/// <summary>
	/// スタート演出をしたかフラグを取得
	/// </summary>
	/// <returns>スタート演出をしたか</returns>
	bool IsStartAnimation() const;

private:
	// ポインタ
	std::unique_ptr<ImageUI> m_pImageUI;

	// スタート演出をしたかフラグ
	bool m_isStartAnimation;
};

