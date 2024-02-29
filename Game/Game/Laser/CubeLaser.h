#pragma once
#include "LaserBase.h"

// プロトタイプ宣言
class Player;

/// <summary>
/// キューブレーザー
/// </summary>
class CubeLaser : public LaserBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="firePos">発射位置</param>
	/// <param name="laserSpeed">レーザーの速さ</param>
	/// <param name="pPlayer">プレイヤーのポインタ</param>
	CubeLaser(const Vector3& firePos, 
			  const float laserSpeed, 
			  const std::shared_ptr<Player>& pPlayer);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CubeLaser();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override final;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override final;

private:
	// プレイヤーのポインタ
	std::shared_ptr<Player> m_pPlayer;

	// 移動ベクトル
	Vector3 m_moveVec;

	// 最大スケール
	Vector3 m_maxScale;

	// 終了時のスケール
	Vector3 m_endScale;

	// 1フレームに回転する量
	Vector3 m_deltaRot;	
};