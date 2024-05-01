#include "MessageManager.h"
#include "StringUtil.h"
#include "../Util/FileUtil.h"

namespace
{
	// メッセージデータのファイルパス
	const std::string message_file_path = "Data/Csv/Message.csv";
}

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
void MessageManager::LoadMessageFileData()
{
	// ファイル情報の読み込み(読み込みに失敗したら止める)
	std::vector<std::vector<std::string>> str = FileUtil::LoadCsvFile(message_file_path);
	for (auto& data : str)
	{
		// csvデータから取得したデータからフォントハンドルの作成して格納
		MessageData messageData;
		std::string font = data[static_cast<int>(MessageDataType::FONT)];
		int size = std::stoi(data[static_cast<int>(MessageDataType::SIZE)]);
		int thick = std::stoi(data[static_cast<int>(MessageDataType::THICK)]);
		messageData.fontHandle = CreateFontToHandle(font.c_str(), size, thick, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

		// 文字列の格納
		messageData.string = data[static_cast<int>(MessageDataType::STRING)];

		// データの格納
		m_messageDataTable[data[static_cast<int>(MessageDataType::ID)]] = messageData;
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
void MessageManager::DrawString(const std::string& id, const int x, const int y, const unsigned int color)
{
	// ロードしていない場合は止める
	assert(m_messageDataTable.find(id) != m_messageDataTable.end());	

	DrawStringToHandle(x, y, m_messageDataTable[id].string.c_str(), color, m_messageDataTable[id].fontHandle);
}

// ファイルに読み込んだ文字列を指定した座標を中心とした位置に表示
void MessageManager::DrawStringCenter(const std::string& id, const int x, const int y, const unsigned int color, const unsigned int edgeColor)
{
	// ロードしていない場合は止める
	assert(m_messageDataTable.find(id) != m_messageDataTable.end());	

	// フォントサイズ、、文字列の長さから文字列の横幅の取得
	std::string string = m_messageDataTable[id].string;
	int fontHandle = m_messageDataTable[id].fontHandle;
	int width = GetDrawStringWidthToHandle(string.c_str(), static_cast<int>(strlen(string.c_str())), fontHandle);
	int height = GetFontSizeToHandle(fontHandle);

	// 取得した文字列の横幅から文字列を横中央位置に表示
	DrawStringToHandle(x - width / 2, y - height / 2, string.c_str(), color, fontHandle, edgeColor);
}

// メッセージのデータの取得
const MessageData& MessageManager::GetMessageData(const std::string& id)
{
	// ロードしていない場合は止める
	assert(m_messageDataTable.find(id) != m_messageDataTable.end());
	return m_messageDataTable[id];
}

// メッセージの幅の取得
int MessageManager::GetMessageWidth(const std::string& id)
{
	// ロードしていない場合は止める
	assert(m_messageDataTable.find(id) != m_messageDataTable.end());

	// フォントサイズ、、文字列の長さから文字列の横幅の取得
	std::string string = m_messageDataTable[id].string;
	int fontHandle = m_messageDataTable[id].fontHandle;
	return GetDrawStringWidthToHandle(string.c_str(), static_cast<int>(strlen(string.c_str())), fontHandle);
}

// メッセージの高さの取得
int MessageManager::GetMessageHeight(const std::string& id)
{
	// ロードしていない場合は止める
	assert(m_messageDataTable.find(id) != m_messageDataTable.end());

	// フォントサイズ、、文字列の長さから文字列の横幅の取得
	int fontHandle = m_messageDataTable[id].fontHandle;
	return GetFontSizeToHandle(fontHandle);
}