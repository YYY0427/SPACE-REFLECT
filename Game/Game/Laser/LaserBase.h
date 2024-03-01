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
	/// <summary>
	/// コンストラクタ
	/// </summary>
	LaserBase();
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~LaserBase();
	
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;
	
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// レーザーを止める
	/// </summary>
	/// <param name="pos">レーザーを止める位置</param>
	virtual void Stop(const Vector3& pos) {};

	/// <summary>
	/// 反射
	/// </summary>
	/// <param name="pos">反射する位置</param>
	virtual void Reflect(const Vector3& pos) {};

	/// <summary>
	/// 反射を元に戻す
	/// </summary>
	virtual void UndoReflect() {};

	/// <summary>
	/// レーザーを消す
	/// </summary>
	void Delete();


	//// ゲッター ////

	/// <summary>
	/// モデルハンドルを取得
	/// </summary>
	/// <returns>モデルハンドル</returns>
	int GetModelHandle() const;			

	/// <summary>
	/// 反射レーザーのキーを取得
	/// </summary>
	/// <returns>反射レーザーのキー</returns>
	int GetReflectLaserKey() const;			

	/// <summary>
	/// 位置情報を取得
	/// </summary>
	/// <returns>位置情報</returns>
	const Vector3& GetPos() const;	

	/// <summary>
	/// レーザーの終端位置を取得
	/// </summary>
	/// <returns>レーザーの終端位置</returns>
	const Vector3& GetEndPos() const;		

	/// <summary>
	/// 方向ベクトルを取得
	/// </summary>
	/// <returns>方向ベクトル</returns>
	virtual Vector3 GetDirection() const;

	/// <summary>
	/// 存在フラグの取得
	/// </summary>
	/// <returns>true : 存在、false : 存在していない</returns>
	bool IsEnabled() const;

	/// <summary>
	/// 反射フラグの取得
	/// </summary>
	/// <returns>true : 反射、false : 反射していない</returns>
	bool IsReflect() const;	


	//// セッター ////

	/// <summary>
	/// 位置情報を設定
	/// </summary>
	/// <param name="pos">位置情報</param>
	void SetPos(const Vector3& pos);

	/// <summary>
	/// 反射レーザーのキーを設定
	/// </summary>
	/// <param name="key">反射レーザーのキー</param>
	void SetReflectLaserKey(const int key);

protected:
	// ポインタ
	std::shared_ptr<Model> m_pModel;

	// 位置情報
	Vector3 m_pos;

	// 開始位置
	Vector3 m_startPos;

	// 終了位置
	Vector3 m_endPos;

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

	// 反射レーザーのキー
	int m_reflectLaserKey;	
};

