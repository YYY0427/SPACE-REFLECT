#include "Application.h"
#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include "Effect/Effekseer3DEffectManager.h"
#include "SaveData.h"
#include "Scene/SceneManager.h"
#include "Scene/DebugScene.h"
#include "Scene/TitleScene.h"
#include "SoundManager.h"
#include "String/MessageManager.h"
#include "String/Font.h"
#include "Util/InputState.h"
#include "MyDebug/DebugText.h"
#include "Score/ScoreRanking.h"
#include <string>
#include <memory>

namespace
{
    // 画面サイズ
    constexpr int window_width = 1280;	
    constexpr int window_height = 720;	

    // ウィンドウタイトル
    const std::string window_title = "REFLECT";

	// カラービット数
	constexpr int color_depth = 32;

	// 音の１メートルに相当する値
	constexpr int one_meter = 100;

	// fps
	constexpr int fps = 60;
}

// コンストラクタ
Application::Application() :
	m_screenSize{ window_width, window_height },
	m_isExit(false)
{
}

// Applicationクラスのインスタンスを取得
Application& Application::GetInstance()
{
	// 自分自身の静的オブジェクトを作成
	static Application instance;

	// 作成したオブジェクトの参照を返す
	return instance;
}

// 初期化
bool Application::Init()
{
	// セーブデータの読み込み
	auto& saveData = SaveData::GetInstance();
	saveData.Load();

	// Windowモード設定
	ChangeWindowMode(saveData.GetSaveData().windowMode);

	// Window名設定
	SetMainWindowText(window_title.c_str());

	// 画面サイズの設定
	SetGraphMode(window_width, window_height, color_depth);

	// ゲーム中にウィンドウモードを切り替えてもグラフィックハンドルをリセットしない
	SetChangeScreenModeGraphicsSystemResetFlag(TRUE);

	// ほかのウィンドウを選択していても動くようにする
	SetAlwaysRunFlag(TRUE);

	// ウィンドウのサイズを変更可能にする
	SetWindowSizeChangeEnableFlag(TRUE);

	// DirectX11を使用
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	// １メートルに相当する値を設定する
	Set3DSoundOneMetre(one_meter);

	// XAudioを有効化
	SetEnableXAudioFlag(TRUE);

	// 垂直同期を有効化
	SetWaitVSyncFlag(TRUE);

	// 3Dでの描画処理のサポート
	SetUseLarge3DPositionSupport(TRUE);

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return false;
	}

	// 非同期読み込み設定に変更
//	SetUseASyncLoadFlag(TRUE);

	// csvファイルに沿ってサウンドをロード
	SoundManager::GetInstance().LoadCsvSoundFile();

	// フォントのロード
	Font::Load();

	// スコアランキングのロード
	ScoreRanking::GetInstance().LoadScore();

	// メッセージをロード
	MessageManager::GetInstance().LoadMessageFileData();

	// Effekseerの初期化
	Effekseer3DEffectManager::GetInstance().Init();

	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ
	// Effekseerを使用する場合は必ず設定する
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DXライブラリのデバイスロストした時のコールバックを設定する
	// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する
	// ただし、DirectX11を使用する場合は実行する必要はない
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// SetDrawScreenを使用した際にカメラがリセットされないようにする
	SetUseSetDrawScreenSettingReset(FALSE);

	// Zバッファを有効化
	SetUseZBufferFlag(TRUE);

	// Zバッファへの書き込みを行う
	SetWriteZBuffer3D(TRUE);

	// ポリゴンの裏面を描画しない
	SetUseBackCulling(TRUE);

	// 入力タイプの初期化
	InputState::Init();

	// デバッグテキストの初期化
	DebugText::Init();

	// ダブルバッファモード
	// 裏画面に描画
	SetDrawScreen(DX_SCREEN_BACK);

	return true;
}

// ゲームループ
void Application::Run()
{
	// 最初のシーンを設定
	SceneManager sceneManager;
#ifdef _DEBUG
	sceneManager.ChangeScene(std::make_shared<DebugScene>(sceneManager));
#else
	sceneManager.ChangeScene(std::make_shared<TitleScene>(sceneManager));
#endif

	// ゲームループ
	while (ProcessMessage() == 0)
	{
		// 時間の取得
		LONGLONG time = GetNowHiPerformanceCount();

		// Windowモード設定
		auto& saveData = SaveData::GetInstance();
		ChangeWindowMode(saveData.GetSaveData().windowMode);

		// デバッグテキストのクリア
		DebugText::Clear();

		// 入力の更新
		InputState::Update();

		// シーンの更新
		sceneManager.Update();

		// サウンドの更新
		SoundManager::GetInstance().Update();

		// シーンの描画
		sceneManager.Draw();

		// デバッグテキストの描画
		DebugText::Draw();

		// 画面の更新
		ScreenFlip();

#ifdef _DEBUG
		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;
#endif

		// ゲーム終了フラグが立っていたら終了
		if(m_isExit) break;

		// fpsを固定
		while (GetNowHiPerformanceCount() - time < static_cast<long long>(1000000 / fps))
		{

		}
	}
	// 終了処理
	End();
}

// 終了処理
void Application::End()
{
	// サウンドの終了処理
	SoundManager::GetInstance().End();

	// Effekseerの終了処理
	Effekseer3DEffectManager::GetInstance().End();

	// フォントの終了処理
	Font::End();

	// スコアランキングのセーブ
	ScoreRanking::GetInstance().SaveScore();

	// MessageManagerの終了処理
	MessageManager::GetInstance().End();

	// DebugTextの終了処理
	DebugText::End();

	// DXライブラリ使用の終了処理
	DxLib_End();
}

// ウィンドウサイズの取得
const Size& Application::GetWindowSize() const
{
	return m_screenSize;
}

// fpsを取得
int Application::GetFps() const
{
	return fps;
}

// 終了
void Application::Exit()
{
	m_isExit = true;
}