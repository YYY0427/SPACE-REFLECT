#pragma once
#include "../Math/Vector2.h"

/// <summary>
/// UIの基底クラス
/// </summary>
class UIBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	UIBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~UIBase();

	/// <summary>
	/// 更新
	/// 純粋仮想関数
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// 純粋仮想関数
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// UIの格納 
	/// </summary>
	/// <param name="vec">格納するベクトル</param>
	/// <param name="storeSpeed">格納速度</param>
	void Store(const Vector2& vec, const float storeSpeed);

	/// <summary>
	/// UIが有効かどうか 
	/// </summary>
	/// <returns></returns>
	bool IsEnabled() const;

protected:
	// UIの座標
	Vector2 m_pos;

	// UIが有効かどうか
	bool m_isEnabled;
};