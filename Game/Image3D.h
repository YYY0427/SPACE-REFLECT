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
	Image3D(std::string filePath);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="imgHandle">画像ハンドル</param>
	Image3D(int imgHandle);

	// デストラクタ
	~Image3D();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 画像ハンドルの取得
	int GetImageHandle() const;

	// 画像の頂点情報の取得
	std::array<VERTEX3D, 6> GetVertex() const;

	// 位置の設定
	void SetPos(Vector3 pos);

	// 回転の設定
	void SetRot(Vector3 rot);

	// 画像の横幅の設定
	void SetImgWidth(float width);

	// 画像の縦幅の設定
	void SetImgHeight(float height);

	// 画像の透明度の設定
	void SetAlpha(int alpha);

	// 画像の横幅と立幅の拡大率を元の画像のサイズに戻す
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