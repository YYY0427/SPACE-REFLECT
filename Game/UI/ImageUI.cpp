#include "ImageUI.h"
#include "../Util/DrawFunctions.h"
#include <DxLib.h>

// コンストラクタ
ImageUI::ImageUI(const std::string& filePath) :
	m_rot(0.0f),
	m_scale(1.0f, 1.0f),
	m_alpha(255),
	m_center(0.0f, 0.0f)
{
	// 画像の読み込み
	m_imgHandle = my::MyLoadGraph(filePath.c_str());
}

// デストラクタ
ImageUI::~ImageUI()
{
	// 画像の削除
	DeleteGraph(m_imgHandle);
}

// 描画
void ImageUI::Draw()
{
	// アルファブレンドの設定
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);

	// 画像の描画
	DrawRotaGraph3(
		m_pos.x, m_pos.y,		// 描画する座標
		m_center.x, m_center.y, // 画像の中心座標
		m_scale.x, m_scale.y,	// 拡大率
		m_rot,					// 回転角度
		m_imgHandle,			// 画像ハンドル
		true);

	// 描画設定を元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// 位置の設定
void ImageUI::SetPos(const Vector2& pos)
{
	m_pos = pos;
}

// 回転の設定
void ImageUI::SetRot(const float rot)
{
	m_rot = rot;
}

// 中心座標の設定
void ImageUI::SetCenter(const Vector2& center)
{
	m_center = center;
}

// 拡大率の設定
void ImageUI::SetScale(const Vector2& scale)
{
	m_scale = scale;
}

// 透明度の設定
void ImageUI::SetAlpha(const float alpha)
{
	m_alpha = alpha;
}

// 位置の取得
const Vector2& ImageUI::GetPos() const
{
	return m_pos;
}

// 回転の取得
float ImageUI::GetRot() const
{
	return m_rot;
}

// 中心座標の取得
const Vector2& ImageUI::GetCenter() const
{
	return m_center;
}

// 拡大率の取得
const Vector2& ImageUI::GetScale() const
{
	return m_scale;
}

// 透明度の取得
float ImageUI::GetAlpha() const
{
	return m_alpha;
}