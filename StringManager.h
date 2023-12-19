#pragma once
#include <DxLib.h>
#include <string>
#include <vector>
#include <map>

/// <summary>
/// 文字列を管理するクラス
/// </summary>
class StringManager
{
public:
	// デストラクタ
	~StringManager();

	/// <summary>
	/// 唯一のインスタンスを返す
	/// </summary>
	/// <returns>唯一の実態の参照</returns>
	static StringManager& GetInstance();

	// ファイルから文字列のデータを読み取ってデータテーブルに格納
	void LoadAndStoreStringFileData();

	// 終了処理
	void End();

	/// <summary>
	/// 文字列を区切る(複数の文字列に変換)
	/// </summary>
	/// <param name="input">区切りたい文字列</param>
	/// <param name="delimiter">文字列を区切る文字</param>
	/// <returns>区切った文字列</returns>
	std::vector<std::string> SplitString(std::string& input, char delimiter);

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
	void DrawFormatStringCenter(std::string id, int num, int x, int y, unsigned int color);

private:
	// コンストラクタ
	// シングルトンパターンなのでprivate
	StringManager();

	// コピーと代入の禁止
	StringManager(const StringManager&) = delete;			// コピーコンストラクタ禁止
	void operator = (const StringManager&) = delete;		// 代入禁止

private:
	// 文字列データの種類
	enum class StringDataType
	{
		ID,
		STRING,
		FONT,
		SIZE,
		THICK
	};

private:
	// 文字列のデータ
	struct StringData
	{
		int fontHandle = -1;
		std::string string;
	};

private:
	// CSVファイルで指定した文字列をIDとした文字列のデータテーブル
	std::map<std::string, StringData> m_stringDataTable;
};