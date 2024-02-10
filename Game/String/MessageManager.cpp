#include "MessageManager.h"
#include "StringUtil.h"
#include <fstream>
#include <sstream>

// コンストラクタ
MessageManager::MessageManager()
{
}

// デストラクタ
MessageManager::~MessageManager()
{
}

// 唯一のインスタンスを返す
MessageManager& MessageManager::GetInstance()
{
	// 唯一の実態
	static MessageManager instance;

	// 唯一の実態参照を返す
	return instance;
}

// ファイルから文字列のデータを読み取ってデータテーブルに格納
void MessageManager::LoadAndStoreStringFileData()
{
	// ファイル情報の読み込み(読み込みに失敗したら止める)
	std::ifstream ifs("Data/Csv/Message.csv");
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
		std::vector<std::string> strvec = StringUtil::Split(line, ',');

		// csvデータから取得したデータからフォントハンドルの作成して格納
		MessageData data;
		std::string font = strvec[static_cast<int>(MessageDataType::FONT)];		
		int size = std::stoi(strvec[static_cast<int>(MessageDataType::SIZE)]);
		int thick = std::stoi(strvec[static_cast<int>(MessageDataType::THICK)]);
		data.fontHandle = CreateFontToHandle(font.c_str(), size, thick, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

		// 文字列の格納
		data.string = strvec[static_cast<int>(MessageDataType::STRING)];	

		// データの格納
		m_messageDataTable[strvec[static_cast<int>(MessageDataType::ID)]] = data;
	}
}

// 終了処理
void MessageManager::End()
{
	// フォントデータの削除
	for (auto& font : m_messageDataTable)
	{
		DeleteFontToHandle(font.second.fontHandle);
	}
}

// ファイルに読み込んだ文字列の表示
void MessageManager::DrawString(std::string id, int x, int y, unsigned int color)
{
	// ロードしていない場合は止める
	assert(m_messageDataTable.find(id) != m_messageDataTable.end());	

	DrawStringToHandle(x, y, m_messageDataTable[id].string.c_str(), color, m_messageDataTable[id].fontHandle);
}

// ファイルに読み込んだ文字列を指定した座標を中心とした位置に表示
void MessageManager::DrawStringCenter(std::string id, int x, int y, unsigned int color)
{
	// ロードしていない場合は止める
	assert(m_messageDataTable.find(id) != m_messageDataTable.end());	

	// フォントサイズ、、文字列の長さから文字列の横幅の取得
	std::string string = m_messageDataTable[id].string;
	int fontHandle = m_messageDataTable[id].fontHandle;
	int width = GetDrawStringWidthToHandle(string.c_str(), static_cast<int>(strlen(string.c_str())), fontHandle);
	int height = GetFontSizeToHandle(fontHandle);

	// 取得した文字列の横幅から文字列を横中央位置に表示
	DrawStringToHandle(x - width / 2, y - height / 2, string.c_str(), color, fontHandle);
}

// メッセージのデータの取得
MessageData MessageManager::GetMessageData(std::string id)
{
	// ロードしていない場合は止める
	assert(m_messageDataTable.find(id) != m_messageDataTable.end());
	return m_messageDataTable[id];
}

// メッセージの幅の取得
int MessageManager::GetMessageWidth(std::string id)
{
	// ロードしていない場合は止める
	assert(m_messageDataTable.find(id) != m_messageDataTable.end());

	// フォントサイズ、、文字列の長さから文字列の横幅の取得
	std::string string = m_messageDataTable[id].string;
	int fontHandle = m_messageDataTable[id].fontHandle;
	return GetDrawStringWidthToHandle(string.c_str(), static_cast<int>(strlen(string.c_str())), fontHandle);
}

// メッセージの高さの取得
int MessageManager::GetMessageHeight(std::string id)
{
	// ロードしていない場合は止める
	assert(m_messageDataTable.find(id) != m_messageDataTable.end());

	// フォントサイズ、、文字列の長さから文字列の横幅の取得
	int fontHandle = m_messageDataTable[id].fontHandle;
	return GetFontSizeToHandle(fontHandle);
}