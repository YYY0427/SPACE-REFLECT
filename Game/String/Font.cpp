#include "Font.h"
#include <cassert>

namespace
{
	// 読み込むフォントファイル名
	const LPCSTR font_data_path[] =
	{
		"Data/Font/Makinas-4-Flat.otf",
		"Data/Font/Makinas-4-Square.otf",
		"Data/Font/azuki.ttf",
		"Data/Font/Abel-Regular.ttf"
	};
}

namespace Font
{
	// フォントのロード
	void Load()
	{
		// フォントのロード
		for (auto& fontPath : font_data_path)
		{
			if (AddFontResourceEx(fontPath, FR_PRIVATE, NULL) == 0)
			{
				// ロードに失敗したら止める
				assert(!"フォントのロード失敗");
			}
		}
	}

	// フォントのアンロード
	void End()
	{
		// フォントのアンロード
		for (auto& fontPath : font_data_path)
		{
			if (RemoveFontResourceEx(fontPath, FR_PRIVATE, NULL) == 0)
			{
				// アンロードに失敗したら止める
				assert(!"フォントのアンロード失敗");
			}
		}
	}
}