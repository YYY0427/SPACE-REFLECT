#pragma once
#include <memory>
#include "../../Math/Vector3.h"

// プロトタイプ宣言
class Model;

/// <summary>
/// レーザーの基底クラス
/// </summary>
class LaserBase
{
public:
	// コンストラクタ
	LaserBase();
	
	// デストラクタ
	virtual ~LaserBase();
	
	// 更新
	virtual void Update() = 0;
	
	// 描画
	virtual void Draw() = 0;

	// ゲッター
	bool IsEnabled() const;	// 存在フラグ
	bool IsReflect() const;	// 反射フラグ

protected:
	// ポインタ
	std::shared_ptr<Model> m_pModel;

	// 位置情報
	Vector3 m_pos;

	// 回転情報
	Vector3 m_rot;

	// 移動ベクトル
	Vector3 m_moveVec;

	// 拡大率
	Vector3 m_scale;

	// 不透明度
	float m_opacity;	

	// 存在フラグ
	bool m_isEnabled;

	// 反射フラグ
	bool m_isReflect;
};

