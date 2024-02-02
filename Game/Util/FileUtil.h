#pragma once
#include <string>
#include <vector>

/// <summary>
/// ファイルユーティリティ
/// </summary>
namespace FileUtil
{
	// フォルダ配下のファイル名の一括読み込み
	std::vector<std::string> LoadFileNames(std::string folderPath, std::string extension);

	// フォルダ配下のフォルダ名の一括読み込み
	std::vector<std::string> LoadDirectoryNames(std::string folderPath);
}