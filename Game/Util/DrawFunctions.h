#pragma once
#include <tchar.h>

// 自分用ライブラリ用名前空間
namespace my
{
	/// <summary>
	/// 画像をロードする
	/// </summary>
	/// <param name="path">パス文字列</param>
	/// <returns>画像のハンドル</returns>
	int MyLoadGraph(const TCHAR* path);

	/// <summary>
	/// モデルをロードする
	/// </summary>
	/// <param name="path">パス文字列</param>
	/// <returns>モデルのハンドル</returns>
	int MyLoadModel(const TCHAR* path);
}
