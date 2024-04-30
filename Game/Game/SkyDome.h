#pragma once
#include <memory>
#include "../Math/Vector3.h"

// プロトタイプ宣言
class Model;

/// <summary>
/// スカイドームクラス
/// </summary>
class SkyDome
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">位置情報</param>
	SkyDome(const Vector3& pos);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SkyDome();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pos">位置情報</param>
	void Update(const Vector3& pos);

	/// <summary>
	/// 描画s
	/// </summary>
	void Draw();

private:
	// ポインタ
	std::unique_ptr<Model> m_pModel;

	// 位置情報
	Vector3 m_pos;
};