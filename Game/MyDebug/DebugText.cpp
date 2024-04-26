#include "DebugText.h"
#include <cassert>

// 静的メンバ変数の実体化
int DebugText::m_logCount = 0;
int DebugText::m_fontHandle = -1;
std::list<std::string> DebugText::m_logList{};

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

// 終了処理
void DebugText::End()
{
#ifdef _DEBUG
	// フォントを削除
	DeleteFontToHandle(m_fontHandle);
#endif
}

// ログのクリア
void DebugText::Clear()
{
#ifdef _DEBUG
	// ログのリストをクリア
	m_logList.clear();

	// カウントをリセット
	m_logCount = 0;
#endif
}

// ログの描画
void DebugText::Draw()
{
#ifdef _DEBUG
	for(auto& log : m_logList)
	{
		// 半透明の背景を描画
		int width, height, line = 0;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, draw_back_alpha);
		GetDrawStringSizeToHandle(&width, &height, &line, log.c_str(), log.size(), m_fontHandle);
		DrawBox(draw_width, height * m_logCount + draw_start_height, draw_width + width, (height * m_logCount) + height + draw_start_height, draw_back_color, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// 文字の描画
		DrawStringToHandle(
			draw_width,
			draw_start_height + (draw_height_interval * m_logCount),
			log.c_str(),
			draw_font_color,
			m_fontHandle);

		// カウント
		m_logCount++;
	}
#endif
}

// ログの追加
void DebugText::AddLog(const std::string& string)
{
#ifdef _DEBUG
	// ログのリストに追加
	m_logList.push_front(string);
#endif
}