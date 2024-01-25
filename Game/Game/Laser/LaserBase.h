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

	// レーザーを止める
	virtual void Stop(Vector3 pos) {};

	// 反射された状態から元に戻す
	virtual void UndoReflect() {};

	// レーザーを消す
	void Delete();

	// ゲッター
	int GetModelHandle() const;		// モデルハンドル
	Vector3 GetPos() const;			// 位置情報
	virtual Vector3 GetDirection() const;	// 方向ベクトル
	bool IsEnabled() const;	// 存在フラグ
	bool IsReflect() const;	// 反射フラグ

protected:
	// ポインタ
	std::shared_ptr<Model> m_pModel;

	// 位置情報
	Vector3 m_pos;

	// 回転情報
	Vector3 m_rot;

	// 拡大率
	Vector3 m_scale;

	// 速度
	float m_speed;		

	// 不透明度
	float m_opacity;	

	// 存在フラグ
	bool m_isEnabled;

	// 反射フラグ
	bool m_isReflect;
};

