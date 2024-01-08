#pragma once
#include <memory>
#include <list>

// プロトタイプ宣言
class SceneBase;

/// <summary>
/// 各シーンを管理するクラス
/// </summary>
class SceneManager
{
public:
	// コンストラクタ
	SceneManager();

	// デストラクタ
	~SceneManager();

	// 更新
	void Update();

	// 描画
	void Draw();

	/// <summary>
	/// シーンの切り替え
	/// </summary>
	/// <param name="scene">切り替えたいシーンのアドレス</param>
	void ChangeScene(std::shared_ptr<SceneBase> nextScene);

	/// <summary>
	/// 現在のシーンの上にシーンを積む(ポーズ)
	/// Updateで実行されるのは上につまれたシーン
	/// </summary>
	/// <param name="scene">上に積みたいシーンのアドレス</param>
	void PushScene(std::shared_ptr<SceneBase> scene);

	// 一番上のシーンを削除
	// ポーズ画面を消すときに使う
	void PopScene();

private:
	// シーン
	std::list<std::shared_ptr<SceneBase>> m_scenes;
};