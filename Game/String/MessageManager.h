#pragma once
#include <DxLib.h>
#include <string>
#include <vector>
#include <cassert>
#include <map>
#include "StringUtil.h"

// 文字列のデータ
struct MessageData
{
	int fontHandle = -1;
	std::string string;
};

/// <summary>
/// 文字列を管理するクラス
/// </summary>
class MessageManager
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~MessageManager();

	/// <summary>
	/// 唯一のインスタンスを返す
	/// </summary>
	/// <returns>唯一の実態の参照</returns>
	static MessageManager& GetInstance();

	/// <summary>
	/// メッセージファイルのデータを読み込む
	/// </summary>
	void LoadMessageFileData();

	/// <summary>
	/// 終了処理
	/// </summary>
	void End();

	/// <summary>
	/// ファイルに読み込んだ文字列の表示
	/// </summary>
	/// <param name="id">表示したい文字列のID</param>
	/// <param name="x">Y軸の座標</param>
	/// <param name="y">Y軸の座標</param>
	/// <param name="color">文字の描画色</param>
	void DrawString(const std::string& id, const int x, const int y, const unsigned int color);

	/// <summary>
	/// ファイルに読み込んだ文字列を指定した座標を中心とした位置に表示
	/// </summary>
	/// <param name="id">表示したい文字列のID</param>
	/// <param name="x">真ん中に指定したいX軸の座標</param>
	/// <param name="y">Y軸の座標</param>
	/// <param name="color">文字の描画色</param>
	void DrawStringCenter(const std::string& id, const int x, const int y, const unsigned int color, const unsigned int edgeColor = 0x000000);

	/// <summary>
	/// フォントに読み込んだデータから作成したフォントデータを使って
	/// 指定した座標を中心とした位置に数字の表示
	/// </summary>
	/// <param name="id">使いたいフォントのID</param>
	/// <param name="num">表示したい数字</param>
	/// <param name="x">真ん中に指定したいX軸の座標</param>
	/// <param name="y">Y軸の座標</param>
	/// <param name="color">文字の描画色</param>
	template<typename T>
	void DrawNumberCenter(const std::string& id, const T value, const int x, const int y, const unsigned const int color, const unsigned int edgeColor = 0x000000)
	{
		// ロードしていない場合は止める
		assert(m_messageDataTable.find(id) != m_messageDataTable.end());

		// 数字を文字列に変換
		std::string str = std::to_string(value);

		// 数字の描画
		StringUtil::DrawStringCenter(x, y, color, str, m_messageDataTable[id].fontHandle, edgeColor);
	}

	/// <summary>
	/// メッセージのデータの取得
	/// </summary>
	/// <param name="id">メッセージID</param>
	/// <returns>メッセージデータ</returns>
	const MessageData& GetMessageData(const std::string& id);

	/// <summary>
	/// メッセージの幅の取得
	/// </summary>
	/// <param name="id">メッセージID</param>
	/// <returns>メッセージの幅</returns>
	int GetMessageWidth(const std::string& id);

	/// <summary>
	/// メッセージの高さの取得
	/// </summary>
	/// <param name="id">メッセージID</param>
	/// <returns>メッセージの高さ</returns>
	int GetMessageHeight(const std::string& id);

private:
	/// <summary>
	/// コンストラクタ
	/// シングルトンのためprivate
	/// </summary>
	MessageManager();

	/// <summary>
	/// コピーコンストラクタ禁止
	/// </summary>
	/// <param name="">コピー元のインスタンス</param>
	MessageManager(const MessageManager&) = delete;			

	/// <summary>
	/// 代入禁止
	/// </summary>
	/// <param name="">代入元のインスタンス</param>
	void operator = (const MessageManager&) = delete;		

private:
	// 文字列データの種類
	enum class MessageDataType
	{
		ID,
		STRING,
		FONT,
		SIZE,
		THICK
	};

private:
	// CSVファイルで指定した文字列をIDとした文字列のデータテーブル
	std::map<std::string, MessageData> m_messageDataTable;
};