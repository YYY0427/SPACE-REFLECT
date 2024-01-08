#pragma once
#include "../Math/Vector2.h"

/// <summary>
/// UIの基底クラス
/// </summary>
class UIBase
{
public:
	// コンストラクタ
	UIBase();

	// デストラクタ
	virtual ~UIBase();

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

	// UIの格納
	void Store(Vector2 vec);

	// UIが有効かどうか
	bool IsEnabled() const;

protected:
	// UIの座標
	Vector2 m_pos;

	// UIが有効かどうか
	bool m_isEnabled;
};