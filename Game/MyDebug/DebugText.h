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

	// 半透明の背景の色
	const unsigned int draw_back_color = 0xffffff;
}

/// <summary>
/// デバッグ用の関数をまとめたクラス
/// </summary>
class DebugText
{
public:
	// 初期化
	static void Init();

	// 終了処理
	static void End();

	// ログのクリア
	static void Clear();

	// ログの描画
	static void Draw();

	// ログを出力する
	static void Log(std::string string);

	// デバッグテキストを描画する
	template <typename T, size_t N>
	static void Log(std::string string, const T(&array)[N])
	{
#ifdef _DEBUG
		// 文字列を作成
		std::string str = string;

		// 配列の中身を文字列に変換
		for (int i = 0; i < N; i++)
		{
			// 1つ目の要素は : で区切る
			if (i == 0)
			{
				str += " : " + std::to_string(array[i]);
			}
			// 2つ目以降の要素は , で区切る
			else
			{
				str += ", " + std::to_string(array[i]);
			}
		}
		// ログを格納
		m_logList.push_front(str);
#endif
	}

private:
	// ログのリスト
	static std::list<std::string> m_logList;

	// ログの出力回数
	static int m_logCount;

	// フォントハンドル
	static int m_fontHandle;
};