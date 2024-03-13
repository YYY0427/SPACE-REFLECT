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
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="modelHandle">モデルハンドル</param>
	/// <param name="data">配置データ</param>
	Planet(const int modelHandle, const UnityGameObject& data);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Planet();

	/// <summary>
	/// ゲームプレイ時の更新
	/// </summary>
	/// <param name="playerVec">プレイヤーの移動ベクトル</param>
	/// <param name="rotVec">モデルの回転ベクトル</param>
	void Update(const Vector3& playerVec, const Vector3& rotVec);

	/// <summary>
	/// ステージ選択時の更新
	/// </summary>
	/// <param name="rotVec">モデルの回転ベクトル</param>
	void UpdateStageSelect(const Vector3& rotVec);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 位置情報の取得
	/// </summary>
	/// <returns>位置情報</returns>
	const Vector3& GetPos() const;

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