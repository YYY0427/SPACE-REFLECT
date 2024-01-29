#include "DebugText.h"
#include <cassert>

// 静的メンバ変数の実体化
int DebugText::m_logCount = 0;
int DebugText::m_fontHandle = -1;

// 初期化
void DebugText::Init()
{
#ifdef _DEBUG
	// 初期化
	m_logCount = 0;

	// フォントを作成
	m_fontHandle = CreateFontToHandle(font.c_str(), font_size, font_thickness);
	assert(m_fontHandle != -1);
#endif
}

// ログのクリア
void DebugText::Clear()
{
#ifdef _DEBUG
	m_logCount = 0;
#endif
}

// ログの描画
void DebugText::Log(std::string string)
{
#ifdef _DEBUG
	// 半透明の背景を描画
	int width, height, line = 0;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);
	GetDrawStringSizeToHandle(&width, &height, &line, string.c_str(), string.size(), m_fontHandle);
	DrawBox(draw_width, height * m_logCount, draw_width + width, (height * m_logCount) + height, draw_back_color, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 描画
	DrawStringToHandle(
		draw_width,
		draw_start_height + (draw_height_interval * m_logCount),
		string.c_str(),
		draw_font_color,
		m_fontHandle);

	// カウント
	m_logCount++;
#endif
}