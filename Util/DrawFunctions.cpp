#include <DxLib.h>
#include <cassert>
#include "DrawFunctions.h"

namespace my
{
	// 画像をロードする
	int MyLoadGraph(const TCHAR* path)
	{
		int handle = LoadGraph(path);

		// -1以外じゃなかったら止める
		assert(handle != -1);	

		return handle;
	}

	int MyLoadModel(const TCHAR* path)
	{
		int handle = MV1LoadModel(path);

		// -1以外じゃなかったら止める
		assert(handle != -1);

		return handle;
	}
}