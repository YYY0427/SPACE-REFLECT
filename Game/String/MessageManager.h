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
	// デストラクタ
	~MessageManager();

	/// <summary>
	/// 唯一のインスタンスを返す
	/// </summary>
	/// <returns>唯一の実態の参照</returns>
	static MessageManager& GetInstance();

	// ファイルから文字列のデータを読み取ってデータテーブルに格納
	void LoadAndStoreStringFileData();

	// 終了処理
	void End();

	/// <summary>
	/// ファイルに読み込んだ文字列の表示
	/// </summary>
	/// <param name="id">表示したい文字列のID</param>
	/// <param name="x">Y軸の座標</param>
	/// <param name="y">Y軸の座標</param>
	/// <param name="color">文字の描画色</param>
	void DrawString(std::string id, int x, int y, unsigned int color);

	/// <summary>
	/// ファイルに読み込んだ文字列を指定した座標を中心とした位置に表示
	/// </summary>
	/// <param name="id">表示したい文字列のID</param>
	/// <param name="x">真ん中に指定したいX軸の座標</param>
	/// <param name="y">Y軸の座標</param>
	/// <param name="color">文字の描画色</param>
	void DrawStringCenter(std::string id, int x, int y, unsigned int color);

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
	void DrawNumberCenter(std::string id, T value, int x, int y, unsigned int color)
	{
		// ロードしていない場合は止める
		assert(m_messageDataTable.find(id) != m_messageDataTable.end());

		// 数字を文字列に変換
		std::string str = std::to_string(value);

		// 数字の描画
		StringUtil::DrawStringCenter(x, y, color, str, m_messageDataTable[id].fontHandle);
	}

	// メッセージのデータの取得
	MessageData GetMessageData(std::string id);

	// メッセージの幅の取得
	int GetMessageWidth(std::string id);

	// メッセージの高さの取得
	int GetMessageHeight(std::string id);

private:
	// コンストラクタ
	// シングルトンパターンなのでprivate
	MessageManager();

	// コピーと代入の禁止
	MessageManager(const MessageManager&) = delete;			// コピーコンストラクタ禁止
	void operator = (const MessageManager&) = delete;		// 代入禁止

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