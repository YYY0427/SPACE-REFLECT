#pragma once
#include "../Math/Vector3.h"
#include "../Editor/DataReaderFromUnity.h"
#include <memory>

// プロトタイプ宣言
class Model;

/// <summary>
/// 惑星クラス
/// </summary>
class Planet
{
public:
	// コンストラクタ
	Planet(int modelHandle, UnityGameObject data);

	// デストラクタ
	~Planet();

	// 更新
	void UpdateStart(Vector3 playerVec, Vector3 rotVec);
	void UpdatePlay(Vector3 playerVec, Vector3 rotVec);
	void Update(Vector3 rotVec);

	// 描画
	void Draw();

	// ゲッター
	Vector3 GetPos() const;

private:
	// ポインタ
	std::unique_ptr<Model> m_pModel;

	// 位置情報
	Vector3 m_pos;

	// 回転情報
	Vector3 m_rot;

	// 拡大率
	Vector3 m_scale;
};