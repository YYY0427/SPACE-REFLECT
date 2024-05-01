#pragma once
#include "UIBase.h"
#include "../Math/Vector2.h"
#include <string>

/// <summary>
/// UIの画像クラス
/// </summary>
class ImageUI : public UIBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="filePath">画像ファイルのパス</param>
	ImageUI(const std::string& filePath);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ImageUI();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override final {};

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override final;


	//// セッター ////

	/// <summary>
	/// 位置の設定
	/// </summary>
	/// <param name="pos">位置</param>
	void SetPos(const Vector2& pos);	

	/// <summary>
	/// 回転の設定
	/// </summary>
	/// <param name="rot">回転</param>
	void SetRot(const float rot);		

	/// <summary>
	/// 中心座標の設定
	/// </summary>
	/// <param name="center">中心座標</param>
	void SetCenter(const Vector2& center);	

	/// <summary>
	/// 拡大率の設定
	/// </summary>
	/// <param name="scale">拡大率</param>
	void SetScale(const Vector2& scale);	

	/// <summary>
	/// 透明度の設定
	/// </summary>
	/// <param name="alpha">透明度</param>
	void SetAlpha(const float alpha);		


	//// ゲッター ////

	/// <summary>
	/// 位置の取得
	/// </summary>
	/// <returns>位置</returns>
	const Vector2& GetPos() const;		

	/// <summary>
	/// 回転の取得
	/// </summary>
	/// <returns>回転</returns>
	float GetRot() const;		

	/// <summary>
	/// 中心座標の取得
	/// </summary>
	/// <returns>中心座標</returns>
	const Vector2& GetCenter() const;	

	/// <summary>
	/// 拡大率の取得
	/// </summary>
	/// <returns>拡大率</returns>
	const Vector2& GetScale() const;	

	/// <summary>
	/// 透明度の取得
	/// </summary>
	/// <returns>透明度</returns>
	float GetAlpha() const;		

private:
	// 画像ハンドル
	int m_imgHandle;

	// 画像の回転率
	float m_rot;

	// 画像の中心座標
	Vector2 m_center;

	// 画像の拡大率
	Vector2 m_scale;

	// 画像の透明度
	float m_alpha;
};