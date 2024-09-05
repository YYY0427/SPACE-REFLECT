#include <DxLib.h>
#include <crtdbg.h>
#include "Application.h"

// エントリーポイント
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
#ifdef _DEBUG
	// メモリリークの確認
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// リークが発生しているメモリ確保回数を引数にすることで、
	// その個所にデバッグブレークをかけてくれる
//	_CrtSetBreakAlloc(136);
#endif

	// アプリケーションのインスタンスを取得
	const auto& app = Application::GetInstance();

	// 初期化
	if (!app->Init())
	{
		// 初期化に失敗したら終了
		return -1;
	}

	// 実行
	app->Run();

	// インスタンスの削除
	app->DeleteInstance();

	// ソフトの終了 
	return 0;				
}