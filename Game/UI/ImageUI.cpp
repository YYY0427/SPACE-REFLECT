#include "ImageUI.h"
#include "../Util/DrawFunctions.h"
#include <DxLib.h>

// コンストラクタ
ImageUI::ImageUI(std::string filePath) :
	m_rot(0.0f),
	m_scale(1.0f, 1.0f),
	m_alpha(1.0f),
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
