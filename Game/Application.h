#pragma once

// ウィンドウのサイズ
struct Size
{
	int width;
	int height;
};

/// <summary>
/// アプリケーション全体を管理するシングルトンクラス
/// </summary>
class Application
{
public:
	/// <summary>
	/// Applicationクラスのインスタンスを取得
	/// </summary>
	/// <returns>Applicationクラスのインスタンス</returns>
	static Application& GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>true : 成功、false : 失敗</returns>
	bool Init();

	/// <summary>
	/// 実行
	/// </summary>
	void Run();
	
	/// <summary>
	/// ウィンドウのサイズを取得
	/// </summary>
	/// <returns>ウィンドウのサイズ</returns>
	const Size& GetWindowSize() const;

	/// <summary>
	/// FPSを取得
	/// </summary>
	/// <returns>FPS</returns>
	int GetFps() const;

	/// <summary>
	/// アプリケーションを終了する
	/// </summary>
	void Exit();

private:
	/// <summary>
	/// コンストラクタ
	/// シングルトンのためprivate
	/// </summary>
	Application();

	/// <summary>
	/// コピーコンストラクタ禁止
	/// </summary>
	/// <param name="app">コピー元のApplicationクラスのインスタンス</param>
	Application(const Application& app) = delete;

	/// <summary>
	/// 代入演算子禁止
	/// </summary>
	/// <param name="app">コピー元のApplicationクラスのインスタンス</param>
	void operator=(const Application& app) = delete;

	/// <summary>
	/// 終了処理
	/// </summary>
	void End();

private:
	// ウィンドウのサイズ
	Size m_screenSize;

	// 終了フラグ
	bool m_isExit;
};