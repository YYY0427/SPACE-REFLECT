#include "StringManager.h"
#include <cassert>
#include <fstream>
#include <sstream>

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

// コンストラクタ
StringManager::StringManager()
{
}

// デストラクタ
StringManager::~StringManager()
{
}

// 唯一のインスタンスを返す
StringManager& StringManager::GetInstance()
{
	// 唯一の実態
	static StringManager instance;

	// 唯一の実態参照を返す
	return instance;
}

// ファイルから文字列のデータを読み取ってデータテーブルに格納
void StringManager::LoadAndStoreStringFileData()
{
	// フォントのロード
	for (auto& fontPath : font_data_path)
	{
		if (AddFontResourceEx(fontPath, FR_PRIVATE, NULL) == 0)
		{
			// ロードに失敗したら止める
			assert(0);
		}
	}

	// ファイル情報の読み込み(読み込みに失敗したら止める)
	std::ifstream ifs("Data/Csv/String.csv");
	assert(ifs);

	// csvデータを1行ずつ読み取る
	bool isFirst = false;
	std::string line;
	while (getline(ifs, line))
	{
		// 1行目は読み込まない
		// 1行目には項目が書いてあるため
		if (!isFirst)
		{
			isFirst = true;
			continue;
		}

		// csvデータ１行を','で複数の文字列に変換
		std::vector<std::string> strvec = SplitString(line, ',');

		// csvデータから取得したデータからフォントハンドルの作成して格納
		StringData data;
		std::string font = strvec[static_cast<int>(StringDataType::FONT)];		
		int size = std::stoi(strvec[static_cast<int>(StringDataType::SIZE)]);
		int thick = std::stoi(strvec[static_cast<int>(StringDataType::THICK)]);
		data.fontHandle = CreateFontToHandle(font.c_str(), size, thick, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

		// 文字列の格納
		data.string = strvec[static_cast<int>(StringDataType::STRING)];	

		// データの格納
		m_stringDataTable[strvec[static_cast<int>(StringDataType::ID)]] = data;
	}
}

// 終了処理
void StringManager::End()
{
	// フォントデータの削除
	for (auto& font : m_stringDataTable)
	{
		DeleteFontToHandle(font.second.fontHandle);
	}
	// フォントのアンロード
	for (auto& fontPath : font_data_path)
	{
		if (RemoveFontResourceEx(fontPath, FR_PRIVATE, NULL) == 0)
		{
			// アンロードに失敗したら止める
			assert(0);
		}
	}
}

// 文字列を区切る(複数の文字列に変換)
std::vector<std::string> StringManager::SplitString(std::string& input, char delimiter)
{
	// stringをgetlineで使えるデータに変換
	std::istringstream stream(input);	

	// 分割した文字列1つ分を格納
	std::string field;					

	// 分割後の文字列の配列
	std::vector<std::string> result;	

	while (getline(stream, field, delimiter))
	{
		// 分割した文字列1つ分を格納する
		result.push_back(field);
	}
	return result;
}

// ファイルに読み込んだ文字列の表示
void StringManager::DrawString(std::string id, int x, int y, unsigned int color)
{
	// ロードしていない場合は止める
	assert(m_stringDataTable.find(id) != m_stringDataTable.end());	

	DrawStringToHandle(x, y, m_stringDataTable[id].string.c_str(), color, m_stringDataTable[id].fontHandle);
}

// ファイルに読み込んだ文字列を指定した座標を中心とした位置に表示
void StringManager::DrawStringCenter(std::string id, int x, int y, unsigned int color)
{
	// ロードしていない場合は止める
	assert(m_stringDataTable.find(id) != m_stringDataTable.end());	

	// フォントサイズ、、文字列の長さから文字列の横幅の取得
	std::string string = m_stringDataTable[id].string;
	int fontHandle = m_stringDataTable[id].fontHandle;
	int width = GetDrawStringWidthToHandle(string.c_str(), static_cast<int>(strlen(string.c_str())), fontHandle);
	int height = GetFontSizeToHandle(fontHandle);

	// 取得した文字列の横幅から文字列を横中央位置に表示
	DrawStringToHandle(x - width / 2, y - height / 2, string.c_str(), color, fontHandle);
}

// フォントを使った数字を指定した座標を中心として表示
void StringManager::DrawFormatStringCenter(std::string id, int num, int x, int y, unsigned int color)
{
	// ロードしていない場合は止める
	assert(m_stringDataTable.find(id) != m_stringDataTable.end());

	// フォントサイズ、文字列の長さから文字列の横幅の取得
	std::string string = std::to_string(num);
	int fontHandle = m_stringDataTable[id].fontHandle;
	int width = GetDrawStringWidthToHandle(string.c_str(), static_cast<int>(strlen(string.c_str())), fontHandle);

	// 取得した文字列の横幅から文字列を横中央位置に表示
	DrawFormatStringToHandle(x - width / 2, y, color, fontHandle, "%d", num);
}