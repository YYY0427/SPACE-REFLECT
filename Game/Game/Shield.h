#pragma once
#include <memory>
#include <DxLib.h>
#include <array>
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"

// プロトタイプ宣言
class Player;
class Image3D;
class Gauge;

/// <summary>
/// シールドクラス
/// </summary>
class Shield
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	Shield(Player& player);

	// デストラクタ
	~Shield();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 位置の取得
	Vector3 GetPos() const;

	// シールドを使用しているか
	bool IsShield() const;

	// シールドの頂点情報の取得
	std::array<VERTEX3D, 6> GetVertex() const;

private:
	// ポインタ
	std::shared_ptr<Image3D> m_pImage;
	std::shared_ptr<Gauge> m_pEnergyGage;

	// プレイヤーの参照
	Player& m_player;

	// シールドのエフェクトハンドル
	int m_effectHandle;

	// エネルギーゲージ
	int m_enerugyGage;

	// シールドの位置
	Vector3 m_pos;

	// シールドの回転
	Vector3 m_rot;

	// シールドの拡大率
	Vector2 m_scale;

	// プレイヤーからの相対位置
	Vector3 m_relativePosToPlayer;

	// 入力されたか
	bool m_isInput;

	// sinカーブのフレーム
	int m_sinFrame;

	// アルファ値
	int m_alpha;
};