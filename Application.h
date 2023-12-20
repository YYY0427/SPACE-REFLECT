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

	// 初期化
	bool Init();

	// 実行
	void Run();

	// 後処理
	void End();
	
	// ウィンドウのサイズを取得
	const Size& GetWindowSize() const;

private:
	// コンストラクタ
	// シングルトンのためprivate
	Application();

	// シングルトンのためコピー禁止
	Application(const Application& app) = delete;

	// シングルトンのため代入禁止
	void operator=(const Application& app) = delete;

	// ウィンドウのサイズ
	Size m_screenSize;
};