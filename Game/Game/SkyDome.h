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
	// コンストラクタ
	SkyDome(Vector3 playerPos);

	// デストラクタ
	~SkyDome();

	// 更新
	void Update(Vector3 playerPos);

	// 描画
	void Draw();

private:
	// ポインタ
	std::unique_ptr<Model> m_pModel;

	// 位置情報
	Vector3 m_pos;
};