#include "DxLib.h"
#include "Application.h"

// エントリーポイント
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// アプリケーションのインスタンスを取得
	auto& app = Application::GetInstance();

	// 初期化
	if (!app.Init())
	{
		// 初期化に失敗したら終了
		return -1;
	}

	// 実行
	app.Run();

	// ソフトの終了 
	return 0;				
}