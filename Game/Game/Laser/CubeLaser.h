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
	// コンストラクタ
	CubeLaser(Vector3 firePos, float laserSpeed, std::shared_ptr<Player> pPlayer);

	// デストラクタ
	~CubeLaser();

	// 更新
	void Update() override final;

	// 描画
	void Draw() override final;

private:
	// ポインタ
	std::shared_ptr<Player> m_pPlayer;	// プレイヤー

	// 移動ベクトル
	Vector3 m_moveVec;

	// 終了時のスケール
	Vector3 m_endScale;

	// 1フレームに回転する量
	float m_deltaRot;	
};