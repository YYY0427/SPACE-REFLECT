#pragma once
#include <DxLib.h>
#include <string>
#include <array>
#include "Math/Vector3.h"

/// <summary>
/// 3D画像クラス
/// </summary>
class Image3D
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="filePath">画像のファイルパス</param>
	Image3D(const std::string& filePath);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="imgHandle">画像ハンドル</param>
	Image3D(const int imgHandle);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Image3D();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 画像ハンドルの取得 
	/// </summary>
	/// <returns>画像ハンドル</returns>
	int GetImageHandle() const;

	/// <summary>
	/// 画像の頂点情報の取得 
	/// </summary>
	/// <returns>画像の頂点情報</returns>
	const std::array<VERTEX3D, 6>& GetVertex() const;

	/// <summary>
	/// 位置の設定 
	/// </summary>
	/// <param name="pos">位置</param>
	void SetPos(const Vector3& pos);

	/// <summary>
	/// 回転の設定
	/// </summary>
	/// <param name="rot">回転</param>
	void SetRot(const Vector3& rot);

	/// <summary>
	/// 画像の横幅の設定 
	/// </summary>
	/// <param name="width">横幅</param>
	void SetImgWidth(const float width);

	/// <summary>
	/// 画像の縦幅の設定 
	/// </summary>
	///	<param name="height">縦幅</param>
	void SetImgHeight(const float height);

	/// <summary>
	/// 画像の透明度の設定 
	/// </summary>
	/// <param name="alpha">透明度</param>
	void SetAlpha(const int alpha);

	/// <summary>
	/// 画像の横幅と立幅の拡大率を元の画像のサイズに戻す 
	/// </summary>
	void SetImgDafualtScale();

private:
	// 画像の頂点
	std::array<VERTEX3D, 6> m_vertex;

	// 画像の位置
	Vector3 m_pos;

	// 画像の回転
	Vector3 m_rot;

	// 画像の透明度
	int m_alpha;

	// 画像のハンドル
	int m_imgHandle;

	// 画像の横幅
	float m_imgWidth;

	// 画像の縦幅
	float m_imgHeight;
};