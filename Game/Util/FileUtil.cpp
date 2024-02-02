#include "FileUtil.h"
#include <filesystem>

namespace FileUtil
{
	// フォルダ配下のファイル名の一括読み込み
	std::vector<std::string> LoadFileNames(std::string folderPath, std::string extension)
	{
		std::vector<std::string> fileNames;

		// ファイル名の一括読み込み
		for (auto& p : std::filesystem::directory_iterator(folderPath))
		{
			// 拡張子の取得
			std::string ext = p.path().extension().string();

			// 拡張子が一致しているか
			if (ext == extension)
			{
				// ファイル名の保存
				fileNames.push_back(p.path().filename().string());
			}
		}
		return fileNames;
	}

	// フォルダ配下のフォルダ名の一括読み込み
	std::vector<std::string> LoadDirectoryNames(std::string folderPath)
	{
		std::vector<std::string> directoryNames;

		// フォルダ名の一括読み込み
		for (auto& p : std::filesystem::directory_iterator(folderPath))
		{
			// フォルダ名の保存
			directoryNames.push_back(p.path().filename().string());
		}
		return directoryNames;
	}
}