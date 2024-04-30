#pragma once
#include <memory>
#include <list>
#include <DxLib.h>

// プロトタイプ宣言
class SceneBase;

/// <summary>
/// 各シーンを管理するクラス
/// </summary>
class SceneManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneManager();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// シーンの切り替え
	/// </summary>
	/// <param name="scene">切り替えたいシーンのアドレス</param>
	void ChangeScene(const std::shared_ptr<SceneBase>& nextScene);

	/// <summary>
	/// シーンを全て削除してから新しいシーンを追加
	/// </summary>
	/// <param name="nextScene"></param>
	void ChangeAndClearScene(const std::shared_ptr<SceneBase>& nextScene);

	/// <summary>
	/// 現在のシーンの上にシーンを積む(ポーズ)
	/// Updateで実行されるのは上につまれたシーン
	/// </summary>
	/// <param name="scene">上に積みたいシーンのアドレス</param>
	void PushScene(const std::shared_ptr<SceneBase>& scene);

	/// <summary>
	/// 一番上のシーンを削除 
	/// </summary>
	void PopScene();

	/// <summary>
	/// シーンを全て削除
	/// </summary>
	void ClearScene();

	/// <summary>
	/// FPSを描画
	/// </summary>
	void DrawFps();

	/// <summary>
	/// 更新時間を描画
	/// </summary>
	void DrawUpdateTime();

	/// <summary>
	/// 描画時間を描画
	/// </summary>
	void DrwaDrawTime();

private:
	// シーン
	std::list<std::shared_ptr<SceneBase>> m_scenes;

	// シーンの更新時間
	LONGLONG m_updateTime;

	// シーンの描画時間
	LONGLONG m_drawTime;

	// fps
	float m_prevFps;
};