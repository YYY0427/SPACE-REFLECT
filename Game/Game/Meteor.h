#pragma once
#include "../Math/Vector3.h"
#include "../Editor/DataReaderFromUnity.h"
#include <memory>

// プロトタイプ宣言
class Model;
class Player;

// 隕石の種類
enum class MeteorType
{
	SMALL,	// 小さい隕石
	NORMAL	// 通常の隕石
};

/// <summary>
/// 隕石クラス
/// </summary>
class Meteor
{
public:
	/// <summary>
	/// コンストラクタ
	/// ランダム生成
	/// </summary>
	/// <param name="type">隕石の種類</param>
	/// <param name="playerPos">プレイヤーの座標</param>
	Meteor(const MeteorType type, const std::shared_ptr<Player>& pPlayer, const float playerRelativePos);

	/// <summary>
	/// コンストラクタ
	/// 配置データで生成
	/// </summary>
	/// <param name="data">配置データ</param>
	Meteor(const UnityGameObject& data, const std::shared_ptr<Player>& pPlayer);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Meteor();

	/// <summary>
	/// ゲームスタート時の更新
	/// </summary>
	void UpdateStart();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="cameraPos">カメラの座標</param>
	void Update(const Vector3& cameraPos);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


	//// ゲッター ////

	/// <summary>
	/// 存在フラグの取得
	/// </summary>
	/// <returns>存在フラグ</returns>
	bool IsEnabled() const;		

	/// <summary>
	/// モデルのハンドルの取得
	/// </summary>
	/// <returns>モデルのハンドル</returns>
	int GetModelHandle() const;

	/// <summary>
	/// 隕石の種類の取得
	/// </summary>
	/// <returns>隕石の種類</returns>
	MeteorType GetType() const;	

private:
	// ポインタ
	std::unique_ptr<Model> m_pModel;
	std::shared_ptr<Player> m_pPlayer;

	// 隕石の種類
	MeteorType m_type;

	// 位置情報
	Vector3 m_pos;

	// 回転角度
	Vector3 m_rot;

	// 拡大率
	Vector3 m_scale;

	// 移動ベクトル
	Vector3 m_moveVec;

	// 回転ベクトル
	Vector3 m_rotVec;

	// 透明度
	float m_opacity;

	// 存在フラグ
	bool m_isEnabled;
};