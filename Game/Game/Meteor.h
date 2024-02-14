#pragma once
#include "../Math/Vector3.h"
#include "../Editor/DataReaderFromUnity.h"
#include <memory>

// プロトタイプ宣言
class Model;

// 隕石の種類
enum class MeteorType
{
	SMALL,
	NORMAL
};

/// <summary>
/// 隕石クラス
/// </summary>
class Meteor
{
public:
	// コンストラクタ
	Meteor(MeteorType type,Vector3 playerPos);

	// コンストラクタ
	// このコンストラクタはUnityからのデータを受け取る
	Meteor(UnityGameObject data);

	// デストラクタ
	~Meteor();

	// 更新
	void Update(Vector3 cameraPos);
	void UpdateStart(Vector3 playerPos);

	// 描画
	void Draw();

	// ゲッター
	bool IsEnabled() const;		// 存在フラグの取得
	int GetModelHandle() const;	// モデルのハンドルの取得
	MeteorType GetType() const;	// 隕石の種類の取得

private:
	// ポインタ
	std::unique_ptr<Model> m_pModel;

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

	// 存在フラグ
	bool m_isEnabled;
};