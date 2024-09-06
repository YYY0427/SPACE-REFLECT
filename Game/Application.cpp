#include "Application.h"
#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include "Effect/Effekseer3DEffectManager.h"
#include "SaveData.h"
#include "Scene/SceneManager.h"
#include "Scene/TransporterScene.h"
#include "Scene/TitleScene.h"
#include "Sound/SoundManager.h"
#include "String/MessageManager.h"
#include "String/Font.h"
#include "Input/InputManager.h"
#include "MyDebug/DebugText.h"
#include "Score/ScoreRanking.h"
#include "Score/ScoreManager.h"
#include "UI/UIManager.h"
#include "Resource/Model/ModelResourceManager.h"
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

	// csvファイルに沿ってサウンドをロード
	Sound::Manager::GetInstance()->LoadCsvSoundFile();

	// フォントのロード
	String::Font::Load();

	// スコアランキングのロード
	Score::Ranking::GetInstance()->LoadScore();

	// メッセージをロード
	String::MessageManager::GetInstance()->LoadMessageFileData();

	// Effekseerの初期化
	Effect::Effekseer3DManager::GetInstance()->Init();

	// 非同期読み込み設定に変更
	SetUseASyncLoadFlag(TRUE);

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
	Input::Manager::Init();

	// デバッグテキストの初期化
	Debug::Text::Init();

	// ダブルバッファモード
	// 裏画面に描画
	SetDrawScreen(DX_SCREEN_BACK);

	return true;
}

// ゲームループ
void Application::Run()
{
	// 最初のシーンを設定
	m_pSceneManager = std::make_shared<Scene::Manager>();
#ifdef _DEBUG
	m_pSceneManager->ChangeScene(std::make_shared<Scene::Transporter>(m_pSceneManager));
#else
	m_pSceneManager->ChangeScene(std::make_shared<Scene::Title>(m_pSceneManager));
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
		Debug::Text::ClearLog();

		// 入力の更新
		Input::Manager::Update();

		// シーンの更新
		m_pSceneManager->Update();

		// サウンドの更新
		Sound::Manager::GetInstance()->Update();

		// シーンの描画
		m_pSceneManager->Draw();

		// デバッグテキストの描画
		Debug::Text::DrawLog();

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
	// フォントの終了処理
	String::Font::End();

	// DebugTextの終了処理
	Debug::Text::End();

	// スコアランキングのセーブ
	Score::Ranking::GetInstance()->SaveScore();

	// シーンを全て削除
	m_pSceneManager->ClearScene();

	// サウンドのインスタンス削除
	Sound::Manager::GetInstance()->DeleteInstance();

	// Effekseerのインスタンス削除
	Effect::Effekseer3DManager::GetInstance()->DeleteInstance();

	// スコアのインスタンス削除
	Score::Ranking::GetInstance()->DeleteInstance();
	Score::Manager::GetInstance()->DeleteInstance();

	// UIManagerのインスタンス削除
	UI::Manager::GetInstance()->DeleteInstance();

	// MessageManagerのインスタンス削除
	String::MessageManager::GetInstance()->DeleteInstance();

	// ModelResourceManagerのインスタンス削除
	Resource::Model::Manager::GetInstance()->DeleteInstance();

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