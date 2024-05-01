#include "StringUtil.h"
#include <fstream>
#include <sstream>

namespace StringUtil
{
	// 文字列を分割する
	std::vector<std::string> StringUtil::Split(const std::string& str, const char delimiter)
	{
		// stringをgetlineで使えるデータに変換
		std::istringstream stream(str);

		// 分割した文字列1つ分を格納
		std::string field;

		// 分割後の文字列の配列
		std::vector<std::string> result;

		// 区切り文字がなくなるまでループ
		while (getline(stream, field, delimiter))
		{
			// 分割した文字列1つ分を格納する
			result.push_back(field);
		}
		return result;
	}

	// 指定した座標を中心に文字列を描画する 
	void DrawStringCenter(const int x, const int y, const unsigned int color, const std::string& str, const int fontHandle, const unsigned int edgeColor)
	{
		// 文字列の幅と長さを取得
		int width = -1;
		int height = -1;
		if (fontHandle == -1)
		{
			// フォントハンドルが指定されていない場合
			width = GetDrawStringWidth(str.c_str(), static_cast<int>(strlen(str.c_str())));
			height = GetFontSize();

			// エラーが起きていないかチェック
			assert(width != -1 && "文字列の長さの取得に失敗");
			assert(height != -1 && "文字列の高さの取得に失敗");

			// フォントハンドルを指定せずに描画
			DrawString(x - width / 2, y - height / 2, str.c_str(), color, edgeColor);
		}
		else
		{
			// フォントハンドルが指定されている場合
			width = GetDrawStringWidthToHandle(str.c_str(), static_cast<int>(strlen(str.c_str())), fontHandle);
			height = GetFontSizeToHandle(fontHandle);

			// エラーが起きていないかチェック
			assert(width != -1 && "文字列の長さの取得に失敗");
			assert(height != -1 && "文字列の高さの取得に失敗");

			// フォントハンドルを指定して描画
			DrawStringToHandle(x - width / 2, y - height / 2, str.c_str(), color, fontHandle, edgeColor);
		}
	}
}