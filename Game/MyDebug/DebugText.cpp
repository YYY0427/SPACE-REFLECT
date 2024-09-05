#include "DebugText.h"
#include <cassert>
#include <DxLib.h>

namespace Debug
{
	// 静的メンバ変数の実体化
	int Text::m_logCount = 0;
	int Text::m_fontHandle = -1;
	std::list<Text::LogData> Text::m_logList{};

	// 初期化
	void Text::Init()
	{
// デバッグ時のみ
#ifdef _DEBUG
		// 初期化
		m_logCount = 0;

		// フォントを作成
		m_fontHandle = CreateFontToHandle(font.c_str(), font_size, font_thickness);

		// フォントハンドルが作成できたか確認
		assert(m_fontHandle != -1);
#endif
	}

	// 終了処理
	void Text::End()
	{
		// デバッグ時のみ
#ifdef _DEBUG
		// ログのクリア
		ClearLog();

		// フォントを削除
		DeleteFontToHandle(m_fontHandle);
#endif
	}

	// ログのクリア
	void Text::ClearLog()
	{
		// デバッグ時のみ
#ifdef _DEBUG
	// ログのリストをクリア
		m_logList.clear();

		// カウントをリセット
		m_logCount = 0;
#endif
	}

	// ログの描画
	void Text::DrawLog()
	{
		// デバッグ時のみ
#ifdef _DEBUG
	// 全てのログを回す
		for (auto& log : m_logList)
		{
			// 半透明の背景を文字の横幅、立幅に合わせて描画
			int width = 0, height = 0, line = 0;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, draw_back_alpha);
			GetDrawStringSizeToHandle(&width, &height, &line, log.str.c_str(), static_cast<int>(log.str.size()), m_fontHandle);
			DrawBox(draw_width, (draw_height_interval * m_logCount) + draw_start_height, draw_width + width, (draw_height_interval * m_logCount) + height + draw_start_height, draw_back_color, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			// 文字の描画
			// ログの数だけ間隔を開けて描画
			DrawStringToHandle(
				draw_width,
				draw_start_height + (draw_height_interval * m_logCount),
				log.str.c_str(),
				log.color,
				m_fontHandle);

			// カウント
			m_logCount++;
		}
#endif
	}

	// ログの追加
	void Text::AddLog(const std::string& string, const unsigned int color)
	{
		// デバッグ時のみ
#ifdef _DEBUG
	// ログデータを作成
		LogData logData;
		logData.str = string;
		logData.color = color;

		// ログのリストに追加
		m_logList.push_back(logData);
#endif
	}
}