#pragma once
#include <string>
#include <DxLib.h>
#include <list>

namespace
{
	// 描画するX座標
	constexpr int draw_width = 0;

	// 描画するスタートY座標
	constexpr int draw_start_height = 150;

	// 描画するY座標の間隔
	constexpr int draw_height_interval = 20;

	// 描画する文字の色
	const unsigned int draw_font_color = 0xffffff;

	// フォント
	const std::string font = "ＭＳ ゴシック";

	// 文字サイズ
	constexpr int font_size = 20;

	// 文字の太さ
	constexpr int font_thickness = 9;

	// 文字の背景の色
	const unsigned int draw_back_color = 0xffffff;

	// 文字の背景の透明度
	constexpr int draw_back_alpha = 127;
}

namespace Debug
{
	class Text
	{
	/// <summary>
	/// デバッグ時に表示するテキスト
	/// printfのようなもの
	/// </summary>
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		static void Init();

		/// <summary>
		/// 終了処理
		/// </summary>
		static void End();

		/// <summary>
		/// ログのクリア
		/// </summary>
		static void ClearLog();

		/// <summary>
		/// ログの描画
		/// </summary>
		static void DrawLog();

		/// <summary>
		/// ログの追加(文字のみ)
		/// </summary>
		/// <param name="string">出力する文字列</param>
		/// <param name="color">文字の色</param>
		static void AddLog(const std::string& string, const unsigned int color = 0xffffff);

		/// <summary>
		/// ログの追加(文字と数字)
		/// </summary>
		/// <param name="string">文字列</param>
		/// <typeparam name="T">変数の型</typeparam>
		/// <param name="array">変数</param>
		/// <typeparam name="N">変数の数</typeparam>
		/// <param name="color">文字の色</param>
		template <typename T, size_t N>
		static void AddLog(const std::string& string, const T(&array)[N], const unsigned int color = 0xffffff)
		{
#ifdef _DEBUG
			// ログデータの作成
			LogData logData;
			logData.str = string;
			logData.color = color;

			// 配列の中身を文字列に変換
			for (int i = 0; i < N; i++)
			{
				// 1つ目の要素は : で区切る
				if (i == 0)
				{
					logData.str += " : " + std::to_string(array[i]);
				}
				// 2つ目以降の要素は , で区切る
				else
				{
					logData.str += ", " + std::to_string(array[i]);
				}
			}
			// ログを格納
			m_logList.push_back(logData);
#endif
		}

	private:
		// ログデータ
		struct LogData
		{
			// 文字列
			std::string str;

			// 文字の色
			unsigned int color = 0xffffff;
		};

	private:
		// ログリスト
		static std::list<LogData> m_logList;

		// ログの出力回数
		static int m_logCount;

		// フォントハンドル
		static int m_fontHandle;
	};
}