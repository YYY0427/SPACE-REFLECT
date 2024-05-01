#include "FileUtil.h"
#include "../String/StringUtil.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <cassert>

namespace FileUtil
{
	// フォルダ配下のファイル名の一括読み込み
	std::vector<std::string> LoadFileNames(const std::string& folderPath, const std::string& extension)
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

	// CSVファイルの読み込み
	// 1行目は読み込まない
	std::vector<std::vector<std::string>> LoadCsvFile(const std::string& filePath)
	{
		std::vector<std::vector<std::string>> data;

		// ファイルを開く
		std::ifstream inputFile(filePath);

		// ファイルが開けたか
		bool isFirstLine = true;
		if (inputFile.is_open())
		{
			// 1行ずつ読み込む
			std::string line;
			while (getline(inputFile, line))
			{
				// 1行目は読み込まない
				if (isFirstLine)
				{
					isFirstLine = false;
					continue;
				}

				// csvデータ１行を','で複数の文字列に変換
				std::vector<std::string> lineData = StringUtil::Split(line, ',');

				// 1行分のデータを保存
				data.push_back(lineData);
			}

			// ファイルを閉じる
			inputFile.close();
		}
		else
		{
			// ファイルが開けなかった
			assert(!"ファイルが開けませんでした");
		}
		return data;
	}
}