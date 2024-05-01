#pragma once
#include <string>
#include <vector>

/// <summary>
/// ファイルユーティリティ
/// </summary>
namespace FileUtil
{
	/// <summary>
	/// フォルダ配下のファイル名、フォルダ名の取得
	/// </summary>
	/// <param name="folderPath">パス</param>
	/// <param name="extension">拡張子(フォルダ名の場合はなし)</param>
	/// <returns>取得したファイル名、フォルダ名の名前</returns>
	std::vector<std::string> LoadFileNames(const std::string& folderPath, const std::string& extension = "");

	/// <summary>
	/// CSVファイルの読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns>読み込んだデータ</returns>
	std::vector<std::vector<std::string>> LoadCsvFile(const std::string& filePath);
}