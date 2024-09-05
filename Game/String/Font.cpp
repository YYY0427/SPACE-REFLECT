#include "Font.h"
#include <cassert>

namespace
{
	// �ǂݍ��ރt�H���g�t�@�C����
	const LPCSTR font_data_path[] =
	{
		"Data/Font/Makinas-4-Flat.otf",
		"Data/Font/Makinas-4-Square.otf",
		"Data/Font/azuki.ttf",
		"Data/Font/Abel-Regular.ttf"
	};
}

namespace String::Font
{
	// �t�H���g�̃��[�h
	void Load()
	{
		// �t�H���g�̃��[�h
		for (auto& fontPath : font_data_path)
		{
			if (AddFontResourceEx(fontPath, FR_PRIVATE, NULL) == 0)
			{
				// ���[�h�Ɏ��s������~�߂�
				assert(!"�t�H���g�̃��[�h���s");
			}
		}
	}

	// �t�H���g�̃A�����[�h
	void End()
	{
		// �t�H���g�̃A�����[�h
		for (auto& fontPath : font_data_path)
		{
			if (RemoveFontResourceEx(fontPath, FR_PRIVATE, NULL) == 0)
			{
				// �A�����[�h�Ɏ��s������~�߂�
				assert(!"�t�H���g�̃A�����[�h���s");
			}
		}
	}
}