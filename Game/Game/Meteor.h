#pragma once
#include "../Math/Vector3.h"
#include "../Editor/DataReaderFromUnity.h"
#include <memory>

// プロトタイプ宣言
class Model;

/// <summary>
/// 隕石クラス
/// </summary>
class Meteor
{
public:
	// コンストラクタ
	Meteor(Vector3 playerPos);

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

	// スローの値の設定
	void SetSlowValue(float slowValue);

	// 存在フラグの取得
	bool IsEnabled() const;

	// モデルハンドルの取得
	int GetModelHandle() const;

private:
	// ポインタ
	std::unique_ptr<Model> m_pModel;

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

	// スローの値
	float m_slowValue;

	// 存在フラグ
	bool m_isEnabled;
};